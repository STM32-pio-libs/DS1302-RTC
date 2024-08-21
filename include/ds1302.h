#ifndef _DS1302_H
#define _DS1302_H
#include "main.h"

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
