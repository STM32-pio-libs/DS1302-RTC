#include "main.h"
#include "ds1302.h"
#include <stdio.h>
#include <stdbool.h>

#define DECTOBCD(value) ((value/10) << 4) | (value % 10)
#define BCDTODEC(value) (((value&0b11110000)>>4)*10 + (value & 0b00001111))

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

static void ds1302_enableWriteMode(DS1302_HandelTypeDef* handel){
    GPIO_InitTypeDef gpioinit = {
        .Pin = handel->IO_Pin.pin,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW
    };
    HAL_GPIO_Init(handel->IO_Pin.port, &gpioinit);
}

static void ds1302_enableReadMode(DS1302_HandelTypeDef* handel){
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
    uint8_t sec_byte = ds1302_readByte(handel, DS1302_SECONDS);
    sec_byte &= 0b01111111;
    return BCDTODEC(sec_byte);
}


int ds1302_getMinute(DS1302_HandelTypeDef* handel){
    uint8_t min_byte = ds1302_readByte(handel, DS1302_MINUTES);
    return BCDTODEC(min_byte);
}


Hour ds1302_getHour(DS1302_HandelTypeDef* handel){
    uint8_t hour_data_raw = ds1302_readByte(handel, DS1302_HOURS);
    Hour hour_data;
    int hour = hour_data_raw & 0b00001111;

    if((hour_data_raw & 0b10000000) >> 7){
        hour_data.meridiem = ((hour_data_raw & 0b00100000) >> 5) ? PM : AM;
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


int ds1302_getDate(DS1302_HandelTypeDef* handel){
    uint8_t date_byte = ds1302_readByte(handel, DS1302_DATE);
    return BCDTODEC(date_byte);
}


int ds1302_getMonth(DS1302_HandelTypeDef* handel){
    uint8_t month_byte = ds1302_readByte(handel, DS1302_MONTH);
    return BCDTODEC(month_byte);
}

int ds1302_getYear(DS1302_HandelTypeDef* handel){
    uint8_t year_byte = ds1302_readByte(handel, DS1302_YEAR);
    return BCDTODEC(year_byte);
}

DaysEnum ds1302_getDay(DS1302_HandelTypeDef* handel){
    uint8_t day_byte = ds1302_readByte(handel, DS1302_DAY);
    return BCDTODEC(day_byte);
}

bool ds1302_setSecond(DS1302_HandelTypeDef* handel, uint8_t sec){
    if(sec > 59) return false;
    ds1302_writeByte(handel, DECTOBCD(sec), DS1302_SECONDS);
    return true;
}

bool ds1302_setMinute(DS1302_HandelTypeDef* handel, uint8_t min){
    if(min > 59) return false;
    ds1302_writeByte(handel, DECTOBCD(min), DS1302_MINUTES);
    return true;
}

bool ds1302_setHour(DS1302_HandelTypeDef* handel, Hour hour){
    if(hour.meridiem == NONE && hour.hour > 23) return false;
    if(hour.meridiem != NONE && hour.hour > 12) return false;
    uint8_t hour_data = DECTOBCD(hour.hour);
    if(hour.meridiem != NONE){
        hour_data |= 0b10000000;
        if(hour.meridiem == PM) hour_data |= 0b00100000;
    }
    ds1302_writeByte(handel, hour_data, DS1302_HOURS);
    return true;
}

bool ds1302_setDate(DS1302_HandelTypeDef* handel, uint8_t date){
    if(date > 31) return false;
    ds1302_writeByte(handel, DECTOBCD(date), DS1302_DATE);
    return true;
}

bool ds1302_setMonth(DS1302_HandelTypeDef* handel, uint8_t month){
    if(month > 12) return false;
    ds1302_writeByte(handel, DECTOBCD(month), DS1302_MONTH);
    return true;
}

bool ds1302_setYear(DS1302_HandelTypeDef* handel, uint8_t year){
    if(year > 99) return false;
    ds1302_writeByte(handel, DECTOBCD(year), DS1302_YEAR);
    return true;
}

bool ds1302_setDay(DS1302_HandelTypeDef* handel, DaysEnum day){
    if(day > 7) return false;
    ds1302_writeByte(handel, day, DS1302_DAY);
    return true;
}

bool ds1302_setDateTime(DS1302_HandelTypeDef* handel, DS1302_TimeRecord datetime){
    if(!ds1302_setSecond(handel, datetime.sec)) return false;
    if(!ds1302_setMinute(handel, datetime.min)) return false;
    if(!ds1302_setHour(handel, datetime.hour)) return false;
    if(!ds1302_setDate(handel, datetime.date)) return false;
    if(!ds1302_setMonth(handel, datetime.month)) return false;
    if(!ds1302_setYear(handel, datetime.year)) return false;
    if(!ds1302_setDay(handel, datetime.day)) return false;
    return true;
}


void ds1302_getUpdateDateTime(DS1302_HandelTypeDef* handel, DS1302_TimeRecord* datetime){
    uint8_t buffer[8];
    ds1302_burstRead(handel, buffer);
    buffer[0] &= 0b01111111;
    datetime->sec = BCDTODEC(buffer[0]);
    datetime->min = BCDTODEC(buffer[1]);
    datetime->hour = ds1302_getHour(handel);
    datetime->date = BCDTODEC(buffer[3]);
    datetime->month = BCDTODEC(buffer[4]);
    datetime->day = BCDTODEC(buffer[5]);
    datetime->year = BCDTODEC(buffer[6]);
}

DS1302_TimeRecord ds1302_getDateTime(DS1302_HandelTypeDef* handel){
    DS1302_TimeRecord datetime;
    ds1302_getUpdateDateTime(handel, &datetime);
    return datetime;
}


void ds1302_burstRead(DS1302_HandelTypeDef* handel, uint8_t* buffer){
    uint8_t address = DS1302_CLOCK_BURST_READ | 0x01;
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

    for(int j=0; j<8; j++){
        buffer[j] = 0;
        for(int i=0; i<8; i++){
            HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
            DelayUs(1);
            if(HAL_GPIO_ReadPin(handel->IO_Pin.port, handel->IO_Pin.pin) == GPIO_PIN_SET){
                buffer[j] |= (1 << i);
            }
            DelayUs(1);
            HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_SET);
            DelayUs(1);
        }
    }

    HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
    DelayUs(1);
    ds1302_enableWriteMode(handel);
}


void ds1302_setClockHalt(DS1302_HandelTypeDef* handel, bool halt){
    uint8_t second = ds1302_readByte(handel, DS1302_SECONDS);
    if (halt) {
        second |= 0b10000000;
    } else {
        second &= 0b01111111;
    }
    ds1302_writeByte(handel, second, DS1302_SECONDS);
}

bool ds1302_isClockHalted(DS1302_HandelTypeDef* handel){
    uint8_t second = ds1302_readByte(handel, DS1302_SECONDS);
    return second >> 7; 
}

void ds1302_set12HourMode(DS1302_HandelTypeDef* handel){
    Hour hour = ds1302_getHour(handel);
    if(hour.meridiem != NONE) return;
    hour.meridiem = (hour.hour >= 12) ? PM : AM;
    hour.hour = (hour.hour % 12 == 0) ? 12 : hour.hour % 12;;
    ds1302_setHour(handel, hour);
}

void ds1302_set24HourMode(DS1302_HandelTypeDef* handel){
    Hour hour = ds1302_getHour(handel);
    if(hour.meridiem == NONE) return;
    hour.hour += (hour.meridiem == PM && hour.hour != 12) ? 12 : 0;
    hour.hour = (hour.meridiem == AM && hour.hour == 12) ? 0 : hour.hour;
    hour.meridiem = NONE;
    ds1302_setHour(handel, hour);
}
