# DS1302-RTC

This is a simple-to-use library to interface with the DS1302 RTC module written for STM32CubeHal.
<img src="https://github.com/user-attachments/assets/291db377-a329-4ae4-878a-7e571ff7136b" width="200">

### Features
1. Set or get individual time components. eg: ds1302_getSecond, ds1302_setDay...
2. Set or get all time component using struct `DS1302_TimeRecord`
3. Halt or unhalt clock
4. Transition 12/24 hour mode
5. Read write to Ram address individually or in burst mode

> NOTICE⚠️: This library expects you to have a file named `main.h` in your include path which should contain an include directive for your stm32hal header such as `stm32f4xx_hal.h`

#### Checkout Examples [here](https://github.com/STM32-pio-libs/DS1302-RTC/tree/main/examples)
#### Checkout Documentation [here](https://github.com/STM32-pio-libs/DS1302-RTC/blob/main/Documentation.md)

