#ifndef INC_STM32F446RE_GPIO_H_
#define INC_STM32F446RE_GPIO_H_

#include <stm32f446re.h>
#include <stdint.h>

typedef struct {

	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinOTYPE;
	uint8_t GPIO_PinPUPD;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinALTFMode;

} GPIO_PINCONFIG;

typedef struct {

	GPIO_REGDEF *pGPIOX;
	GPIO_PINCONFIG GPIOX_PINCONFIG;

} GPIO_Handle;


#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_ALTFN 2
#define GPIO_MODE_ANALOG 3
#define GPIO_MODE_IT_FT 4
#define GPIO_MODE_IT_RT 5
#define GPIO_MODE_IT_RFT 6

#define GPIO_OTYPE_PUSHPULL 0
#define GPIO_OTYPE_OPENDRAIN 1

#define GPIO_SPEED_LOW 0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_FAST 2
#define GPIO_SPEED_HIGH 3

#define GPIO_PUPD_NO 0
#define GPIO_PUPD_PU 1
#define GPIO_PUPD_PD 2

#define GPIO_PIN_0 0
#define GPIO_PIN_1 1
#define GPIO_PIN_2 2
#define GPIO_PIN_3 3
#define GPIO_PIN_4 4
#define GPIO_PIN_5 5
#define GPIO_PIN_6 6
#define GPIO_PIN_7 7
#define GPIO_PIN_8 8
#define GPIO_PIN_9 9
#define GPIO_PIN_10 10
#define GPIO_PIN_11 11
#define GPIO_PIN_12 12
#define GPIO_PIN_13 13
#define GPIO_PIN_14 14
#define GPIO_PIN_15 15


void GPIO_PERICLK(GPIO_REGDEF *pGPIOX, uint8_t state);

void GPIO_INIT(GPIO_Handle *pGPIO_Handle) ;

void GPIO_DEINIT(GPIO_REGDEF *pGPIOX);

uint8_t GPIO_READPIN(GPIO_REGDEF *pGPIOX, uint8_t PinNumber);

uint16_t GPIO_READPORT(GPIO_REGDEF *pGPIOX);

void GPIO_WRITEPIN(GPIO_REGDEF *pGPIOX, uint8_t PinNumber, uint8_t VALUE);

void GPIO_WRITEPORT(GPIO_REGDEF *pGPIOX, uint16_t VALUE);

void GPIO_TOGGLEPIN(GPIO_REGDEF *pGPIOX, uint8_t PinNumber);

void GPIO_IRQCONFIG(uint8_t IRQNumber, uint32_t state);

void GPIO_IRQPRIORITY(uint8_t IRQNumber, uint8_t Priority);

void GPIO_IRQHANDLE(uint8_t PinNumber);


//TODO
//1.handle invalid inputs
//2.handle uninitialized inputs and inactive registers using memset

#endif
