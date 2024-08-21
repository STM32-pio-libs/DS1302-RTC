#include "main.h"
#include "ds1302.h"
#include <stdio.h>

static void DelayUs(uint32_t us) {
   for (uint32_t i = 0; i < us * (HAL_RCC_GetHCLKFreq() / 10000000) / 3; i++);
}

void ds1302_init(DS1302_HandelTypeDef* handel){
    HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);

    GPIO_InitTypeDef gpioinit = {
        .Pin = handel->IO_Pin.pin,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW
    };

    HAL_GPIO_Init(handel->IO_Pin.port, &gpioinit);
    
    gpioinit.Pin = handel->CE_Pin.pin;
    HAL_GPIO_Init(handel->CE_Pin.port, &gpioinit);

    gpioinit.Pin = handel->SCLK_Pin.pin;
    HAL_GPIO_Init(handel->SCLK_Pin.port, &gpioinit);
}

void ds1302_enableWriteMode(DS1302_HandelTypeDef* handel){
    GPIO_InitTypeDef gpioinit = {
        .Pin = handel->IO_Pin.pin,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW
    };
    HAL_GPIO_Init(handel->IO_Pin.port, &gpioinit);
}

void ds1302_enableReadMode(DS1302_HandelTypeDef* handel){
    GPIO_InitTypeDef gpioinit = {
        .Pin = handel->IO_Pin.pin,
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
    };
    HAL_GPIO_Init(handel->IO_Pin.port, &gpioinit);
}

uint8_t ds1302_readByte(DS1302_HandelTypeDef* handel, uint8_t address){
    uint8_t data = 0;
    address |= 0x01;
    HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_SET);
    
    // Send the address
    for(int i=0; i<8; i++){
        HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
        DelayUs(1);
        HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin, 
                            GET_BIT(address, i) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        DelayUs(1);
        HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_SET);
        DelayUs(1);
    }

    ds1302_enableReadMode(handel);

    // Read the data byte
    for(int i=0; i<8; i++){
        HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
        DelayUs(1);
        if(HAL_GPIO_ReadPin(handel->IO_Pin.port, handel->IO_Pin.pin) == GPIO_PIN_SET){
            data |= (1 << i);
        }
        DelayUs(1);
        HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_SET);
        DelayUs(1);
    }

    HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
    DelayUs(1);
    ds1302_enableWriteMode(handel);
    return data;
}


void ds1302_writeByte(DS1302_HandelTypeDef* handel, uint8_t data, uint8_t address){
    HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_SET);
    
    // Send the address
    for(int i=0; i<8; i++){
        HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
        DelayUs(1);
        HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin, 
                            GET_BIT(address, i) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        DelayUs(1);
        HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_SET);
        DelayUs(1);
    }

    // Send the data
    for(int i=0; i<8; i++){
        HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
        DelayUs(1);
        HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin, 
                            GET_BIT(data, i) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        DelayUs(1);
        HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_SET);
        DelayUs(1);
    }

    HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
    DelayUs(1);
}

int ds1302_getSecond(DS1302_HandelTypeDef* handel){
    uint8_t sec_data = ds1302_readByte(handel, DS1302_SECONDS);
    int sec = sec_data & 0b00001111;
    int sec10 = (sec_data & 0b01110000) >> 4;
    return sec10*10 + sec;
}

int ds1302_getMinute(DS1302_HandelTypeDef* handel){
    uint8_t min_data = ds1302_readByte(handel, DS1302_MINUTES);
    int min = min_data & 0b00001111;
    int min10 = (min_data & 0b01110000) >> 4;
    return min10*10 + min;
}

Hour ds1302_getHour(DS1302_HandelTypeDef* handel){
    uint8_t hour_data_raw = ds1302_readByte(handel, DS1302_HOURS);
    Hour hour_data;
    int hour = hour_data_raw & 0b00001111;

    if((hour_data_raw & 0b10000000) >> 7){
        hour_data.meridiem = ((hour_data_raw & 0b00010000) >> 5) ? PM : AM;
        int hour10 = (hour_data_raw & 0b00010000) >> 4;
        hour_data.hour = hour10*10 + hour;
    }
    else{
        hour_data.meridiem = NONE;
        int hour10 = (hour_data_raw & 0b00110000) >> 4;
        hour_data.hour = hour10*10 + hour;
    }

    return hour_data;
}
