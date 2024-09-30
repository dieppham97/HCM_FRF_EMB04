/*
 * S32K1xx_gpio_driver.c
 *
 *  Created on: Sep 29, 2023
 *      Author: Diep Pham
 */
#include "S32K1xx_gpio_driver.h"

/*******************************************************************************
 * @fn				- GPIO_PeriClockControl
 *
 * @brief			- This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]		- base address of the gpio peripheral
 * @param[in]		-ENABLE or DISABLE macros
 * @param[in]		-
 *
 * @return			-none
 *
 * @Note			-none

 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		if (pGPIOx == GPIOA)
		{
			PCC_PORTA_EN();
		}
		else if (pGPIOx == GPIOB)
		{
			PCC_PORTB_EN();
		}
		else if (pGPIOx == GPIOC)
		{
			PCC_PORTC_EN();
		}
		else if (pGPIOx == GPIOD)
		{
			PCC_PORTD_EN();
		}
	}
	else
	{
		if (pGPIOx == GPIOA)
		{
			PCC_PORTA_DI();
		}
		else if (pGPIOx == GPIOB)
		{
			PCC_PORTB_DI();
		}
		else if (pGPIOx == GPIOC)
		{
			PCC_PORTC_DI();
		}
		else if (pGPIOx == GPIOD)
		{
			PCC_PORTD_DI();
		}
	}
}

/*******************************************************************************
 * @fn				- SetPinGPIO
 *
 * @brief			- This function sets the mode for a specific GPIO pin
 *
 * @param[in]		- base address of the gpio peripheral
 * @param[in]		- mode to set for the pin (e.g., GPIO, ALT1, ALT2, etc.)
 * @param[in]		- pin number to configure
 *
 * @return			- none
 *
 * @Note			- This function modifies the Pin Control Register (PCR) for the specified pin
 */
void SetPinGPIO(GPIO_RegDef_t *pGPIOx, uint8_t mode, uint8_t pinNumber)
{
	if (pGPIOx == GPIOA)
	{
		PORTA->PCR[pinNumber] |= (uint32_t)(mode << PORT_PCR_MUX);
	}
	else if (pGPIOx == GPIOB)
	{
		PORTB->PCR[pinNumber] |= (uint32_t)(mode << PORT_PCR_MUX);
	}
	else if (pGPIOx == GPIOC)
	{
		PORTC->PCR[pinNumber] |= (uint32_t)(mode << PORT_PCR_MUX);
	}
	else if (pGPIOx == GPIOD)
	{
		PORTD->PCR[pinNumber] |= (uint32_t)(mode << PORT_PCR_MUX);
	}
}

/*******************************************************************************
 * @fn				- EnPUPD
 *
 * @brief			- This function enables the pull-up or pull-down resistor for a specific GPIO pin
 *
 * @param[in]		- pointer to GPIO handle structure
 * @param[in]		- pin number to configure
 *
 * @return			- none
 *
 * @Note			- This function modifies the Pin Control Register (PCR) to enable pull-up/pull-down and set the appropriate resistor type
 */
void EnPUPD(GPIO_Handle_t *pGPIOHandle, uint8_t pinNumber)
{
	if (pGPIOHandle->pGPIOx == GPIOA)
	{
		PORTA->PCR[pinNumber] |= (1U << PORT_PCR_PE);
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdType == GPIO_PU)
			PORTA->PCR[pinNumber] |= (1U << PORT_PCR_PS);
	}
	else if (pGPIOHandle->pGPIOx == GPIOB)
	{
		PORTB->PCR[pinNumber] |= (1U << PORT_PCR_PE);
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdType == GPIO_PU)
			PORTB->PCR[pinNumber] |= (1U << PORT_PCR_PS);
	}
	else if (pGPIOHandle->pGPIOx == GPIOC)
	{
		PORTC->PCR[pinNumber] |= (1U << PORT_PCR_PE);
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdType == GPIO_PU)
			PORTC->PCR[pinNumber] |= (1U << PORT_PCR_PS);
	}
	else if (pGPIOHandle->pGPIOx == GPIOD)
	{
		PORTD->PCR[pinNumber] |= (1U << PORT_PCR_PE);
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdType == GPIO_PU)
			PORTD->PCR[pinNumber] |= (1U << PORT_PCR_PS);
	}
}

/*******************************************************************************
 * @fn				- DisPUPD
 *
 * @brief			- This function disables the pull-up or pull-down resistor for a specific GPIO pin
 *
 * @param[in]		- pGPIOx: base address of the GPIO peripheral
 * @param[in]		- pinNumber: pin number to configure
 *
 * @return			- none
 *
 * @Note			- This function modifies the Pin Control Register (PCR) to disable pull-up/pull-down for the specified pin
 */
void DisPUPD(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
	if (pGPIOx == GPIOA)
	{
		PORTA->PCR[pinNumber] &= ~(1U << PORT_PCR_PE);
	}
	else if (pGPIOx == GPIOB)
	{
		PORTB->PCR[pinNumber] &= ~(1U << PORT_PCR_PE);
	}
	else if (pGPIOx == GPIOC)
	{
		PORTC->PCR[pinNumber] &= ~(1U << PORT_PCR_PE);
	}
	else if (pGPIOx == GPIOD)
	{
		PORTD->PCR[pinNumber] &= ~(1U << PORT_PCR_PE);
	}
}

/*******************************************************************************
 * @fn				- setModeIT
 *
 * @brief			- This function sets the interrupt mode for a specific GPIO pin
 *
 * @param[in]		- pGPIOx: base address of the GPIO peripheral
 * @param[in]		- modeIT: interrupt mode to set
 * @param[in]		- pinNumber: pin number to configure
 *
 * @return			- none
 *
 * @Note			- This function modifies the Pin Control Register (PCR) to set the interrupt mode for the specified pin
 */
void setModeIT(GPIO_RegDef_t *pGPIOx, uint8_t modeIT, uint8_t pinNumber)
{
	if (pGPIOx == GPIOA)
	{
		PORTA->PCR[pinNumber] |= (uint32_t)(modeIT << PORT_PCR_IRQ);
	}
	else if (pGPIOx == GPIOB)
	{
		PORTB->PCR[pinNumber] |= (uint32_t)(modeIT << PORT_PCR_IRQ);
	}
	else if (pGPIOx == GPIOC)
	{
		PORTC->PCR[pinNumber] |= (uint32_t)(modeIT << PORT_PCR_IRQ);
	}
	else if (pGPIOx == GPIOD)
	{
		PORTD->PCR[pinNumber] |= (uint32_t)(modeIT << PORT_PCR_IRQ);
	}
}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	/* Enable clock  */
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	/* Set pin as GPIO */
	SetPinGPIO(pGPIOHandle->pGPIOx, pGPIOHandle->GPIO_PinConfig.GPIO_PinAlterFunMode, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	/* Disable PUPD */
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == DISABLE)
	{
		DisPUPD(pGPIOHandle->pGPIOx, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == ENABLE)
	{
		EnPUPD(pGPIOHandle, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

	pGPIOHandle->pGPIOx->PDOR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	/* Set pin as Output */
	if (pGPIOHandle->GPIO_PinConfig.GPIO_InOut == OUT_PUT)
	{
		pGPIOHandle->pGPIOx->PDDR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->PDDR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	else if (pGPIOHandle->GPIO_PinConfig.GPIO_InOut == IN_PUT)
	{
		pGPIOHandle->pGPIOx->PDDR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		/*pGPIOHandle->pGPIOx->PDIR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); */
	}

	/*Set mode interrupt*/
	if (pGPIOHandle->GPIO_PinConfig.GPIO_Interrupt == ENABLE)
	{
		if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_L0)
		{
			setModeIT(pGPIOHandle->pGPIOx, IT_L0, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_RE)
		{
			setModeIT(pGPIOHandle->pGPIOx, IT_RE, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_FE)
		{
			setModeIT(pGPIOHandle->pGPIOx, IT_FE, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_RFE)
		{
			setModeIT(pGPIOHandle->pGPIOx, IT_RFE, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_L1)
		{
			setModeIT(pGPIOHandle->pGPIOx, IT_L1, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
	}
	else if (pGPIOHandle->GPIO_PinConfig.GPIO_Interrupt == DISABLE)
	{
		setModeIT(pGPIOHandle->pGPIOx, 0, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
}

/*******************************************************************************
 * @fn				- GPIO_WriteToOutput
 *
 * @brief			- This function writes a value to the output of a specific GPIO pin
 *
 * @param[in]		- pGPIOx: base address of the GPIO peripheral
 * @param[in]		- PinNumber: pin number to write to
 * @param[in]		- EnOrDe: ENABLE to set the pin high, DISABLE to set the pin low
 *
 * @return			- none
 *
 * @Note			- This function modifies the Port Data Output Register (PDOR) to set or clear the specified pin
 */
void GPIO_WriteToOutput(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t EnOrDe)
{
	if (EnOrDe == ENABLE)
	{
		pGPIOx->PDOR |= (1 << PinNumber);
	}
	else
	{
		pGPIOx->PDOR &= ~(1 << PinNumber);
	}
}

/*******************************************************************************
 * @fn				- GPIO_TogglePin
 *
 * @brief			- This function toggles the output state of a specific GPIO pin
 *
 * @param[in]		- pGPIOx: base address of the GPIO peripheral
 * @param[in]		- PinNumber: pin number to toggle
 *
 * @return			- none
 *
 * @Note			- This function uses the XOR operation on the Port Data Output Register (PDOR) to toggle the specified pin
 */
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->PDOR ^= (1 << PinNumber);
}

/*******************************************************************************
 * @fn				- ReadFromInput
 *
 * @brief			- This function reads the input state of a specific GPIO pin
 *
 * @param[in]		- pGPIOx: base address of the GPIO peripheral
 * @param[in]		- PinNumber: pin number to read from
 *
 * @return			- uint8_t: The state of the input pin (0 or 1)
 *
 * @Note			- This function reads the Port Data Input Register (PDIR) to get the state of the specified pin
 */
uint8_t ReadFromInput(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->PDIR >> PinNumber) & 0x01);
	return value;
}

/*******************************************************************************
 * @fn				- GPIO_IRQInterruptConfig
 *
 * @brief			- This function configures the interrupt for a specific IRQ number
 *
 * @param[in]		- IRQNumber: IRQ number to configure
 * @param[in]		- EnOrDi: ENABLE to enable the interrupt, DISABLE to disable it
 *
 * @return			- none
 *
 * @Note			- This function enables or disables the interrupt in the NVIC (Nested Vectored Interrupt Controller)
 *                    for the specified IRQ number by setting the appropriate bit in the NVIC_ISERx or NVIC_ICERx register
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		if (IRQNumber <= 31)
		{
			*NVIC_ISER0 |= (1 << IRQNumber);
		}
		else if (31 < IRQNumber && IRQNumber < 64)
		{
			*NVIC_ISER1 |= (1 << IRQNumber % 32);
		}
		else if (64 <= IRQNumber && IRQNumber < 96)
		{
			*NVIC_ISER2 |= (1 << IRQNumber % 64);
		}
		else if (96 <= IRQNumber && IRQNumber < 128)
		{
			*NVIC_ISER3 |= (1 << IRQNumber % 96);
		}
		else if (128 <= IRQNumber && IRQNumber < 160)
		{
			*NVIC_ISER4 |= (1 << IRQNumber % 128);
		}
		else if (160 <= IRQNumber && IRQNumber < 192)
		{
			*NVIC_ISER5 |= (1 << IRQNumber % 160);
		}
	}
	else
	{
		if (IRQNumber <= 31)
		{
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if (31 < IRQNumber && IRQNumber < 64)
		{
			*NVIC_ICER1 |= (1 << IRQNumber % 32);
		}
		else if (64 <= IRQNumber && IRQNumber < 96)
		{
			*NVIC_ICER2 |= (1 << IRQNumber % 64);
		}
		else if (96 <= IRQNumber && IRQNumber < 128)
		{
			*NVIC_ICER3 |= (1 << IRQNumber % 96);
		}
		else if (128 <= IRQNumber && IRQNumber < 160)
		{
			*NVIC_ICER4 |= (1 << IRQNumber % 128);
		}
		else if (160 <= IRQNumber && IRQNumber < 192)
		{
			*NVIC_ICER5 |= (1 << IRQNumber % 160);
		}
	}
}

/*******************************************************************************
 * @fn				- GPIO_IRQPriorityConfig
 *
 * @brief			- This function configures the priority of a specific interrupt
 *
 * @param[in]		- IRQNumber: IRQ number of the interrupt
 * @param[in]		- IRQPriority: Priority level to be set for the interrupt
 *
 * @return			- none
 *
 * @Note			- This function sets the priority of the specified interrupt in the NVIC Priority Register
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t IPRx = IRQNumber / 4;					/* div */
	uint8_t IPRx_section = IRQNumber % 4; /* mod */
	*(NVIC_PR_BASE_ADDR + IPRx) |= (uint32_t)IRQPriority << ((8U * IPRx_section) + 6);
}

/*******************************************************************************
 * @fn				- GPIO_IRQHandling
 *
 * @brief			- This function handles GPIO interrupts by clearing the Interrupt Status Flag (ISF)
 *
 * @param[in]		- pGPIOx: base address of the GPIO peripheral
 * @param[in]		- PinNumber: pin number that triggered the interrupt
 *
 * @return			- none
 *
 * @Note			- This function clears the ISF bit in the Pin Control Register (PCR) for the specified pin
 *					  The ISF bit is cleared by writing 1 to it, which might seem counterintuitive
 *					  The function checks which GPIO port is involved and accesses the corresponding PORT register
 *					  PORTA, PORTB, PORTC, and PORTD are specific PORT registers for each GPIO port
 *					  PORT_PCR_ISF is likely a macro defining the bit position of the ISF in the PCR
 */
void GPIO_IRQHandling(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	if (pGPIOx == GPIOA)
	{
		PORTA->PCR[PinNumber] |= (1 << PORT_PCR_ISF);
	}
	else if (pGPIOx == GPIOB)
	{
		PORTB->PCR[PinNumber] |= (1 << PORT_PCR_ISF);
	}
	else if (pGPIOx == GPIOC)
	{
		PORTC->PCR[PinNumber] |= (1 << PORT_PCR_ISF);
	}
	else if (pGPIOx == GPIOD)
	{
		PORTD->PCR[PinNumber] |= (1 << PORT_PCR_ISF);
	}
}

/*******************************************************************************
 * @fn				- checkLed
 *
 * @brief			- This function checks the state of a specific GPIO pin (LED)
 *
 * @param[in]		- pGPIOx: base address of the GPIO peripheral
 * @param[in]		- PinNumber: pin number to check
 *
 * @return			- uint8_t: 1 if LED is on, 0 if LED is off
 *
 * @Note			- This function reads the Port Data Output Register (PDOR) to determine the LED state
 *					- The function uses bit shifting and masking to isolate the state of the specific pin:
 *					  1. (pGPIOx->PDOR >> PinNumber) shifts the register value right by PinNumber bits
 *					  2. & 0x01 masks all bits except the least significant bit
 *					- The result is cast to uint8_t to ensure a consistent return type
 *					- Assumes that a logic high (1) in PDOR corresponds to the LED being on
 */
uint8_t checkLed(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->PDOR >> PinNumber) & 0x01);
	return value;
}
