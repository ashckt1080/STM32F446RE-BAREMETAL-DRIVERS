#include <stm32f446re_spi.h>
#include <stdint.h>

static void SPI_TXE_INTERRUPT_HANDLE(SPI_HANDLE *pSPIX_Handle);
static void SPI_RXNE_INTERRUPT_HANDLE(SPI_HANDLE *pSPIX_Handle);
static void SPI_OVR_INTERRUPT_HANDLE(SPI_HANDLE *pSPIX_Handle);

uint8_t SPI_GETFLAG(SPI_REGDEF *pSPIX, uint32_t flag) {

	if(pSPIX->SR & flag) {
		return FLAG_SET;
	}

	return FLAG_RESET;
}

void SPI_PERICLK(SPI_REGDEF *pSPIX, uint8_t state) {

	if(state == ENABLE) {
		if(pSPIX == SPI1) {
			SPI1_PERICLK_EN();
		}
		else if(pSPIX == SPI2) {
			SPI2_PERICLK_EN();
		}
		else if(pSPIX == SPI3) {
			SPI3_PERICLK_EN();
		}
		else if(pSPIX == SPI4) {
			SPI4_PERICLK_EN();
		}
	}
	else {
		if(pSPIX == SPI1) {
			SPI1_PERICLK_DI();
		}
		else if(pSPIX == SPI2) {
			SPI2_PERICLK_DI();
		}
		else if(pSPIX == SPI3) {
			SPI3_PERICLK_DI();
		}
		else if(pSPIX == SPI4) {
			SPI4_PERICLK_DI();
		}
	}
}

void SPI_DENINIT(SPI_REGDEF *pSPIX) {

	if(pSPIX == SPI2) {
		RCC->APB1_RSTR |= (1 << 14);
		RCC->APB1_RSTR &= ~(1 << 14);
	}

	else if (pSPIX == SPI3) {
		RCC->APB1_RSTR |= (1 << 15);
		RCC->APB1_RSTR &= ~(1 << 15);
	}

	else if(pSPIX == SPI1) {
		RCC->APB2_RSTR |= (1 << 12);
		RCC->APB2_RSTR &= ~(1 << 12);
	}

	else if(pSPIX == SPI4) {
		RCC->APB2_RSTR |= (1 << 13);
		RCC->APB2_RSTR &= ~(1 << 13);
	}
}

void SPI_INIT(SPI_HANDLE *pSPIX_Handle) {

	SPI_PERICLK(pSPIX_Handle->pSPIX, ENABLE);

	uint32_t TEMP = 0;

	TEMP |= pSPIX_Handle->SPIX_CONFIG.SPI_DeviceMode << 2;

	if(pSPIX_Handle->SPIX_CONFIG.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
		TEMP &= ~(1 << 15);
		//clear bidirectional mode
	}
	else if(pSPIX_Handle->SPIX_CONFIG.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
		TEMP |= (1 << 15);
		//enable bidirectional mode
	}
	else if(pSPIX_Handle->SPIX_CONFIG.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RX) {
		TEMP &= ~(1 << 15);
		TEMP |= (1 << 10);
		//clear bidirectional mode and enable RX only mode
	}

	//DFF
	TEMP |= pSPIX_Handle->SPIX_CONFIG.SPI_DFF << 11;

	//CPHA AND CPOL
	TEMP |= pSPIX_Handle->SPIX_CONFIG.SPI_CPHA << 0;
	TEMP |= pSPIX_Handle->SPIX_CONFIG.SPI_CPOL << 1;

	//SPEED
	TEMP |= pSPIX_Handle->SPIX_CONFIG.SPI_SCLKSpeed << 3;

	//SSM
	TEMP |= pSPIX_Handle->SPIX_CONFIG.SPI_SSM << 9;

	pSPIX_Handle->pSPIX->CR1 = TEMP;

	pSPIX_Handle->pTXBuffer_ = NULL;
	pSPIX_Handle->pRXBuffer_ = NULL;
	pSPIX_Handle->TXLen_ = 0;
	pSPIX_Handle->RXLen_ = 0;
	pSPIX_Handle->TXState_ = SPI_READY;
	pSPIX_Handle->RXState_ = SPI_READY;

}

void SPI_SEND(SPI_REGDEF *pSPIX, uint8_t *pTXBuffer, int length) {

	if(pSPIX->CR1 & (1 << 15)) {
		pSPIX->CR1 |= (1 << 14);
	}

	while(length > 0) {

		while(SPI_GETFLAG(pSPIX, SPI_TXE_FLAG) == FLAG_RESET);

		if(pSPIX->CR1 & (1 << 11)) {
			if(length == 1) {
				pSPIX->DR = *(pTXBuffer);
				printf("Sending character: %c\n", pTXBuffer[0]);
				length--;
				pTXBuffer++;
			}
			else {
				pSPIX->DR = *((uint16_t*)pTXBuffer);
				printf("Sending character: %c\n", pTXBuffer[0]);
				printf("Sending character: %c\n", pTXBuffer[1]);
				length--;
				length--;
				pTXBuffer += 2;
			}
		}

		else {
			pSPIX->DR = *(pTXBuffer);
			printf("Sent: %c\n", *pTXBuffer);
			length--;
			pTXBuffer++;
		}

	}
}

void SPI_RECEIVE(SPI_REGDEF *pSPIX, uint8_t *pRXBuffer, uint32_t length) {

	if(pSPIX->CR1 & (1 << 15)) {
		pSPIX->CR1 &= ~(1 << 14);
	}

	while(length > 0) {

		while(SPI_GETFLAG(pSPIX, SPI_RXNE_FLAG) == FLAG_RESET);

		if(pSPIX->CR1 & (1 << 11)) {
			if(length == 1) {
				*(pRXBuffer) = pSPIX->DR;
				printf("Received character: %c\n", *pRXBuffer);
				length--;
				pRXBuffer++;
			}
			else {
				*((uint16_t*)pRXBuffer) = pSPIX->DR;
				printf("Received word: %c\n", *((uint16_t*)pRXBuffer));
				length--;
				length--;
				pRXBuffer += 2;
			}
		}

		else {
			*(pRXBuffer) =  pSPIX->DR;
			printf("Received character: %c\n", *pRXBuffer);
			length--;
			pRXBuffer++;
		}

	}
}

//EXPLICIT CONTROL FUNCTIONS
void SPI_PERI_CONTROL(SPI_REGDEF *pSPIX, uint8_t state) {

	if(state == ENABLE) {
		pSPIX->CR1 |= (1 << 6);
	}

	else {
		pSPIX->CR1 &= ~(1 << 6);
	}
}

void SPI_SSI_CONTROL(SPI_REGDEF *pSPIX, uint8_t state) {
	if(state == ENABLE) {
		pSPIX->CR1 |= (1 << 8);
	}

	else {
		pSPIX->CR1 &= ~(1 << 8);
	}
}

void SPI_SSOE_CONTROL(SPI_REGDEF *pSPIX, uint8_t state) {
	if(state == ENABLE) {
		pSPIX->CR2 |= (1 << 2);
	}

	else {
		pSPIX->CR2 &= ~(1 << 2);
	}
}

void SPI_IRQCONFIG(uint8_t IRQNumber, uint32_t state) {

    if(state == ENABLE) {
        if(IRQNumber <= 31) {
            *NVIC_ISER0 = (1U << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64) {
            *NVIC_ISER1 = (1U << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96) {

            *NVIC_ISER2 = (1U << (IRQNumber % 32));
        }
    }
    else {
        if(IRQNumber <= 31) {
            *NVIC_ICER0 = (1U << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64) {
            *NVIC_ICER1 = (1U << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96) {

            *NVIC_ICER2 = (1U << (IRQNumber % 32));
        }
    }
}

uint8_t SPI_SEND_IT(SPI_HANDLE *pSPIX_Handle, uint8_t *pTXBuffer, uint32_t length) {

	uint8_t state = pSPIX_Handle->TXState_;

	if(!length) {
		return state;
	}

	if(pSPIX_Handle->pSPIX->CR1 & (1 << 15)) {
		pSPIX_Handle->pSPIX->CR1 |= (1 << 14);
	}

	if(state != SPI_BUSY_IN_TX) {

		pSPIX_Handle->pTXBuffer_ = pTXBuffer;
		pSPIX_Handle->TXLen_ = length;

		pSPIX_Handle->TXState_ = SPI_BUSY_IN_TX;

		pSPIX_Handle->pSPIX->CR2 |= (1 << 7 );

	}

	return state;

}

uint8_t SPI_RECEIVE_IT(SPI_HANDLE *pSPIX_Handle, uint8_t *pRXBuffer, uint32_t length) {

	uint8_t state = pSPIX_Handle->RXState_;

	if(!length) {
		return state;
	}

	if(pSPIX_Handle->pSPIX->CR1 & (1 << 15)) {
		pSPIX_Handle->pSPIX->CR1 &= ~(1 << 14);
	}

	if(state != SPI_BUSY_IN_RX) {

		pSPIX_Handle->pRXBuffer_ = pRXBuffer;
		pSPIX_Handle->RXLen_ = length;

		pSPIX_Handle->RXState_ = SPI_BUSY_IN_RX;

		pSPIX_Handle->pSPIX->CR2 |= (1 << 6 );

	}

	return state;

}

void SPI_IRQHANDLE(SPI_HANDLE *pSPIX_Handle) {

	uint8_t TEMP1, TEMP2;

	TEMP1 = pSPIX_Handle->pSPIX->SR & (1 << 6 );
	TEMP2 = pSPIX_Handle->pSPIX->CR2 & (1 << 5 );

	if(TEMP1 && TEMP2) {
		SPI_OVR_INTERRUPT_HANDLE(pSPIX_Handle);
	}

	TEMP1 = pSPIX_Handle->pSPIX->SR & (1 << 1 );
	TEMP2 = pSPIX_Handle->pSPIX->CR2 & (1 << 7 );

	if(TEMP1 && TEMP2) {
		SPI_TXE_INTERRUPT_HANDLE(pSPIX_Handle);
	}

	TEMP1 = pSPIX_Handle->pSPIX->SR & (1 << 0 );
	TEMP2 = pSPIX_Handle->pSPIX->CR2 & (1 << 6 );

	if(TEMP1 && TEMP2) {
		SPI_RXNE_INTERRUPT_HANDLE(pSPIX_Handle);
	}

}

static void SPI_TXE_INTERRUPT_HANDLE(SPI_HANDLE *pSPIX_Handle) {

	if(pSPIX_Handle->pSPIX->CR1 & (1 << 11)) {

		if(pSPIX_Handle->TXLen_ == 1) {
			pSPIX_Handle->pSPIX->DR = *(pSPIX_Handle->pTXBuffer_);
			printf("Sending character: %c\n", pSPIX_Handle->pTXBuffer_[0]);
			pSPIX_Handle->TXLen_ --;
			pSPIX_Handle->pTXBuffer_++;
		}
		else {
			pSPIX_Handle->pSPIX->DR = *((uint16_t*)pSPIX_Handle->pTXBuffer_);
			printf("Sending character: %c\n", pSPIX_Handle->pTXBuffer_[0]);
			printf("Sending character: %c\n", pSPIX_Handle->pTXBuffer_[1]);
			pSPIX_Handle->TXLen_ --;
			pSPIX_Handle->TXLen_ --;

			pSPIX_Handle->pTXBuffer_ += 2;
		}
	}

	else {

		pSPIX_Handle->pSPIX->DR = *(pSPIX_Handle->pTXBuffer_);

		pSPIX_Handle->TXLen_ --;
		pSPIX_Handle->pTXBuffer_++;
	}

	if(!pSPIX_Handle->TXLen_) {
		pSPIX_Handle->pSPIX->CR2 &= ~(1 << 7);
		pSPIX_Handle->pTXBuffer_ = NULL;
		pSPIX_Handle->TXLen_ = 0;
		pSPIX_Handle->TXState_ = SPI_READY;
		SPI_ApplicationEventCallback(pSPIX_Handle, SPI_EVENT_TX_CMPLT);
	}
}

static void SPI_RXNE_INTERRUPT_HANDLE(SPI_HANDLE *pSPIX_Handle) {


	if(pSPIX_Handle->pSPIX->CR1 & (1 << 11)) {

		if(pSPIX_Handle->RXLen_ == 1) {
			*(pSPIX_Handle->pRXBuffer_) =  pSPIX_Handle->pSPIX->DR;
			printf("Received character: %c\n", *(pSPIX_Handle->pRXBuffer_));
			pSPIX_Handle->RXLen_ --;
			pSPIX_Handle->pRXBuffer_ ++;
		}
		else {
			*((uint16_t*)pSPIX_Handle->pRXBuffer_) = (uint16_t)pSPIX_Handle->pSPIX->DR;
			printf("Received word: %c\n", *((uint16_t*)pSPIX_Handle->pRXBuffer_));
			pSPIX_Handle->RXLen_ --;
			pSPIX_Handle->RXLen_ --;
			pSPIX_Handle->pRXBuffer_ += 2;
		}
	}

	else {
		*(pSPIX_Handle->pRXBuffer_) =  pSPIX_Handle->pSPIX->DR;
		printf("Received character: %c\n", *(pSPIX_Handle->pRXBuffer_));
		pSPIX_Handle->RXLen_ --;
		pSPIX_Handle->pRXBuffer_ ++;
	}

	if(!pSPIX_Handle->RXLen_) {
		pSPIX_Handle->pSPIX->CR2 &= ~(1 << 6);
		pSPIX_Handle->pRXBuffer_ = NULL;
		pSPIX_Handle->RXLen_ = 0;
		pSPIX_Handle->RXState_ = SPI_READY;
		SPI_ApplicationEventCallback(pSPIX_Handle, SPI_EVENT_RX_CMPLT);
	}
}

static void SPI_OVR_INTERRUPT_HANDLE(SPI_HANDLE *pSPIX_Handle) {

	uint8_t TEMP;

	TEMP = pSPIX_Handle->pSPIX->DR;
	TEMP = pSPIX_Handle->pSPIX->SR;
	(void)TEMP;

	SPI_ApplicationEventCallback(pSPIX_Handle, SPI_EVENT_OVR_ERR);
}

void SPI_CLOSE_TRANSMISSION(SPI_HANDLE *pSPIX_Handle) {
	pSPIX_Handle->pSPIX->CR2 &= ~(1 << 7);
	pSPIX_Handle->pTXBuffer_ = NULL;
	pSPIX_Handle->TXLen_ = 0;
	pSPIX_Handle->TXState_ = SPI_READY;
}


void SPI_CLOSE_RECEPTION(SPI_HANDLE *pSPIX_Handle) {
	pSPIX_Handle->pSPIX->CR2 &= ~(1 << 6);
	pSPIX_Handle->pRXBuffer_ = NULL;
	pSPIX_Handle->RXLen_ = 0;
	pSPIX_Handle->RXState_ = SPI_READY;
}

void SPI_CLEAR_OVR_FLAG(SPI_REGDEF *pSPIX) {

	uint8_t TEMP;

	TEMP = pSPIX->DR;
	TEMP = pSPIX->SR;
	(void)TEMP;

}


__attribute__ ((weak)) void SPI_ApplicationEventCallback (SPI_HANDLE *pSPIX_Handle, uint8_t event ) {}





