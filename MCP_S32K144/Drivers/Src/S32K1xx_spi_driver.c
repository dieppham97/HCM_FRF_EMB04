/*
 * S32K1xx_spi_driver.c
 *
 *  Created on: Sep 29, 2023
 *      Author: Diep Pham
 */
#include "S32K1xx_spi_driver.h"

extern uint32_t SOSCDIV2;
extern uint32_t SIRCDIV2;
extern uint32_t FIRCDIV2;
extern uint32_t SPLLDIV2;

static uint32_t prescale[8] = {1, 2, 4, 8, 16, 32, 44, 128};

/*******************************************************************************
 * @fn				- SPI_PeriClockControl
 *
 * @brief			- This function enables or disables the peripheral clock for a specific SPI module
 *
 * @param[in]		- pSPIHandle: pointer to SPI handle structure
 * @param[in]		- EnOrDi: ENABLE to enable the clock, DISABLE to disable the clock
 *
 * @return			- none
 *
 * @Note			- This function configures the Peripheral Clock Control (PCC) registers for the specified SPI module
 */
void SPI_PeriClockControl(SPI_Handle_t *pSPIHandle, uint8_t EnOrDi)
{
	uint32_t temp = 0;
	// Configure the clock source for the SPI module
	temp = ((uint32_t)pSPIHandle->SPIConfig.SPI_ClockSource << 24);

	if (EnOrDi == ENABLE)
	{
		// Enable the clock for the specified SPI module
		if (pSPIHandle->pSPIx == SPI0)
		{
			PCC->PCC_LPSPI0 = temp; // Set the clock source for SPI0
			PCC_SPI0_EN();					// Enable the clock for SPI0
		}
		else if (pSPIHandle->pSPIx == SPI1)
		{
			PCC->PCC_LPSPI1 = temp; // Set the clock source for SPI1
			PCC_SPI1_EN();					// Enable the clock for SPI1
		}
		else if (pSPIHandle->pSPIx == SPI2)
		{
			PCC->PCC_LPSPI2 = temp; // Set the clock source for SPI2
			PCC_SPI2_EN();					// Enable the clock for SPI2
		}
	}
	else
	{
		// Disable the clock for the specified SPI module
		if (pSPIHandle->pSPIx == SPI0)
		{
			PCC_SPI0_DI(); // Disable the clock for SPI0
		}
		else if (pSPIHandle->pSPIx == SPI1)
		{
			PCC_SPI1_DI(); // Disable the clock for SPI1
		}
		else if (pSPIHandle->pSPIx == SPI2)
		{
			PCC_SPI2_DI(); // Disable the clock for SPI2
		}
	}
}

/*******************************************************************************
 * @fn				- SetSCK
 *
 * @brief			- This function calculates the SCK (Serial Clock) divider value for the SPI module
 *
 * @param[in]		- pSPIHandle: pointer to SPI handle structure
 *
 * @return			- uint32_t: The calculated SCK divider value
 *
 * @Note			- This function calculates the SCK divider based on the selected clock source and prescaler
 */
uint32_t SetSCK(SPI_Handle_t *pSPIHandle)
{
	uint32_t tempFSPI = 0;

	// Calculate the input frequency based on the selected clock source
	switch (pSPIHandle->SPIConfig.SPI_ClockSource)
	{
	case PCS_SOSC:
		tempFSPI = (SOSCDIV2 / prescale[pSPIHandle->SPIConfig.SPI_Prescale]);
		break;
	case PCS_SIRC:
		tempFSPI = (SIRCDIV2 / prescale[pSPIHandle->SPIConfig.SPI_Prescale]);
		break;
	case PCS_FIRC:
		tempFSPI = (FIRCDIV2 / prescale[pSPIHandle->SPIConfig.SPI_Prescale]);
		break;
	case PCS_SPLL:
		tempFSPI = (SPLLDIV2 / prescale[pSPIHandle->SPIConfig.SPI_Prescale]);
		break;
	default:
		// Handle error or set a default value
		break;
	}

	// Calculate and return the SCK divider value
	// Note: Multiply SPI_SCK by 1000000 to convert from MHz to Hz
	// Subtract 2 from the result as per the hardware requirement
	return ((tempFSPI / (pSPIHandle->SPIConfig.SPI_SCK * 1000000)) - 2);
}

/*******************************************************************************
 * @fn				- SPI_PeripheralControl
 *
 * @brief			- This function enables or disables the SPI peripheral
 *
 * @param[in]		- pSPIx: pointer to SPI peripheral base address
 * @param[in]		- Enordi: ENABLE to enable the peripheral, DISABLE to disable it
 *
 * @return			- none
 *
 * @Note			- This function modifies the Control Register (CR) to enable or disable the SPI module
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t Enordi)
{
	if (Enordi == ENABLE)
	{
		// Set the MEN (Module Enable) bit in the Control Register
		pSPIx->CR |= (1 << SPI_CR_MEN);
	}
	else
	{
		// Clear the MEN (Module Enable) bit in the Control Register
		// Note: We use 1U (unsigned 1) to avoid potential warnings about integer overflow
		pSPIx->CR &= ~(1U << SPI_CR_MEN);
	}
}

/*******************************************************************************
 * @fn				- SPI_Init
 *
 * @brief			- This function initializes the SPI peripheral with the given configuration
 *
 * @param[in]		- pSPIHandle: pointer to SPI handle structure containing SPI configuration
 *
 * @return			- none
 *
 * @Note			- This function configures various SPI registers (TCR, CCR, FCR, CFGR1, CR)
 *                    based on the provided configuration in the SPI handle
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	uint32_t temp = 0;

	// Enable the peripheral clock for this SPI
	SPI_PeriClockControl(pSPIHandle, ENABLE);

	/* Configure Transmit Command Register (TCR) */
	temp |= (uint32_t)(pSPIHandle->SPIConfig.SPI_CPOL << SPI_TCR_CPOL);		// Set clock polarity
	temp |= (uint32_t)(pSPIHandle->SPIConfig.SPI_CPHA << SPI_TCR_CPHA);		// Set clock phase
	temp |= (uint32_t)((pSPIHandle->SPIConfig.SPI_DFF) << SPI_TCR_FRAME); // Set data frame format
	// Note: LSBF (Least Significant Bit First) configuration is commented out
	// temp |= (uint32_t)(pSPIHandle->SPIConfig.SPI_LSBF	<<	SPI_TCR_LSBF);
	temp |= (uint32_t)(pSPIHandle->SPIConfig.SPI_PCS << SPI_TCR_PCS);						// Set peripheral chip select
	temp |= (uint32_t)(pSPIHandle->SPIConfig.SPI_Prescale << SPI_TCR_PRESCALE); // Set prescaler value

	// Apply TCR configuration to the appropriate SPI instance
	if (pSPIHandle->pSPIx == SPI0)
	{
		SPI0->TCR = temp;
	}
	else if (pSPIHandle->pSPIx == SPI1)
	{
		SPI1->TCR = temp;
	}
	else if (pSPIHandle->pSPIx == SPI2)
	{
		SPI2->TCR = temp;
	}
	temp = 0; // Reset temp variable for next use

	/* Configure Clock Configuration Register (CCR) */
	// Set SCK (Serial Clock) divider value
	if (pSPIHandle->pSPIx == SPI0)
	{
		SPI0->CCR = (SetSCK(pSPIHandle) << SPI_CCR_SCKDIV);
	}
	else if (pSPIHandle->pSPIx == SPI1)
	{
		SPI1->CCR = (SetSCK(pSPIHandle) << SPI_CCR_SCKDIV);
	}
	else if (pSPIHandle->pSPIx == SPI2)
	{
		SPI2->CCR = (SetSCK(pSPIHandle) << SPI_CCR_SCKDIV);
	}

	/* Configure FIFO Control Register (FCR) */
	temp |= (uint32_t)(pSPIHandle->SPIConfig.SPI_LengRx << SPI_FCR_RXWATER); // Set RX FIFO watermark
	temp |= (uint32_t)(pSPIHandle->SPIConfig.SPI_LengTx << SPI_FCR_TXWATER); // Set TX FIFO watermark

	// Apply FCR configuration to the appropriate SPI instance
	if (pSPIHandle->pSPIx == SPI0)
	{
		SPI0->FCR = temp;
	}
	else if (pSPIHandle->pSPIx == SPI1)
	{
		SPI1->FCR = temp;
	}
	else if (pSPIHandle->pSPIx == SPI2)
	{
		SPI2->FCR = temp;
	}
	temp = 0; // Reset temp variable for next use

	/* Configure Module Configuration Register (CFGR1) */
	if (pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_MASTER)
	{
		temp |= (uint32_t)(pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CFGR1_MASTER); // Set as Master
	}
	else if (pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_SLAVE)
	{
		temp |= (uint32_t)(pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CFGR1_MASTER); // Set as Slave
	}
	temp |= (1 << 3); // Note: Purpose of this bit setting is unclear and should be documented

	// Apply CFGR1 configuration to the appropriate SPI instance
	if (pSPIHandle->pSPIx == SPI0)
	{
		SPI0->CFGR1 = temp;
	}
	else if (pSPIHandle->pSPIx == SPI1)
	{
		SPI1->CFGR1 = temp;
	}
	else if (pSPIHandle->pSPIx == SPI2)
	{
		SPI2->CFGR1 = temp;
	}
	temp = 0; // Reset temp variable for next use

	/* Configure Control Register (CR) */
	temp |= (1 << SPI_CR_DBGEN); // Enable debug mode

	/* Enable peripheral */
	if (pSPIHandle->pSPIx == SPI0)
	{
		SPI0->CR = temp;
	}
	else if (pSPIHandle->pSPIx == SPI1)
	{
		SPI1->CR = temp;
	}
	else if (pSPIHandle->pSPIx == SPI2)
	{
		SPI2->CR = temp;
	}
}

/*******************************************************************************
 * @fn				- SPI_SendData
 *
 * @brief			- This function sends data through the SPI peripheral
 *
 * @param[in]		- pSPIx: pointer to the SPI peripheral structure
 * @param[in]		- pTxBuffer: 16-bit data to be transmitted
 *
 * @return			- none
 *
 * @Note			- This function waits for the Transmit Data Flag (TDF) to be set before sending data
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint16_t pTxBuffer)
{
	// Wait until the Transmit Data Flag (TDF) is set
	// TDF indicates that the Transmit Data Register (TDR) is empty and ready for new data
	while (!((pSPIx->SR >> SPI_SR_TDF) & 0x1))
		;

	// Write the data to be transmitted into the Transmit Data Register (TDR)
	pSPIx->TDR = (pTxBuffer);
}

/*******************************************************************************
 * @fn				- SPI_ReceiveData
 *
 * @brief			- This function receives data through the SPI peripheral
 *
 * @param[in]		- pSPIx: pointer to the SPI peripheral structure
 * @param[in]		- pRxBuffer: pointer to buffer where received data will be stored
 *
 * @return			- none
 *
 * @Note			- This function receives data until the End of Sequence (EOS) flag is set
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer)
{
	uint32_t EOS = 1;
	while (EOS)
	{
		// Wait until the Receive Data Flag (RDF) is set
		// RDF indicates that the Receive Data Register (RDR) contains valid received data
		while (!((pSPIx->SR >> SPI_SR_RDF) & 0x1))
			;

		// Read the received data from RDR and store it in the buffer
		// Note: We're only reading the lower 8 bits (1 byte) of the 16-bit RDR
		*(pRxBuffer) = (pSPIx->RDR & (uint8_t)0xFF);

		// Move to the next position in the buffer
		pRxBuffer++;

		// Check if End of Sequence (EOS) flag is set (bit 10 of SR)
		// If set, exit the loop
		if (pSPIx->SR >> 10)
			EOS = 0;
	}

	// Null-terminate the received data string
	*(pRxBuffer) = 0;
}
