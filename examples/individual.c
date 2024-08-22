#include <stdio.h>
#include "main.h"
#include "ds1302.h"

const char* DaysName[] = {0, "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

int main(void){
    /* 
        System Initialisation Block
    */

    // Set the GPIO pins for DS1302 rtc module
    DS1302_HandelTypeDef rtc = {
        .CE_Pin = {GPIOA, GPIO_PIN_9},
        .SCLK_Pin = {GPIOB, GPIO_PIN_10},
        .IO_Pin = {GPIOA, GPIO_PIN_8}
    };
   
    // The init funtion sets CE, SCLK and IO pin for GPIO Output
    ds1302_init(&rtc); 

    bool status; // Each set function returns a bool where false means invalid data
    status = ds1302_setSecond(&rtc, 10);
    status = ds1302_setMinute(&rtc, 23);
    status = ds1302_setHour(&rtc, (Hour){.hour=11, .meridiem=AM}); // Set meridiem to NONE if using 24 hrs clock
    status = ds1302_setDate(&rtc, 22);
    status = ds1302_setMonth(&rtc, 8);
    status = ds1302_setYear(&rtc, 24);
    status = ds1302_setDay(&rtc, TUE);


    while (1){
        DS1302_TimeRecord now;
        now.sec = ds1302_getSecond(&rtc);
        now.min = ds1302_getMinute(&rtc);
        now.hour = ds1302_getHour(&rtc);
        now.date = ds1302_getDate(&rtc);
        now.month = ds1302_getMonth(&rtc);
        now.year = ds1302_getYear(&rtc);
        now.day = ds1302_getDay(&rtc);

        printf("%02d-%02d-%02d %s %02d:%02d:%02d", now.date, now.month, now.year, DaysName[now.day],
                                        now.hour.hour, now.min, now.sec);
        if(now.hour.meridiem != NONE){
            printf(" %s", (now.hour.meridiem == AM)?"AM":"PM");
        }
        printf("\n");

        HAL_Delay(500);
    }
}
