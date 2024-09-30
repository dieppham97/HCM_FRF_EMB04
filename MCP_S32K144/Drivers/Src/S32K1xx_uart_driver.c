/*
 * S32K1xx_uart_driver.c
 *
 *  Created on: Sep 29, 2023
 *      Author: Diep Pham
 */
#include "S32K1xx_uart_driver.h"

/*
 *	UART0_RX: PTA2, PTB0, PTC2
 * UART0_TX: PTA3, PTB1, PTC3
 * ALT6: PTA2, PTA3
 * ALT2: PTB1, PTB0
 * ALT4: PTC2, PTC3
 * CLOCK SOURCE: SOSCDIV2, SIRCDIV2, FIRCDIV2, SPLLDIV2
 */

/*******************************************************************************
 * @fn				- UART_PeriClockControl
 *
 * @brief			- This function enables or disables the peripheral clock for a specific UART
 *
 * @param[in]		- pUARTHandl: pointer to the UART handle structure
 * @param[in]		- EnOrDi: ENABLE to enable the clock, DISABLE to disable it
 *
 * @return			- none
 *
 * @Note			- This function configures the Peripheral Clock Control (PCC) register for the specified UART
 */
void UART_PeriClockControl(UART_Handle_t *pUARTHandl, uint8_t EnOrDi)
{
	uint32_t temp = 0;
	// Shift the clock source to the appropriate bit position in the PCC register
	temp = ((uint32_t)pUARTHandl->USART_Config.UART_ClockSource << 24);

	if (EnOrDi == ENABLE)
	{
		// Enable the clock for the specified UART
		if (pUARTHandl->pUARTx == UART0)
		{
			PCC->PCC_LPUART0 = temp; // Set the clock source for UART0
			PCC_UART0_EN();					 // Enable the clock for UART0
		}
		else if (pUARTHandl->pUARTx == UART1)
		{
			PCC->PCC_LPUART1 = temp; // Set the clock source for UART1
			PCC_UART1_EN();					 // Enable the clock for UART1
		}
		else if (pUARTHandl->pUARTx == UART2)
		{
			PCC->PCC_LPUART2 = temp; // Set the clock source for UART2
			PCC_UART2_EN();					 // Enable the clock for UART2
		}
	}
	else
	{
		// Disable the clock for the specified UART
		if (pUARTHandl->pUARTx == UART0)
		{
			PCC_UART0_DI(); // Disable the clock for UART0
		}
		else if (pUARTHandl->pUARTx == UART1)
		{
			PCC_UART1_DI(); // Disable the clock for UART1
		}
		else if (pUARTHandl->pUARTx == UART2)
		{
			PCC_UART2_DI(); // Disable the clock for UART2
		}
	}
}

/*******************************************************************************
 * @fn				- UART_SetBaudRate
 *
 * @brief			- This function configures the baud rate for the UART peripheral
 *
 * @param[in]		- pUARTHandl: pointer to the UART handle structure
 *
 * @return			- none
 *
 * @Note			- This function calculates and sets the baud rate based on the selected clock source and oversampling rate
 */
void UART_SetBaudRate(UART_Handle_t *pUARTHandl)
{
	uint32_t calClock = 0;
	uint32_t tempClock = 0;

	// Calculate the clock value based on the selected clock source
	if (pUARTHandl->USART_Config.UART_ClockSource == PCS_SOSC)
	{
		calClock = (SOSCDIV2 / (pUARTHandl->USART_Config.UART_BAUD * (pUARTHandl->USART_Config.UART_OverSampling + 1)));
	}
	else if (pUARTHandl->USART_Config.UART_ClockSource == PCS_SIRC)
	{
		calClock = (SIRCDIV2 / (pUARTHandl->USART_Config.UART_BAUD * (pUARTHandl->USART_Config.UART_OverSampling + 1)));
	}
	else if (pUARTHandl->USART_Config.UART_ClockSource == PCS_FIRC)
	{
		calClock = (FIRCDIV2 / (pUARTHandl->USART_Config.UART_BAUD * (pUARTHandl->USART_Config.UART_OverSampling + 1)));
	}
	else if (pUARTHandl->USART_Config.UART_ClockSource == PCS_SPLL)
	{
		calClock = (SPLLDIV2 / (pUARTHandl->USART_Config.UART_BAUD * (pUARTHandl->USART_Config.UART_OverSampling + 1)));
	}

	// Configure the BAUD register
	// Set the oversampling rate (OSR) in the BAUD register
	tempClock = (uint32_t)(pUARTHandl->USART_Config.UART_OverSampling << UART_BAUD_OSR);
	// Set the Baud Rate Modulo Divisor (SBR) in the BAUD register
	tempClock |= calClock << UART_BAUD_SBR;

	// Set the calculated BAUD register value for the appropriate UART
	if (pUARTHandl->pUARTx == UART0)
	{
		UART0->BAUD = tempClock;
	}
	else if (pUARTHandl->pUARTx == UART1)
	{
		UART1->BAUD = tempClock;
	}
	else if (pUARTHandl->pUARTx == UART2)
	{
		UART2->BAUD = tempClock;
	}
}

/*******************************************************************************
 * @fn				- UART_Init
 *
 * @brief			- This function initializes the UART peripheral with the specified configuration
 *
 * @param[in]		- pUARTHandl: pointer to the UART handle structure containing configuration details
 *
 * @return			- none
 *
 * @Note			- This function configures various UART parameters such as baud rate, stop bits,
 *                    word length, parity, mode (Tx/Rx), and interrupts
 */
void UART_Init(UART_Handle_t *pUARTHandl)
{
	uint32_t tempreg = 0;

	// Enable the peripheral clock for the UART
	UART_PeriClockControl(pUARTHandl, ENABLE);

	// Set the baud rate
	UART_SetBaudRate(pUARTHandl);

	// Configure stop bits
	tempreg |= (uint32_t)(pUARTHandl->USART_Config.UART_StopBit << UART_BAUD_SBSN);
	pUARTHandl->pUARTx->BAUD |= tempreg;

	// Reset tempreg for CTRL register configuration
	tempreg = 0;

	// Configure word length
	tempreg |= (uint32_t)(pUARTHandl->USART_Config.UART_WordLength << UART_CTRL_M);

	// Configure parity
	if (pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_EN_EVEN)
	{
		tempreg |= (1 << UART_CTRL_PE); // Enable parity
	}
	else if (pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_EN_ODD)
	{
		tempreg |= ((1 << UART_CTRL_PE) | (1 << UART_CTRL_PT)); // Enable parity and set to odd
	}

	// Configure UART mode (Tx, Rx, or both)
	if (pUARTHandl->USART_Config.UART_MODE == UART_MODE_ONLY_TX)
	{
		tempreg |= (1 << UART_CTRL_TE); // Enable transmitter
	}
	else if (pUARTHandl->USART_Config.UART_MODE == UART_MODE_ONLY_RX)
	{
		tempreg |= (1 << UART_CTRL_RE); // Enable receiver
	}
	else if (pUARTHandl->USART_Config.UART_MODE == UART_MODE_TXRX)
	{
		tempreg |= (1 << UART_CTRL_RE) | (1 << UART_CTRL_TE); // Enable both transmitter and receiver
	}

	// Apply the CTRL register configuration
	pUARTHandl->pUARTx->CTRL = tempreg;

	// Configure interrupts
	if (pUARTHandl->USART_Config.UART_ITMODE == UART_ITMODE_TC)
	{
		pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_TCIE); // Enable transmission complete interrupt
		pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_TIE);	 // Enable transmit interrupt
	}
	else if (pUARTHandl->USART_Config.UART_ITMODE == UART_ITMODE_RX)
	{
		pUARTHandl->pUARTx->CTRL |= (1 << UART_CTRL_RIE); // Enable receive interrupt
	}

// Configure idle line detection
#if 1
	pUARTHandl->pUARTx->CTRL |= (1U << 20); // Enable idle line interrupt
	pUARTHandl->pUARTx->CTRL |= (4U << 8);	// Set idle line configuration (4 bit times)
	pUARTHandl->pUARTx->CTRL |= (1U << 2);	// Set idle line type select to after stop bit
#endif
}

/*******************************************************************************
 * @fn				- UART_SendData
 *
 * @brief			- This function sends data through the UART interface
 *
 * @param[in]		- pUARTHandl: pointer to the UART handle structure
 * @param[in]		- pTxBuffer: pointer to the transmit buffer containing data to be sent
 *
 * @return			- none
 *
 * @Note			- This function sends data byte by byte, handling both 8-bit and 9-bit word lengths
 */
void UART_SendData(UART_Handle_t *pUARTHandl, uint8_t *pTxBuffer)
{
	uint16_t *tempData;
	uint32_t Len = strlen((const char *)pTxBuffer);
	pTxBuffer[Len + 1] = '\0'; // Add null terminator at the end of the buffer
	for (uint32_t i = 0; i < Len; i++)
	{
		// Wait until the transmit data register is empty
		while (!((pUARTHandl->pUARTx->STAT >> UART_STAT_TDRE) & 0x1))
			;
		if (pUARTHandl->USART_Config.UART_WordLength == UART_WORDLEN_9BITS)
		{
			tempData = (uint16_t *)pTxBuffer;
			pUARTHandl->pUARTx->DATA = (*tempData & (uint16_t)0x1FF); // Send 9 bits of data
			if (pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_DISABLE)
			{
				pTxBuffer += 2; // Move pointer by 2 bytes if parity is disabled (full 9-bit data)
			}
			else
			{
				pTxBuffer++; // Move pointer by 1 byte if parity is enabled (8-bit data + 1 parity bit)
			}
		}
		else
		{
			pUARTHandl->pUARTx->DATA = (*pTxBuffer & (uint8_t)0xFF); // Send 8 bits of data
			pTxBuffer++;																						 // Move to the next byte
		}
	}
	// Wait until the transmission is complete
	while (!((pUARTHandl->pUARTx->STAT >> UART_STAT_TC) & 0x1))
		;
}

/*******************************************************************************
 * @fn				- UART_ReceiveData
 *
 * @brief			- This function receives data through the UART interface
 *
 * @param[in]		- pUARTHandl: pointer to the UART handle structure
 * @param[out]		- pRxBuffer: pointer to the receive buffer to store incoming data
 * @param[in]		- Len: number of bytes to receive
 *
 * @return			- none
 *
 * @Note			- This function receives data byte by byte, handling both 8-bit and 9-bit word lengths
 *                 - It implements a timeout mechanism to prevent infinite waiting
 */
void UART_ReceiveData(UART_Handle_t *pUARTHandl, uint8_t *pRxBuffer, uint32_t Len)
{
	uint32_t timeoutCount = 0;
	const uint32_t TIMEOUT_MAX = 150000; // Maximum timeout count to prevent infinite loop

	for (uint32_t i = 0; i < Len; i++)
	{
		timeoutCount = 0;
		// Wait until the receive data register is full or timeout occurs
		while (!((pUARTHandl->pUARTx->STAT >> UART_STAT_RDRF) & 0x1))
		{
			timeoutCount++;
			if (timeoutCount > TIMEOUT_MAX)
			{
				*pRxBuffer = '\0'; // Null-terminate the buffer if timeout occurs
				break;
			}
		}

		if (pUARTHandl->USART_Config.UART_WordLength == UART_WORDLEN_9BITS)
		{
			if (pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_DISABLE)
			{
				// For 9-bit data without parity, read all 9 bits
				*((uint16_t *)pRxBuffer) = (pUARTHandl->pUARTx->DATA & (uint16_t)0x1FF);
				pRxBuffer++;
			}
			else
			{
				// For 9-bit data with parity, read only 8 bits (parity bit is handled by hardware)
				*pRxBuffer = (pUARTHandl->pUARTx->DATA & (uint8_t)0xFF);
				pRxBuffer++;
			}
		}
		else // 8-bit word length
		{
			if (pUARTHandl->USART_Config.UART_ParityBit == UART_PARITY_DISABLE)
			{
				// For 8-bit data without parity, read all 8 bits
				*pRxBuffer = (pUARTHandl->pUARTx->DATA & (uint8_t)0xFF);
			}
			else
			{
				// For 8-bit data with parity, read only 7 bits (parity bit is handled by hardware)
				*pRxBuffer = (uint8_t)0x7F;
			}
			pRxBuffer++;
		}
	}

	// Clear the UART receive interrupt flag
	*NVIC_ICPR1 |= (1 << 1); // Note: This might need adjustment based on the specific UART interrupt number
}

/*******************************************************************************
 * @fn				- UART_IRQInterruptConfig
 *
 * @brief			- This function configures the UART interrupt in the NVIC (Nested Vectored Interrupt Controller)
 *
 * @param[in]		- IRQNumber: The interrupt request number for the UART
 * @param[in]		- EnorDi: ENABLE to enable the interrupt, DISABLE to disable it
 *
 * @return			- none
 *
 * @Note			- This function enables or disables the UART interrupt in the NVIC based on the provided IRQ number
 */
void UART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		// Enable the interrupt
		if (IRQNumber <= 31)
		{
			*NVIC_ISER0 |= (1 << IRQNumber);
		}
		else if (31 < IRQNumber && IRQNumber < 64)
		{
			*NVIC_ISER1 |= (1 << (IRQNumber % 32)); // Use modulo to get the correct bit position
		}
		else if (64 <= IRQNumber && IRQNumber < 96)
		{
			*NVIC_ISER2 |= (1 << (IRQNumber % 64)); // Use modulo to get the correct bit position
		}
		else if (96 <= IRQNumber && IRQNumber < 128)
		{
			*NVIC_ISER3 |= (1 << (IRQNumber % 96)); // Use modulo to get the correct bit position
		}
		else if (128 <= IRQNumber && IRQNumber < 160)
		{
			*NVIC_ISER4 |= (1 << (IRQNumber % 128)); // Use modulo to get the correct bit position
		}
		else if (160 <= IRQNumber && IRQNumber < 192)
		{
			*NVIC_ISER5 |= (1 << (IRQNumber % 160)); // Use modulo to get the correct bit position
		}
	}
	else
	{
		// Disable the interrupt
		if (IRQNumber <= 31)
		{
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if (31 < IRQNumber && IRQNumber < 64)
		{
			*NVIC_ICER1 |= (1 << (IRQNumber % 32)); // Use modulo to get the correct bit position
		}
		else if (64 <= IRQNumber && IRQNumber < 96)
		{
			*NVIC_ICER2 |= (1 << (IRQNumber % 64)); // Use modulo to get the correct bit position
		}
		else if (96 <= IRQNumber && IRQNumber < 128)
		{
			*NVIC_ICER3 |= (1 << (IRQNumber % 96)); // Use modulo to get the correct bit position
		}
		else if (128 <= IRQNumber && IRQNumber < 160)
		{
			*NVIC_ICER4 |= (1 << (IRQNumber % 128)); // Use modulo to get the correct bit position
		}
		else if (160 <= IRQNumber && IRQNumber < 192)
		{
			*NVIC_ICER5 |= (1 << (IRQNumber % 160)); // Use modulo to get the correct bit position
		}
	}
}
