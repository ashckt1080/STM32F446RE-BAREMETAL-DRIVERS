#include <stm32f446re_gpio.h>
#include <stdint.h>

uint8_t GPIO_PORTCODE(GPIO_REGDEF *pGPIOX) {
    return ((uint32_t)pGPIOX - (uint32_t)GPIOA) / 0x400;
}

void GPIO_PERICLK(GPIO_REGDEF *pGPIOX, uint8_t state) {

	uint8_t PORT = ((uint32_t)pGPIOX - (uint32_t)GPIOA) / 0x400;

	if (state == ENABLE) {
	        RCC->AHB1_ENR |=  (0x1 << PORT);
	}
	else {
	    RCC->AHB1_ENR &= ~(0x1 << PORT);
	}

}

void GPIO_INIT(GPIO_Handle *pGPIO_Handle) {

    uint32_t temp = 0;

    GPIO_PERICLK(pGPIO_Handle->pGPIOX, ENABLE);


    if (pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        temp = ((uint32_t)pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinMode << (2 * pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber));

        pGPIO_Handle->pGPIOX->MODER &= ~(0x3U << (2 * pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber));
        pGPIO_Handle->pGPIOX->MODER |= temp;
    }
    else
    {

        pGPIO_Handle->pGPIOX->MODER &= ~(0x3U << (2 * pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber));

        if (pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinMode == GPIO_MODE_IT_FT) {
            EXTI->FTSR |= (1U << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
            EXTI->RTSR &= ~(1U << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
        }
        else if (pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinMode == GPIO_MODE_IT_RT) {
            EXTI->RTSR |= (1U << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
            EXTI->FTSR &= ~(1U << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
        }
        else if (pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinMode == GPIO_MODE_IT_RFT) {
            EXTI->RTSR |= (1U << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
            EXTI->FTSR |= (1U << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
        }

        uint8_t temp1 = pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber / 4;
        uint8_t temp2 = pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber % 4;
        uint8_t portcode = GPIO_PORTCODE(pGPIO_Handle->pGPIOX);
        SYSCFG_PERICLK_EN();
        SYSCFG->EXTICR[temp1] &= ~(0xFU << (temp2 * 4));
        SYSCFG->EXTICR[temp1] |= (portcode << (temp2 * 4));

        EXTI->PR = (1U << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
        EXTI->IMR |= (1U << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
    }

    temp = ((uint32_t)pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinSpeed << (2 * pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber));

    pGPIO_Handle->pGPIOX->OSPEEDR &= ~(0x3U << (2 * pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber));
    pGPIO_Handle->pGPIOX->OSPEEDR |= temp;


    temp = ((uint32_t)pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinPUPD << (2 * pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber));

    pGPIO_Handle->pGPIOX->PUPDR &= ~(0x3U << (2 * pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber));
    pGPIO_Handle->pGPIOX->PUPDR |= temp;

    temp = ((uint32_t)pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinOTYPE << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
    pGPIO_Handle->pGPIOX->OTYPER &= ~(0x1U << pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber);
    pGPIO_Handle->pGPIOX->OTYPER |= temp;

    if (pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        uint8_t temp1, temp2;
        temp1 = pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber / 8;
        temp2 = pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinNumber % 8;
        pGPIO_Handle->pGPIOX->AFR[temp1] &= ~(0xFU << (4 * temp2));
        pGPIO_Handle->pGPIOX->AFR[temp1] |= ((uint32_t)pGPIO_Handle->GPIOX_PINCONFIG.GPIO_PinALTFMode << (4 * temp2));
    }
}

void GPIO_DEINIT(GPIO_REGDEF *pGPIOX) {

	uint8_t PORT = ((uint32_t)pGPIOX - (uint32_t)GPIOA) / 0x400;
	RCC->AHB1_RSTR |=  (1 << PORT);
	RCC->AHB1_RSTR &= ~(1 << PORT);
}

uint8_t GPIO_READPIN(GPIO_REGDEF *pGPIOX, uint8_t PinNumber) {
	uint8_t VALUE = (uint8_t)(pGPIOX->IDR >> PinNumber) & 0x00000001;
	return VALUE;

}

uint16_t GPIO_READPORT(GPIO_REGDEF *pGPIOX) {
	uint16_t VALUE = (uint16_t)(pGPIOX->IDR);
	return VALUE;
}

void GPIO_WRITEPIN(GPIO_REGDEF *pGPIOX, uint8_t PinNumber, uint8_t VALUE) {
	if(VALUE == SET) {
		pGPIOX->BSRR = (1U << PinNumber);
	}
	else {
		pGPIOX->BSRR = (1U << (PinNumber + 16));
	}
}

void GPIO_WRITEPORT(GPIO_REGDEF *pGPIOX, uint16_t VALUE) {
	pGPIOX->ODR  = VALUE;
}

void GPIO_TOGGLEPIN(GPIO_REGDEF *pGPIOX, uint8_t PinNumber) {
	pGPIOX->ODR ^= (0x1U << PinNumber);
}


void GPIO_IRQCONFIG(uint8_t IRQNumber, uint32_t state) {

	if(state == ENABLE) {
		if(IRQNumber <= 31) {
			*NVIC_ISER0 |= (1U << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64) {
			*NVIC_ISER1 |= (1U << IRQNumber % 32);
		}
		else if(IRQNumber >= 64  && IRQNumber < 96) {
			*NVIC_ISER2 |= (1U << IRQNumber % 64);
		}
	}

	else {
		if(IRQNumber <= 31) {
			*NVIC_ICER0 = (1U << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64) {
			*NVIC_ICER1 = (1U << IRQNumber % 32);
		}
		else if(IRQNumber >= 64 && IRQNumber < 96) {
			*NVIC_ICER2 = (1U << IRQNumber % 64);
		}

	}
}

void GPIO_IRQPRIORITY(uint8_t IRQNumber, uint8_t Priority) {
	uint8_t IPRX = IRQNumber / 4;
	uint8_t IPRXS = IRQNumber % 4;
	*(NVIC_PR_BASEADDR + IPRX) &= ~(0xFU << (IPRXS * 8 + 4));
	*(NVIC_PR_BASEADDR + IPRX) |= ((uint32_t)(Priority & 0xF) << (IPRXS * 8 + 4));
}


void GPIO_IRQHANDLE(uint8_t PinNumber) {
	if(EXTI->PR & (1U << PinNumber)) {
		EXTI->PR = (1U << PinNumber);
	}
}











