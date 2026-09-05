#ifndef INC_STM32F446RE_SPI_H_
#define INC_STM32F446RE_SPI_H_

#include <stm32f446re.h>
#include <stdint.h>

typedef struct {

	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SCLKSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;

} SPI_CONFIG;

typedef struct {

	SPI_REGDEF *pSPIX;
	SPI_CONFIG SPIX_CONFIG;
	uint8_t *pTXBuffer_;
	uint8_t *pRXBuffer_;
	uint32_t TXLen_;
	uint32_t RXLen_;
	uint8_t TXState_;
	uint8_t RXState_;

} SPI_HANDLE;


#define SPI_READY 0
#define SPI_BUSY_IN_RX 1
#define SPI_BUSY_IN_TX 2

#define SPI_EVENT_TX_CMPLT 1
#define SPI_EVENT_RX_CMPLT 2
#define SPI_EVENT_OVR_ERR 3


#define SPI_DEVICEMODE_MASTER 1
#define SPI_DEVICEMODE_SLAVE 0

#define SPI_BUS_CONFIG_FD 1
#define SPI_BUS_CONFIG_HD 2
#define SPI_BUS_CONFIG_SIMPLEX_RX 3

#define SPI_SCLKSPEED_DIV2     0
#define SPI_SCLKSPEED_DIV4     1
#define SPI_SCLKSPEED_DIV8     2
#define SPI_SCLKSPEED_DIV16    3
#define SPI_SCLKSPEED_DIV32    4
#define SPI_SCLKSPEED_DIV64    5
#define SPI_SCLKSPEED_DIV128   6
#define SPI_SCLKSPEED_DIV256   7

#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW 0

#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW 0

#define SPI_SSM_EN 1
#define SPI_SSM_DI 0

#define SPI_DFF_8BIT 0
#define SPI_DFF_16BIT 1

#define SPI_TXE_FLAG (1 << 1 )
#define SPI_RXNE_FLAG (1 << 0 )
#define SPI_BUSY_FLAG (1 << 7 )


void SPI_PERICLK(SPI_REGDEF *pSPIX, uint8_t state);
void SPI_INIT(SPI_HANDLE *pSPIX_Handle);
void SPI_DENINIT(SPI_REGDEF *pSPIX);
void SPI_PERI_CONTROL(SPI_REGDEF *pSPIX, uint8_t state);
void SPI_SSI_CONTROL(SPI_REGDEF *pSPIX, uint8_t state);
void SPI_SSOE_CONTROL(SPI_REGDEF *pSPIX, uint8_t state);

void SPI_CLEAR_OVR_FLAG(SPI_REGDEF *pSPIX);
void SPI_CLOSE_TRANSMISSION(SPI_HANDLE *pSPIX_Handle);
void SPI_CLOSE_RECEPTION(SPI_HANDLE *pSPIX_Handle);

uint8_t SPI_GETFLAG(SPI_REGDEF *pSPIX, uint32_t flag);

void SPI_SEND(SPI_REGDEF *pSPIX, uint8_t *pTXBuffer, int length);
void SPI_RECEIVE(SPI_REGDEF *pSPIX, uint8_t *pRXBuffer, uint32_t length);

uint8_t SPI_SEND_IT(SPI_HANDLE *pSPIX_Handle, uint8_t *pTXBuffer, uint32_t length);
uint8_t SPI_RECEIVE_IT(SPI_HANDLE *pSPIX_Handle, uint8_t *pRXBuffer, uint32_t length);

void SPI_IRQHANDLE(SPI_HANDLE *pSPIX_Handle);
void SPI_IRQCONFIG(uint8_t IRQNumber, uint32_t state);

void SPI_ApplicationEventCallback(SPI_HANDLE *pSPIX_Handle, uint8_t event );

#endif
