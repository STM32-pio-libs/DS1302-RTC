#ifndef _DS1302_H
#define _DS1302_H
#include "main.h"
#include <stdbool.h>

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

#define GET_BIT(value, bit) (((value) >> (bit)) & 0x01)

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


void ds1302_init(DS1302_HandelTypeDef* handel);
uint8_t ds1302_readByte(DS1302_HandelTypeDef* handel, uint8_t address);
void ds1302_writeByte(DS1302_HandelTypeDef* handel, uint8_t data, uint8_t address);
int ds1302_getSecond(DS1302_HandelTypeDef* handel);
int ds1302_getMinute(DS1302_HandelTypeDef* handel);
Hour ds1302_getHour(DS1302_HandelTypeDef* handel);
int ds1302_getDate(DS1302_HandelTypeDef* handel);
int ds1302_getMonth(DS1302_HandelTypeDef* handel);
int ds1302_getYear(DS1302_HandelTypeDef* handel);
DaysEnum ds1302_getDay(DS1302_HandelTypeDef* handel);
bool ds1302_setSecond(DS1302_HandelTypeDef* handel, uint8_t sec);
bool ds1302_setMinute(DS1302_HandelTypeDef* handel, uint8_t min);
bool ds1302_setHour(DS1302_HandelTypeDef* handel, Hour hour);;
bool ds1302_setDate(DS1302_HandelTypeDef* handel, uint8_t date);
bool ds1302_setMonth(DS1302_HandelTypeDef* handel, uint8_t month);
bool ds1302_setYear(DS1302_HandelTypeDef* handel, uint8_t year);
bool ds1302_setDay(DS1302_HandelTypeDef* handel, DaysEnum day);

#endif
