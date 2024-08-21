#ifndef _DS1302_H
#define _DS1302_H
#include "main.h"

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

#endif
