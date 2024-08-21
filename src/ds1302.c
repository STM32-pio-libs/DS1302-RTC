#include "main.h"
#include "ds1302.h"

static void DelayUs(uint32_t us) {
   for (uint32_t i = 0; i < us * (HAL_RCC_GetHCLKFreq() / 1000000) / 3; i++);
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

