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

    // Create a timerecord to set the datetime
    DS1302_TimeRecord datetime = {
        .sec = 10,
        .min = 23,
        .hour = {.hour=11, .meridiem=AM}, // Set meridiem to NONE if using 24 hrs clock
        .date = 22,
        .month = 8,
        .year = 24,
        .day = TUE
    };

    // Set the time in rtc module
    bool status = ds1302_setDateTime(&rtc, datetime);
    if(status == false){
        printf("Invalid Data entered!");
    }

    while (1){
        // Fetch current datetime, 
        // NOTE: Alternately you can use ds1302_getUpdateDateTime(handel, DS1302_TimeRecord* datetime)
        DS1302_TimeRecord now = ds1302_getDateTime(&rtc);   

        // Print the datetime
        printf("%02d-%02d-%02d %s %02d:%02d:%02d", now.date, now.month, now.year, DaysName[now.day],
                                        now.hour.hour, now.min, now.sec);
        if(now.hour.meridiem != NONE) printf(" %s", (now.hour.meridiem == AM)?"AM":"PM");
        printf("\n");

        HAL_Delay(500);
    }
}

