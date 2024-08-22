#include <stdio.h>
#include <string.h>
#include "main.h"
#include "ds1302.h"

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

    // buffer to receive data and str to be sent and stored in ram of DS1302
    uint8_t buffer[31];
    char* str = "anurag3301";

    while (1){
        // Use burst write and read to write multiple bytes
        ds1302_burstWriteRam(&rtc, (uint8_t*)str, strlen(str)+1);
        ds1302_burstReadRam(&rtc, buffer, 30);
        printf("Data from burst read: %s\n", buffer);

        // read and write a single byte from an address
        uint8_t address = 20;
        ds1302_writeByteRam(&rtc, address, 0xC);
        printf("Data at Ram Address %d: %x\n", address, ds1302_readByteRam(&rtc, address));

        HAL_Delay(1000);
    }
}

