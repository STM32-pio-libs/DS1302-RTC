#ifndef _DS1302_H
#define _DS1302_H
#include "main.h"

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

typedef struct{
    GPIO_TypeDef* port;
    uint16_t pin;
}GpioPin;


typedef struct{
    GpioPin CE_Pin;
    GpioPin IO_Pin;
    GpioPin SCLK_Pin;
}DS1302_HandelTypeDef;


void ds1302_init(DS1302_HandelTypeDef* handel);
uint8_t ds1302_readByte(DS1302_HandelTypeDef* handel, uint8_t address);
void ds1302_writeByte(DS1302_HandelTypeDef* handel, uint8_t data, uint8_t address);

#endif
