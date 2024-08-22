# DS1302 RTC Driver Documentation

This document provides an overview and reference for the DS1302 RTC driver implemented for STM32 microcontrollers using the STM32Cube HAL library.

## Data Structures

### `GpioPin`
Represents a GPIO pin configuration.

```c
typedef struct{
    GPIO_TypeDef* port;
    uint16_t pin;
} GpioPin;
```

- **port**: GPIO port (e.g., `GPIOA`).
- **pin**: GPIO pin number (e.g., `GPIO_PIN_5`).

### `Hour`
Represents the hour with optional AM/PM indication.

```c
typedef struct{
    int hour;
    MeridiemEnum meridiem;
} Hour;
```

- **hour**: The hour value (0-23 or 1-12).
- **meridiem**: Enum indicating AM, PM, or NONE for 24-hour mode.

### `DS1302_TimeRecord`
Represents a complete date and time record.

```c
typedef struct{
    int sec;
    int min;
    Hour hour;
    int date;
    int month;
    int year;
    DaysEnum day;
} DS1302_TimeRecord;
```

- **sec**: Seconds (0-59).
- **min**: Minutes (0-59).
- **hour**: `Hour` structure representing the hour and meridiem.
- **date**: Day of the month (1-31).
- **month**: Month (1-12).
- **year**: Year (0-99).
- **day**: Enum indicating the day of the week.

### `DS1302_HandelTypeDef`
Represents the hardware interface to the DS1302 RTC.

```c
typedef struct{
    GpioPin CE_Pin;
    GpioPin IO_Pin;
    GpioPin SCLK_Pin;
} DS1302_HandelTypeDef;
```

- **CE_Pin**: Chip Enable pin configuration.
- **IO_Pin**: I/O pin configuration.
- **SCLK_Pin**: Serial Clock pin configuration.

## Functions

### Initialization

#### `void ds1302_init(DS1302_HandelTypeDef* handel)`
Initializes the GPIO Pins to interface with DS1302

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.

### Read/Write Operations

#### `uint8_t ds1302_readByte(DS1302_HandelTypeDef* handel, uint8_t address)`
Reads a byte from the DS1302 at the specified address.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **address**: Address to read from.

#### `void ds1302_writeByte(DS1302_HandelTypeDef* handel, uint8_t data, uint8_t address)`
Writes a byte to the DS1302 at the specified address.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **data**: Data byte to write.
- **address**: Address to write to.

#### `void ds1302_burstRead(DS1302_HandelTypeDef* handel, uint8_t* buffer, uint8_t size, bool ram)`
Performs a burst read operation from the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **buffer**: Pointer to a buffer to store the read data.
- **size**: Number of bytes to read.
- **ram**: Set to `true` to read from RAM, `false` to read from clock registers.

#### `void ds1302_burstWrite(DS1302_HandelTypeDef* handel, uint8_t* buffer, uint8_t size, bool ram)`
Performs a burst write operation to the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **buffer**: Pointer to the buffer containing data to write.
- **size**: Number of bytes to write.
- **ram**: Set to `true` to write to RAM, `false` to write to clock registers.

### Time Retrieval

#### `int ds1302_getSecond(DS1302_HandelTypeDef* handel)`
Returns the current seconds from the DS1302.

#### `int ds1302_getMinute(DS1302_HandelTypeDef* handel)`
Returns the current minutes from the DS1302.

#### `Hour ds1302_getHour(DS1302_HandelTypeDef* handel)`
Returns the current hour from the DS1302.

#### `int ds1302_getDate(DS1302_HandelTypeDef* handel)`
Returns the current date from the DS1302.

#### `int ds1302_getMonth(DS1302_HandelTypeDef* handel)`
Returns the current month from the DS1302.

#### `int ds1302_getYear(DS1302_HandelTypeDef* handel)`
Returns the current year from the DS1302.

#### `DaysEnum ds1302_getDay(DS1302_HandelTypeDef* handel)`
Returns the current day of the week from the DS1302.

#### `void ds1302_getUpdateDateTime(DS1302_HandelTypeDef* handel, DS1302_TimeRecord* datetime)`
Fetches the current date and time from the DS1302 and updates the provided `DS1302_TimeRecord` structure.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **datetime**: Pointer to a `DS1302_TimeRecord` structure to be updated.

#### `DS1302_TimeRecord ds1302_getDateTime(DS1302_HandelTypeDef* handel)`
Returns the current date and time from the DS1302 as a `DS1302_TimeRecord`.

### Time Setting

#### `bool ds1302_setSecond(DS1302_HandelTypeDef* handel, uint8_t sec)`
Sets the seconds on the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **sec**: Seconds value to set (0-59).

#### `bool ds1302_setMinute(DS1302_HandelTypeDef* handel, uint8_t min)`
Sets the minutes on the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **min**: Minutes value to set (0-59).

#### `bool ds1302_setHour(DS1302_HandelTypeDef* handel, Hour hour)`
Sets the hour on the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **hour**: `Hour` structure to set.

#### `bool ds1302_setDate(DS1302_HandelTypeDef* handel, uint8_t date)`
Sets the date on the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **date**: Date value to set (1-31).

#### `bool ds1302_setMonth(DS1302_HandelTypeDef* handel, uint8_t month)`
Sets the month on the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **month**: Month value to set (1-12).

#### `bool ds1302_setYear(DS1302_HandelTypeDef* handel, uint8_t year)`
Sets the year on the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **year**: Year value to set (0-99).

#### `bool ds1302_setDay(DS1302_HandelTypeDef* handel, DaysEnum day)`
Sets the day of the week on the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **day**: Enum indicating the day of the week.

#### `bool ds1302_setDateTime(DS1302_HandelTypeDef* handel, DS1302_TimeRecord datetime)`
Sets the entire date and time on the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **datetime**: `DS1302_TimeRecord` structure containing the date and time to set.

### RAM Read/Write Operations

#### `void ds1302_writeByteRam(DS1302_HandelTypeDef* handel, uint8_t address, uint8_t data)`
Writes a byte to the DS1302 RAM.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **address**: RAM address to write to.
- **data**: Data byte to write.

#### `uint8_t ds1302_readByteRam(DS1302_HandelTypeDef* handel, uint8_t address)`
Reads a byte from the DS1302 RAM.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **address**: RAM address to read from.

#### `void ds1302_burstWriteRam(DS1302_HandelTypeDef* handel, uint8_t* buffer, uint8_t size)`
Performs a burst write operation to the DS1302 RAM.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **buffer**: Pointer to the buffer containing data to write.
- **size**: Number of bytes to write.

#### `void ds1302_burstReadRam(DS1302_HandelTypeDef* handel, uint8_t* buffer, uint8_t size)`
Performs a burst read operation from the DS1302 RAM.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **buffer**: Pointer to a buffer to store the read data.
- **size**: Number of bytes to read.

### Other Functions

#### `void ds1302_setClockHalt(DS1302_HandelTypeDef* handel, bool halt)`
Sets the clock halt status on the DS1302.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.
- **halt**: `true` to halt the clock, `false` to resume.

#### `bool ds1302_isClockHalted(DS1302_HandelTypeDef* handel)`
Checks if the DS1302 clock is halted.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.

#### `void ds1302_set12HourMode(DS1302_HandelTypeDef* handel)`
Sets the DS1302 to 12-hour mode.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.

#### `void ds1302_set24HourMode(DS1302_HandelTypeDef* handel)`
Sets the DS1302 to 24-hour mode.

- **handel**: Pointer to a `DS1302_HandelTypeDef` structure.

## Constants

### DS1302 Addresses

- `DS1302_SECONDS`: Address for seconds register (0x80).
- `DS1302_MINUTES`: Address for minutes register (0x82).
- `DS1302_HOURS`: Address for hours register (0x84).
- `DS1302_DATE`: Address for date register (0x86).
- `DS1302_MONTH`: Address for month register (0x88).
- `DS1302_DAY`: Address for day register (0x8A).
- `DS1302_YEAR`: Address for year register (0x8C).
- `DS1302_CLOCK_BURST`: Address for clock burst mode (0xBE).
- `DS1302_CLOCK_BURST_WRITE`: Address for clock burst write (0xBE).
- `DS1302_CLOCK_BURST_READ`: Address for clock burst read (0xBF).
- `DS1302_RAMSTART`: Address for the start of RAM (0xC0).
- `DS1302_RAMEND`: Address for the end of RAM (0xFC).
- `DS1302_RAM_BURST`: Address for RAM burst mode (0xFE).
- `DS1302_RAM_BURST_WRITE`: Address for RAM burst write (0xFE).
- `DS1302_RAM_BURST_READ`: Address for RAM burst read (0xFF).

### Enums

#### `MeridiemEnum`

- `AM`: AM time.
- `PM`: PM time.
- `NONE`: No meridiem (24-hour mode).

#### `DaysEnum`

- `MON`: Monday.
- `TUE`: Tuesday.
- `WED`: Wednesday.
- `THU`: Thursday.
- `FRI`: Friday.
- `SAT`: Saturday.
- `SUN`: Sunday.
