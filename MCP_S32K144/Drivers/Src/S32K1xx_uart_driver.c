#include "S32K1xx_uart_driver.h"



/*
*	UART0_RX: PTA2, PTB0, PTC2
* UART0_TX: PTA3, PTB1, PTC3
* ALT6: PTA2, PTA3
* ALT2: PTB1, PTB0
* ALT4: PTC2, PTC3
* CLOCK SOURCE: SOSCDIV2, SIRCDIV2, FIRCDIV2, SPLLDIV2
*/

void UART_PeriClockControl(UART_Handle_t *pUARTHandl, uint8_t EnOrDi)
{
	uint32_t temp = 0;
	temp = ((uint32_t)pUARTHandl->USART_Config.UART_ClockSource << 24);
	if (EnOrDi == ENABLE)
	{
		if (pUARTHandl->pUARTx == UART0)
		{
			PCC->PCC_LPUART0 = temp;
			PCC_UART0_EN();
		} else if(pUARTHandl->pUARTx == UART1)
		{
			PCC->PCC_LPUART1 = temp;
			PCC_UART1_EN();
		} else if (pUARTHandl->pUARTx == UART2)
		{
			PCC->PCC_LPUART2 = temp;
			PCC_UART2_EN();
		}
	}else
	{
		if (pUARTHandl->pUARTx == UART0)
		{
			PCC_UART0_DI();
		} else if(pUARTHandl->pUARTx == UART1)
		{
			PCC_UART1_DI();
		} else if (pUARTHandl->pUARTx == UART2)
		{
			PCC_UART2_DI();
		}
	}
	
}

void UART_SetBaudRate(UART_Handle_t *pUARTHandl)
{
	uint32_t calClock = 0;
	if (pUARTHandl->USART_Config.UART_ClockSource == Source_SOSC)
	{
		calClock = (SOSCDIV2 /(pUARTHandl->USART_Config.UART_BAUD * (pUARTHandl->USART_Config.UART_OverSampling + 1)));
	}else if (pUARTHandl->USART_Config.UART_ClockSource == Source_SIRC)
	{
		calClock = (SIRCDIV2 /(pUARTHandl->USART_Config.UART_BAUD * (pUARTHandl->USART_Config.UART_OverSampling + 1)));
	}else if (pUARTHandl->USART_Config.UART_ClockSource == Source_FIRC)
	{
		calClock = (FIRCDIV2 /(pUARTHandl->USART_Config.UART_BAUD * (pUARTHandl->USART_Config.UART_OverSampling + 1)));
	}else if (pUARTHandl->USART_Config.UART_ClockSource == Source_SPLL)
	{
		calClock = (SPLLDIV2 /(pUARTHandl->USART_Config.UART_BAUD * (pUARTHandl->USART_Config.UART_OverSampling + 1)));
	}
	
	uint32_t tempClock;
	tempClock = (uint32_t)(pUARTHandl->USART_Config.UART_OverSampling << UART_BAUD_OSR);
	tempClock |= calClock << UART_BAUD_SBR;
	/* tempClock = Source/ baudrate*osr */
	if (pUARTHandl->pUARTx == UART0)
	{
		UART0->BAUD = tempClock;
		
	}else if(pUARTHandl->pUARTx == UART1)
	{
		UART1->BAUD = tempClock;
	}else if(pUARTHandl->pUARTx == UART1)
	{
		UART2->BAUD = tempClock;
	}
}

void UART_Init(UART_Handle_t *pUARTHandl)
{
	uint32_t tempreg = 0;
	
	UART_PeriClockControl(pUARTHandl, ENABLE);
	
	/* Set Baud rate*/
	UART_SetBaudRate (pUARTHandl);
	
	/* Select stop bit */
	tempreg |= (uint32_t)(pUARTHandl->USART_Config.UART_StopBit << UART_BAUD_SBSN);
	pUARTHandl->pUARTx->BAUD |= tempreg;
	
	/* Select data number */
	tempreg = 0;
	tempreg |= (uint32_t)(pUARTHandl->USART_Config.UART_WordLength << UART_CTRL_M);
	/*pUARTHandl->pUARTx->CTRL = tempreg;*/
	
	/* UART_CTRL Register */
	/* Set Parity Bit */
	if (pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_EN_EVEN)
	{
		/*pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_PE); */
		tempreg |=(1 << UART_CTRL_PE);
	} else if (pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_EN_ODD)
	{
		/* pUARTHandl->pUARTx->CTRL = ((1 << UART_CTRL_PE) | (1 << UART_CTRL_PT));*/
		tempreg |= ((1 << UART_CTRL_PE) | (1 << UART_CTRL_PT));
	}
	
	/* Set Rx or Tx */
	if (pUARTHandl->USART_Config.UART_MODE == UART_MODE_ONLY_TX)
	{
		/* pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_TE); */
		tempreg |= (1 << UART_CTRL_TE);
	} else if (pUARTHandl->USART_Config.UART_MODE == UART_MODE_ONLY_RX)
	{
		/* pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_RE); */
		tempreg |= (1 << UART_CTRL_RE);
	}else if (pUARTHandl->USART_Config.UART_MODE == UART_MODE_TXRX)
	{
/*	pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_RE);
		pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_TE);*/
		tempreg |= (1 << UART_CTRL_RE);
		tempreg |= (1 << UART_CTRL_TE);
	}
	pUARTHandl->pUARTx->CTRL= tempreg;
	
	/* Set Interrupt Rx or Tx */
	if(pUARTHandl->USART_Config.UART_ITMODE == UART_ITMODE_TC)
	{
		pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_TCIE);
		pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_TIE);
	}else if(pUARTHandl->USART_Config.UART_ITMODE == UART_ITMODE_RX)
	{
		pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_RIE);
	}
}



void UART_SendData(UART_Handle_t *pUARTHandl, uint8_t *pTxBuffer, uint32_t Len)
{
	uint16_t *tempData;
	uint16_t check;
	for (uint32_t i = 0; i < Len; i++)
	{
		while (!((pUARTHandl->pUARTx->STAT >> UART_STAT_TDRE) & 0x1));
		if(pUARTHandl->USART_Config.UART_WordLength == UART_WORDLEN_9BITS)
		{
			tempData = (uint16_t*)pTxBuffer;
			pUARTHandl->pUARTx->DATA = (*tempData & (uint16_t)0x1FF);
			if(pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_DISABLE)
			{
				pTxBuffer++;
				pTxBuffer++;
			}else
			{
				pTxBuffer++;
			}
		}else
		{
			pUARTHandl->pUARTx->DATA = (*pTxBuffer & (uint8_t)0xFF);
			check = (*pTxBuffer & (uint8_t)0xFF);
			pTxBuffer++;
		}
	}
	while(!((pUARTHandl->pUARTx->STAT >> UART_STAT_TC) & 0x1));
}

void UART_ReceiveData(UART_Handle_t *pUARTHandl, uint8_t *pRxBuffer, uint32_t Len)
{
	for (uint32_t i = 0; i < Len; i++)
	{
		while (!((pUARTHandl->pUARTx->STAT >> UART_STAT_RDRF) & 0x1));
		if(pUARTHandl->USART_Config.UART_WordLength == UART_WORDLEN_9BITS)
		{
			if(pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_DISABLE)
			{
				*((uint16_t*) pRxBuffer) = (pUARTHandl->pUARTx->DATA & (uint16_t)0x1FF);
				pRxBuffer++;
			}else
			{
				*pRxBuffer = (pUARTHandl->pUARTx->DATA & (uint8_t)0xFF);
				pRxBuffer++;
			}
		}else
		{
			if(pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_DISABLE)
			{
				*pRxBuffer = (pUARTHandl->pUARTx->DATA & (uint8_t)0xFF);
			}else
			{
				*pRxBuffer = (uint8_t) 0x7F;
			}
			pRxBuffer++;
		}
	}
}

void UART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (IRQNumber <= 31)
		{
			*NVIC_ISER0 |= (1 << IRQNumber);
		} else if (31 < IRQNumber && IRQNumber < 64)
		{
			*NVIC_ISER1 |= (1 << IRQNumber % 32);
		}else if (64 <= IRQNumber && IRQNumber < 96)
		{
			*NVIC_ISER2 |= (1 << IRQNumber % 64);
		}else if (96 <= IRQNumber && IRQNumber < 128)
		{
			*NVIC_ISER3 |= (1 << IRQNumber % 96);
		}else if (128 <= IRQNumber && IRQNumber < 160)
		{
			*NVIC_ISER4 |= (1 << IRQNumber % 128);
		}else if (160 <= IRQNumber && IRQNumber < 192)
		{
			*NVIC_ISER5 |= (1 << IRQNumber % 160);
		}
	}else
	{
		if (IRQNumber <= 31)
		{
			*NVIC_ICER0 |= (1 << IRQNumber);
		} else if (31 < IRQNumber && IRQNumber < 64)
		{
			*NVIC_ICER1 |= (1 << IRQNumber % 32);
		}else if (64 <= IRQNumber && IRQNumber < 96)
		{
			*NVIC_ICER2 |= (1 << IRQNumber % 64);
		}else if (96 <= IRQNumber && IRQNumber < 128)
		{
			*NVIC_ICER3 |= (1 << IRQNumber % 96);
		}else if (128 <= IRQNumber && IRQNumber < 160)
		{
			*NVIC_ICER4 |= (1 << IRQNumber % 128);
		}else if (160 <= IRQNumber && IRQNumber < 192)
		{
			*NVIC_ICER5 |= (1 << IRQNumber % 160);
		}
	}
}

void UART_IRQHandling(UART_Handle_t *pUSARTHandle)
{
	uint32_t temp1, temp2;
	
	/************************* TC and TCIE flag *************************/
	temp1 = pUSARTHandle->pUARTx->CTRL & (1 << UART_CTRL_TCIE);
	temp2 = pUSARTHandle->pUARTx->STAT & (1 << UART_STAT_TC);
	if(temp1 && temp2)
	{
		USART_ApplicationEventCallback(pUSARTHandle, UART_ITMODE_TC);
	}
	
	/************************* TDRE and TIE flag *************************/
	temp1 = pUSARTHandle->pUARTx->CTRL & (1 << UART_CTRL_TIE);
	temp2 = pUSARTHandle->pUARTx->STAT & (1 << UART_STAT_TDRE);
	if(temp1 && temp2)
	{
		USART_ApplicationEventCallback(pUSARTHandle, UART_ITMODE_TC);
	}
	
	/************************* RDRF and RIE flag *************************/
	temp1 = pUSARTHandle->pUARTx->CTRL & (1 << UART_CTRL_RIE);
	temp2 = pUSARTHandle->pUARTx->STAT ^ (1 << UART_STAT_RDRF);
	if(temp1 && temp2)
	{
		USART_ApplicationEventCallback(pUSARTHandle, UART_ITMODE_RX);
	}
}

__weak void USART_ApplicationEventCallback(UART_Handle_t *pUSARTHandle, uint8_t event)
{

}

