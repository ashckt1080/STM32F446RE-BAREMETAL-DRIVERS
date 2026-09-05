#ifndef INC_STM32F446RE_H_
#define INC_STM32F446RE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define ENABLE 1
#define SET 1
#define DISABLE 0
#define RESET 0
#define FLAG_SET 1
#define FLAG_RESET 0

//MEMORY BASE ADDRESSES

#define FLASH_BASEADDR 0x08000000U
#define SRAM1_BASEADDR 0x20000000U
#define SRAM2_BASEADDR 0x2001C000U
#define ROM_BASEADDR   0x1FFF0000U

//BUS DOMAIN BASE ADRESSES

#define APB1_BASEADDR 0x40000000U
#define APB2_BASEADDR 0x40010000U
#define AHB1_BASEADDR 0x40020000U
#define AHB2_BASEADDR 0x50000000U

//PROCESSOR NVIC BASE ADDRESSES

#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1 ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2 ((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3 ((volatile uint32_t*)0xE000E10C)

#define NVIC_ICER0 ((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1 ((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2 ((volatile uint32_t*)0xE000E188)
#define NVIC_ICER3 ((volatile uint32_t*)0xE000E18C)

#define NVIC_PR_BASEADDR ((volatile uint32_t*)0xE000E400)

//IRQ NUMBERS

#define IRQ_NO_EXTI0 6
#define IRQ_NO_EXTI1 7
#define IRQ_NO_EXTI2 8
#define IRQ_NO_EXTI3 9
#define IRQ_NO_EXTI4 10
#define IRQ_NO_EXTI9_5 23
#define IRQ_NO_EXTI15_10 40

#define IRQ_NO_SPI1 35
#define IRQ_NO_SPI2 36
#define IRQ_NO_SPI3 51
#define IRQ_NO_SPI4 84

//SYSTEM REGISTERS BASE ADDRESSES

#define RCC_BASEADDR    (AHB1_BASEADDR + 0x3800)
#define EXTI_BASEADDR   (APB2_BASEADDR + 0x3C00)
#define SYSCFG_BASEADDR (APB2_BASEADDR + 0x3800)

//GPIO BASE ADDRESSES AND REGISTER STRUCTURE

#define GPIOA_BASEADDR	(AHB1_BASEADDR + 0x0000)
#define GPIOB_BASEADDR	(AHB1_BASEADDR + 0x0400)
#define GPIOC_BASEADDR	(AHB1_BASEADDR + 0x0800)
#define GPIOD_BASEADDR	(AHB1_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR	(AHB1_BASEADDR + 0x1000)
#define GPIOF_BASEADDR	(AHB1_BASEADDR + 0x1400)
#define GPIOG_BASEADDR	(AHB1_BASEADDR + 0x1800)
#define GPIOH_BASEADDR	(AHB1_BASEADDR + 0x1C00)

typedef struct {

	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];

} GPIO_REGDEF;

#define GPIOA ((GPIO_REGDEF*)GPIOA_BASEADDR)
#define GPIOB ((GPIO_REGDEF*)GPIOB_BASEADDR)
#define GPIOC ((GPIO_REGDEF*)GPIOC_BASEADDR)
#define GPIOD ((GPIO_REGDEF*)GPIOD_BASEADDR)
#define GPIOE ((GPIO_REGDEF*)GPIOE_BASEADDR)
#define GPIOF ((GPIO_REGDEF*)GPIOF_BASEADDR)
#define GPIOG ((GPIO_REGDEF*)GPIOG_BASEADDR)
#define GPIOH ((GPIO_REGDEF*)GPIOH_BASEADDR)


//RCC REGISTER STRUCTURE AND PHERIPHERAL CLOCK CONTROL MACROS

typedef struct {

	volatile uint32_t CR;
	volatile uint32_t PLL_CFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1_RSTR;
	volatile uint32_t AHB2_RSTR;
	volatile uint32_t ABH3_RSTR;
	volatile uint32_t RESERVED0;
	volatile uint32_t APB1_RSTR;
	volatile uint32_t APB2_RSTR;
	volatile uint32_t RESERVED1;
	volatile uint32_t RESERVED2;
	volatile uint32_t AHB1_ENR;
	volatile uint32_t AHB2_ENR;
	volatile uint32_t ABH3_ENR;
	volatile uint32_t RESERVED3;
	volatile uint32_t APB1_ENR;
	volatile uint32_t APB2_ENR;
	volatile uint32_t RESERVED4;
	volatile uint32_t RESERVED5;
	volatile uint32_t AHB1_LPENR;
	volatile uint32_t AHB2_LPENR;
	volatile uint32_t AHB3_LPENR;
	volatile uint32_t RESERVED6;
	volatile uint32_t APB1_LPENR;
	volatile uint32_t APB2_LPENR;
	volatile uint32_t RESERVED7;
	volatile uint32_t RESERVED8;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t RESERVED9;
	volatile uint32_t RESERVED10;
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2S_CFGR;
	volatile uint32_t PLLSAI_CFGR;
	volatile uint32_t DCK_CFGR;
	volatile uint32_t CKGAT_ENR;
	volatile uint32_t DCK_CFGR2;

} RCC_REGDEF;

#define RCC ((RCC_REGDEF*)RCC_BASEADDR)

#define SYSCFG_PERICLK_EN()	(RCC->APB2_ENR |= (1 << 14))

//EXTI REGISTER STRUCTURE

typedef struct {

	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;

} EXTI_REGDEF;

#define EXTI ((EXTI_REGDEF*)EXTI_BASEADDR)


//SYSCFG REGISTER STRUCTURE

typedef struct {

	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	volatile uint32_t RESERVED1;
	volatile uint32_t RESERVED2;
	volatile uint32_t CMPCR;
	volatile uint32_t RESERVED3;
	volatile uint32_t RESERVED4;
	volatile uint32_t CFGR;


} SYSCFG_REGDEF;

#define SYSCFG ((SYSCFG_REGDEF*)SYSCFG_BASEADDR)

//SPI REGISTER BASE ADDRESSES

#define SPI1_BASEADDR (APB2_BASEADDR + 0x3000)
#define SPI2_BASEADDR (APB1_BASEADDR + 0x3800)
#define SPI3_BASEADDR (APB1_BASEADDR + 0x3C00)
#define SPI4_BASEADDR (APB2_BASEADDR + 0x3400)

#define SPI1_PERICLK_EN() (RCC->APB2_ENR |= 0x1 << 12 )
#define SPI2_PERICLK_EN() (RCC->APB1_ENR |= 0x1 << 14 )
#define SPI3_PERICLK_EN() (RCC->APB1_ENR |= 0x1 << 15 )
#define SPI4_PERICLK_EN() (RCC->APB2_ENR |= 0x1 << 13 )

#define SPI1_PERICLK_DI() (RCC->APB2_ENR &= ~(0x1 << 12 ))
#define SPI2_PERICLK_DI() (RCC->APB1_ENR &= ~(0x1 << 14 ))
#define SPI3_PERICLK_DI() (RCC->APB1_ENR &= ~(0x1 << 15 ))
#define SPI4_PERICLK_DI() (RCC->APB2_ENR &= ~(0x1 << 13 ))

typedef struct {

	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;

} SPI_REGDEF;

#define SPI1 ((SPI_REGDEF*)SPI1_BASEADDR)
#define SPI2 ((SPI_REGDEF*)SPI2_BASEADDR)
#define SPI3 ((SPI_REGDEF*)SPI3_BASEADDR)
#define SPI4 ((SPI_REGDEF*)SPI4_BASEADDR)

//INCLUDE OTHER DRIVER HEADERS
#include <stm32f446re_gpio.h>
#include <stm32f446re_spi.h>
#include <stm32f446re_i2c.h>

#endif
