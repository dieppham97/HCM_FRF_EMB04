/*
 * S32K1xx_lpit_driver.c
 *
 *  Created on: Sep 29, 2023
 *      Author: Diep Pham
 */
#include "S32K1xx_lpit_driver.h"

/*******************************************************************************
 * @fn				- LPIT_PeriClockControl
 *
 * @brief			- This function controls the peripheral clock for the Low Power Interrupt Timer (LPIT)
 *
 * @param[in]		- selectClock: Clock source selection (1-7, corresponding to different clock sources)
 * @param[in]		- EnOrDi: ENABLE to enable the clock, DISABLE to disable
 *
 * @return			- none
 *
 * @Note			- This function enables or disables the LPIT clock based on the selected clock source
 *                 - Clock sources:
 *                   1: SOSCDIV2 (System OSC Divider 2)
 *                   2: SIRCDIV2 (Slow IRC Divider 2)
 *                   3: FIRCDIV2 (Fast IRC Divider 2)
 *                   6: SPLLDIV2 (System PLL Divider 2)
 *                   7: LPO128 (Low Power Oscillator 128kHz)
 *                 - The function uses PCC (Peripheral Clock Control) functions to configure the clock
 */
void LPIT_PeriClockControl(uint8_t selectClock, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		switch (selectClock)
		{
		case 1:
			PCC_LPIT_Clock1(); // Configure SOSCDIV2 as clock source
			PCC_LPIT_EN();		 // Enable LPIT clock
			break;
		case 2:
			PCC_LPIT_Clock2(); // Configure SIRCDIV2 as clock source
			PCC_LPIT_EN();		 // Enable LPIT clock
			break;
		case 3:
			PCC_LPIT_Clock3(); // Configure FIRCDIV2 as clock source
			PCC_LPIT_EN();		 // Enable LPIT clock
			break;
		case 6:
			PCC_LPIT_Clock6(); // Configure SPLLDIV2 as clock source
			PCC_LPIT_EN();		 // Enable LPIT clock
			break;
		case 7:
			PCC_LPIT_Clock7(); // Configure LPO128 as clock source
			PCC_LPIT_EN();		 // Enable LPIT clock
			break;
		default:
			// Invalid clock source selected
			break;
		}
	}
	else
	{
		PCC_LPIT_DI(); // Disable LPIT clock
	}
}

/*******************************************************************************
 * @fn				- ChannelClock
 *
 * @brief			- This function configures and enables a specific LPIT channel
 *
 * @param[in]		- LPIT_PinConfig: structure containing LPIT configuration parameters
 *
 * @return			- none
 *
 * @Note			- This function configures the Timer Control Register (TCTRLn) for the specified LPIT channel
 *                 - It sets the timer mode and enables the channel
 *                 - TCR_MODE: Timer Mode bits in TCTRLn register
 *                 - TCR_EN: Timer Enable bit in TCTRLn register
 *                 - The function uses a switch statement to handle different channels (0-3)
 *                 - For each channel, it configures the mode and enables the timer
 *                 - LPIT->TCTRLn registers are used to control each channel independently
 */
void ChannelClock(LPIT_PinConfig_t LPIT_PinConfig)
{
	switch (LPIT_PinConfig.Channel)
	{
	case 0:
	{
		/* Config channel mode */
		LPIT->TCTRL0 |= ((uint32_t)LPIT_PinConfig.Mode << TCR_MODE);
		/* Enable timer channel */
		LPIT->TCTRL0 |= (uint32_t)(1 << TCR_EN);
		break;
	}
	case 1:
	{
		LPIT->TCTRL1 |= ((uint32_t)LPIT_PinConfig.Mode << TCR_MODE);
		LPIT->TCTRL1 |= (1 << TCR_EN);
		break;
	}
	case 2:
	{
		LPIT->TCTRL2 |= ((uint32_t)LPIT_PinConfig.Mode << TCR_MODE);
		LPIT->TCTRL2 |= (1 << TCR_EN);
		break;
	}
	case 3:
	{
		LPIT->TCTRL3 |= ((uint32_t)LPIT_PinConfig.Mode << TCR_MODE);
		LPIT->TCTRL3 |= (1 << TCR_EN);
		break;
	}
	}
}

/*******************************************************************************
 * @fn				- LPIT_Init
 *
 * @brief			- This function initializes and configures the Low Power Interrupt Timer (LPIT)
 *
 * @param[in]		- LPIT_PinConfig: structure containing LPIT configuration parameters
 *
 * @return			- none
 *
 * @Note			- This function performs the following operations:
 *                  1. Enables the LPIT clock module
 *                  2. Configures debug mode
 *                  3. Enables interrupts if specified
 *                  4. Sets the timer value for the specified channel
 *                  5. Configures and enables the specified channel
 *
 *                  - LPIT->MCR: Module Control Register
 *                    - LPIT_MCR: Module Clock Enable bit
 *                    - LPIT_DBG_EN: Debug Enable bit
 *
 *                  - LPIT->MIER: Module Interrupt Enable Register
 *
 *                  - LPIT->TVALn: Timer Value Registers (n = 0-3)
 *                    - The timer value is calculated using the formula:
 *                      Timer Value = (Desired Time * (LPO128_CLK / 1000)) - 1
 *                      Where LPO128_CLK is the clock frequency in Hz
 *
 *                  - ChannelClock(): Function to configure and enable the specified channel
 */
void LPIT_Init(LPIT_PinConfig_t LPIT_PinConfig)
{
	/* Enable clock Module */
	LPIT->MCR |= (1 << LPIT_MCR);

	/* Enable debug mode */
	LPIT->MCR |= (1 << LPIT_DBG_EN);

	/* Set timer value for the specified channel */
	switch (LPIT_PinConfig.Channel)
	{
	case 0:
	{
		LPIT->TVAL0 = ((LPIT_PinConfig.ValueTime * (LPO128_CLK / 1000)) - 1);
		break;
	}
	case 1:
	{
		LPIT->TVAL1 = ((LPIT_PinConfig.ValueTime * (LPO128_CLK / 1000)) - 1);
		break;
	}
	case 2:
	{
		LPIT->TVAL2 = ((LPIT_PinConfig.ValueTime * (LPO128_CLK / 1000)) - 1);
		break;
	}
	case 3:
	{
		LPIT->TVAL3 = ((LPIT_PinConfig.ValueTime * (LPO128_CLK / 1000)) - 1);
		break;
	}
	}
	if (LPIT_PinConfig.Interrupt == ENABLE)
	{
		LPIT->MIER |= (1 << LPIT_PinConfig.Channel);
	}

	/* Configure and enable the specified channel */
	ChannelClock(LPIT_PinConfig);
}

/*******************************************************************************
 * @fn				- LPIT_IRQInterruptConfig
 *
 * @brief			- This function configures the NVIC (Nested Vectored Interrupt Controller) for LPIT interrupts
 *
 * @param[in]		- IRQNumber: The interrupt request number to be configured
 * @param[in]		- EnOrDi: ENABLE to enable the interrupt, DISABLE to disable it
 *
 * @return			- none
 *
 * @Note			- This function sets or clears the appropriate bit in the NVIC ISER (Interrupt Set-Enable Register)
 *                    or ICER (Interrupt Clear-Enable Register) based on the IRQ number and enable/disable choice
 */
void LPIT_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		// Enable the interrupt by setting the corresponding bit in the appropriate ISER
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
		// Disable the interrupt by setting the corresponding bit in the appropriate ICER
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

/*******************************************************************************
 * @fn				- GetValueLPIT
 *
 * @brief			- This function retrieves the current value of the LPIT (Low Power Interrupt Timer) counter
 *
 * @param[in]		- None
 *
 * @return			- uint32_t: The current value of the LPIT counter
 *
 * @Note			- This function reads the Current Value register (CVAL0) of the LPIT
 *                 - LPIT is assumed to be properly initialized before calling this function
 */
uint32_t GetValueLPIT(void)
{
	uint32_t value = 0;
	value = (uint32_t)(LPIT->CVAL0); // Read the current value from CVAL0 register
																	 // LPIT->CVAL0 is a pointer to the CVAL0 register
																	 // Casting to uint32_t ensures we get a 32-bit value
	return value;
}

/*******************************************************************************
 * @fn				- LPIT_IRQHandling
 *
 * @brief			- This function handles the interrupt for a specific LPIT channel
 *
 * @param[in]		- Channel: The LPIT channel number (0-3) for which to handle the interrupt
 *
 * @return			- none
 *
 * @Note			- This function clears the interrupt flag for the specified LPIT channel
 *                 - It modifies the Module Status Register (MSR) of the LPIT
 */
void LPIT_IRQHandling(uint8_t Channel)
{
	// Clear the interrupt flag for the specified channel
	// MSR: Module Status Register
	// Writing 1 to the corresponding bit clears the flag
	LPIT->MSR |= (1 << Channel);
}
