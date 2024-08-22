#ifndef _DS1302_H
#define _DS1302_H
#include "main.h"
#include <stdbool.h>

// Some useful address
#define DS1302_SECONDS           0x80
#define DS1302_MINUTES           0x82
#define DS1302_HOURS             0x84
#define DS1302_DATE              0x86
#define DS1302_MONTH             0x88
#define DS1302_DAY               0x8A
#define DS1302_YEAR              0x8C
#define DS1302_CLOCK_BURST       0xBE
#define DS1302_CLOCK_BURST_WRITE 0xBE
#define DS1302_CLOCK_BURST_READ  0xBF
#define DS1302_RAMSTART          0xC0
#define DS1302_RAMEND            0xFC
#define DS1302_RAM_BURST         0xFE
#define DS1302_RAM_BURST_WRITE   0xFE
#define DS1302_RAM_BURST_READ    0xFF


typedef enum {AM, PM, NONE} MeridiemEnum;
typedef enum {MON=1, TUE, WED, THU, FRI, SAT, SUN} DaysEnum;

typedef struct{
    GPIO_TypeDef* port;
    uint16_t pin;
}GpioPin;

typedef struct{
    int hour;
    MeridiemEnum meridiem;
}Hour;

typedef struct{
    int sec;
    int min;
    Hour hour;
    int date;
    int month;
    int year;
    DaysEnum day;
}DS1302_TimeRecord;

typedef struct{
    GpioPin CE_Pin;
    GpioPin IO_Pin;
    GpioPin SCLK_Pin;
}DS1302_HandelTypeDef;

// DS1302 setup and operational funtions
void ds1302_init(DS1302_HandelTypeDef* handel);
uint8_t ds1302_readByte(DS1302_HandelTypeDef* handel, uint8_t address);
void ds1302_writeByte(DS1302_HandelTypeDef* handel, uint8_t data, uint8_t address);
void ds1302_burstRead(DS1302_HandelTypeDef* handel, uint8_t* buffer, uint8_t size, bool ram);
void ds1302_burstWrite(DS1302_HandelTypeDef* handel, uint8_t* buffer, uint8_t size, bool ram);

// Various funtions to fetch time info 
int ds1302_getSecond(DS1302_HandelTypeDef* handel);
int ds1302_getMinute(DS1302_HandelTypeDef* handel);
Hour ds1302_getHour(DS1302_HandelTypeDef* handel);
int ds1302_getDate(DS1302_HandelTypeDef* handel);
int ds1302_getMonth(DS1302_HandelTypeDef* handel);
int ds1302_getYear(DS1302_HandelTypeDef* handel);
DaysEnum ds1302_getDay(DS1302_HandelTypeDef* handel);
void ds1302_getUpdateDateTime(DS1302_HandelTypeDef* handel, DS1302_TimeRecord* datetime);
DS1302_TimeRecord ds1302_getDateTime(DS1302_HandelTypeDef* handel);

// Various funtions to set time info 
bool ds1302_setSecond(DS1302_HandelTypeDef* handel, uint8_t sec);
bool ds1302_setMinute(DS1302_HandelTypeDef* handel, uint8_t min);
bool ds1302_setHour(DS1302_HandelTypeDef* handel, Hour hour);
bool ds1302_setDate(DS1302_HandelTypeDef* handel, uint8_t date);
bool ds1302_setMonth(DS1302_HandelTypeDef* handel, uint8_t month);
bool ds1302_setYear(DS1302_HandelTypeDef* handel, uint8_t year);
bool ds1302_setDay(DS1302_HandelTypeDef* handel, DaysEnum day);
bool ds1302_setDateTime(DS1302_HandelTypeDef* handel, DS1302_TimeRecord datetime);

// Funtions to read write to ram address
void ds1302_writeByteRam(DS1302_HandelTypeDef* handel, uint8_t address, uint8_t data);
uint8_t ds1302_readByteRam(DS1302_HandelTypeDef* handel, uint8_t address);
void ds1302_burstWriteRam(DS1302_HandelTypeDef* handel, uint8_t* buffer, uint8_t size);
void ds1302_burstReadRam(DS1302_HandelTypeDef* handel, uint8_t* buffer, uint8_t size);

// Some other useful funtions
void ds1302_setClockHalt(DS1302_HandelTypeDef* handel, bool halt);
bool ds1302_isClockHalted(DS1302_HandelTypeDef* handel);
void ds1302_set12HourMode(DS1302_HandelTypeDef* handel);
void ds1302_set24HourMode(DS1302_HandelTypeDef* handel);

#endif
