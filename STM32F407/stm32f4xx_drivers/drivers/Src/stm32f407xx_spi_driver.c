/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Mar 25, 2024
 *      Author: diepp
 */
#include "stm32f407xx_spi_driver.h"


static void SPI_TXE_INTERRUPT_HANDLE(SPI_Handle_t *pSPIHandle);

static void SPI_RXNE_INTERRUPT_HANDLE(SPI_Handle_t *pSPIHandle);

static void SPI_OVR_ERR_INTERRUPT_HANDLE(SPI_Handle_t *pSPIHandle);


/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)	//chọn thành phần cần cấu hình bật hoặc tắt
{

	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if (pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if (pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}else if (pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}else if (pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
	}
}

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	//enable clock enable
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//first lets configure the SPI_CR1 register

	uint32_t tempreg = 0;

	//1. Configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	//2. Configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//BIDI mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		//pSPIHandle->SPIConfig.SPI_BusConfig &= ~(1 << SPI_CR1_BIDIMODE);


	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDI mode should be set
		tempreg |= (1 << SPI_CR1_BIDIMODE);
		tempreg |= (1 << 14);
		//pSPIHandle->SPIConfig.SPI_BusConfig |= (1 << SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		//RXONLU bit must be set
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	//3.  Configure the clock speed
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed  << SPI_CR1_BR;

	//4. Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF <<SPI_CR1_DFF;

	//5. Configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6. Configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;		//the CR 1 is freshly initializing the CR1 register so can use assignment operator
}


void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}else if (pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}else if (pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
}

/*
 * Data Send and Receive
 */

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

//this is the Transmitter so use the TX at RegDef, and need a pointer to the data
//third argument is size of data transfer, the uint32_t len is the standard
/*
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. wait until TXE is set
		//while (!(pSPIx->SR & (1 << 1)));	//if the expression is 0 then we have to wait
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET);				//use this function to replace code above line

		//2. Check the DFF bit in CR1
		if((pSPIx->CR1 & (1 << SPI_CR1_DFF)))
		{
			//16 bit DFF
			//1. load the data in Data Register
			pSPIx->DR = *((uint16_t*)pTxBuffer);				//use uint16 because load 2 byte
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;					//da chuyen 2 byte nen can tang 2 byte de den vi tri ke tiep

		}else
		{
			//8 bit DFF
			pSPIx->DR = *(pTxBuffer);
			Len--;
			if (Len > 0)
			pTxBuffer++;
		}
	}
}*/
void SPI_SendData(SPI_RegDef_t *pSPIx, uint16_t pTxBuffer)
{
	while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET);
	pSPIx->DR = (pTxBuffer);
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_4, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_4, GPIO_PIN_SET);

}

//this is the Receptor so use the RX at RegDef
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while (Len > 0) {
		//1. wait until RxNE is set
		//while (!(pSPIx->SR & (1 << 1)));	//if the expression is 0 then we have to wait
		while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);				//use this function to replace code above line


		//2. Check the DFF bit in CR1
		if ((pSPIx->CR1 & (1 << SPI_CR1_DFF))) {
			//16 bit DFF
			//1. load the data from DR to Rxbuffer address
			*((uint16_t*) pRxBuffer) = pSPIx->DR;//use uint16 because load 2 byte
			Len--;
			Len--;
			(uint16_t*) pRxBuffer++;//da chuyen 2 byte nen can tang 2 byte de den vi tri ke tiep

		} else {
			//8 bit DFF
			*(pRxBuffer) = pSPIx->DR;
			Len--;
			if (Len > 0)
			pRxBuffer++;
		}

	}
}



/*
 * IRQ Configuration and ISR handling (whenever interrupt occurs so they user application can call this function in order to process that interrupt)
 */

//void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
//void SPI_IRQPriorityConfig(uint8_t IRQPriorrity, uint32_t IRQNumber);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);


void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t Enordi)
{
	if(Enordi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}

}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t Enordi)
{
	if(Enordi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t Enordi)
{
	if(Enordi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX)
	{
		//1. Save the Tx buffer address and Len information in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;

		//2. Mark the SPI state as busy in transmission so that
		//	 no other code can take over same SPI peripheral until transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

	}
	return state;
}

uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX)
	{
		//1. Save the Rx buffer address and Len information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;

		//2. Mark the SPI state as busy in transmission so that
		//	 no other code can take over same SPI peripheral until transmission is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

	}
	return state;
}




void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
	uint8_t temp1, temp2;
	//first lets check for TXE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE); //temp1 = 1: set; temp1 = 0: reset
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2)
	{
		//handle TXE
		SPI_TXE_INTERRUPT_HANDLE(pHandle);

	}
	//  check for RXNE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE); //temp1 = 1: set; temp1 = 0: reset
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if (temp1 && temp2) {
		//handle TXE
		SPI_RXNE_INTERRUPT_HANDLE(pHandle);

	}

	//  check for ovr flag
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR); //temp1 = 1: set; temp1 = 0: reset
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);
	if (temp1 && temp2) {
		//handle TXE
		SPI_OVR_ERR_INTERRUPT_HANDLE(pHandle);

	}

}


//some helper function implementations

static void SPI_TXE_INTERRUPT_HANDLE(SPI_Handle_t *pSPIHandle)
{
	// Check the DFF bit in CR1
	if ((pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)))
	{
		//16 bit DFF
		//1. load the data in Data Register
		pSPIHandle->pSPIx->DR = *((uint16_t*) pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t*) pSPIHandle->pTxBuffer++;
	} else {
		//8 bit DFF
		pSPIHandle->pSPIx->DR =  *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}
	if(!(pSPIHandle->TxLen == 0))
	{
		//TxLen is zero, so close the spi transmission and inform the appllication that TX is over

		//this prevents interrupts from setting up of TXE flag

		/*pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
		pSPIHandle->pTxBuffer = NULL;
		pSPIHandle->TxLen = 0;
		pSPIHandle->RxState = SPI_READY;*/

		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}

static void SPI_RXNE_INTERRUPT_HANDLE(SPI_Handle_t *pSPIHandle)
{
	//during as per the dff
	if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		//16 bit
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;
		pSPIHandle->pRxBuffer--;
		pSPIHandle->pRxBuffer--;
	}else
	{
		//8bit
		*(pSPIHandle->pRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer--;
	}
	if(!pSPIHandle->RxLen)
	{
		//reception is complete
		//let turn off the RXNEIE interrupt

		/*pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
		pSPIHandle->pRxBuffer = NULL;
		pSPIHandle->RxLen = 0;
		pSPIHandle->RxState = SPI_READY;*/

		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}

static void SPI_OVR_ERR_INTERRUPT_HANDLE(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;
	//1. clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;

	//2. inform the application
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
}


__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
	//this is a weak implementation. the application may override this function
}


