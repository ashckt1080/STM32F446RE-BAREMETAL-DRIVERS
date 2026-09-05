# STM32F446RE Baremetal Drivers

Baremetal peripheral drivers for the **STM32F446RE** microcontroller, written in C using direct register-level programming. The repository currently includes drivers for **GPIO, SPI, and EXTI interrupts**, and is intended for learning and experimenting with STM32 peripheral programming without relying on the STM32 HAL or LL libraries.

**MCU:** STM32F446RE
**Architecture:** ARM Cortex-M4


## Features

### GPIO

* GPIO peripheral clock control
* Input and output configuration
* Alternate function configuration
* Push-pull and open-drain output modes
* Configurable output speed
* Pull-up and pull-down configuration
* Pin and port read/write operations
* Pin toggle support
* External interrupt configuration
* Rising, falling, and both-edge triggering
* NVIC interrupt enable/disable and priority configuration

### SPI

* SPI peripheral clock control
* Master and slave configuration
* Full-duplex and Half-duplex communication
* Simplex receive mode
* 8-bit and 16-bit data frames
* Configurable SPI clock speed
* Configurable clock polarity (CPOL) and clock phase (CPHA)
* Software slave management
* Blocking transmit and receive
* Interrupt-driven transmit and receive
* Overrun error handling
* Application event callbacks

## Example Usage

`stm32f446re.h` contains the STM32F446RE peripheral base addresses, register definitions, IRQ numbers, and peripheral clock-control definitions used by the drivers. The peripheral-specific header files contain configuration structures, handles, macros, and public driver APIs. The drivers access the STM32F446RE memory-mapped peripheral registers directly.

Include the main device header:

```c
#include <stm32f446re.h>
```

Configure a GPIO pin:

```c
GPIO_Handle GPIO_LED;

GPIO_LED.pGPIOX = GPIOA;

GPIO_LED.GPIOX_PINCONFIG.GPIO_PinNumber = GPIO_PIN_5;
GPIO_LED.GPIOX_PINCONFIG.GPIO_PinMode = GPIO_MODE_OUTPUT;
GPIO_LED.GPIOX_PINCONFIG.GPIO_PinOTYPE = GPIO_OTYPE_PUSHPULL;
GPIO_LED.GPIOX_PINCONFIG.GPIO_PinPUPD = GPIO_PUPD_NO;
GPIO_LED.GPIOX_PINCONFIG.GPIO_PinSpeed = GPIO_SPEED_LOW;

GPIO_INIT(&GPIO_LED);
```

Write, Toggle, Read GPIO:

```c
GPIO_WRITEPIN(GPIOA, GPIO_PIN_5, SET);
GPIO_TOGGLEPIN(GPIOA, GPIO_PIN_5);

uint8_t state;
state = GPIO_READPIN(GPIOA, GPIO_PIN_5);
```

## Notes

This is a bare-metal learning project and is not intended to replace production-grade STM32 HAL or LL drivers.
Peripheral configuration and register behavior are based on the STM32F446RE hardware architecture.

