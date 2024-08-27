#include "S32K1xx_gpio_driver.h"


void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		if (pGPIOx == GPIOA)
		{
			PCC_PORTA_EN();
		}else if (pGPIOx == GPIOB)
		{
			PCC_PORTB_EN();
		}else if (pGPIOx == GPIOC)
		{
			PCC_PORTC_EN();
		}else if (pGPIOx == GPIOD)
		{
			PCC_PORTD_EN();
		}
	}else
	{
		if (pGPIOx == GPIOA)
		{
			PCC_PORTA_DI();
		}else if (pGPIOx == GPIOB)
		{
			PCC_PORTB_DI();
		}else if (pGPIOx == GPIOC)
		{
			PCC_PORTC_DI();
		}else if (pGPIOx == GPIOD)
		{
			PCC_PORTD_DI();
		}
	}
}

void SetPinGPIO (GPIO_RegDef_t *pGPIOx, uint8_t mode, uint8_t pinNumber)
{
	if (pGPIOx == GPIOA)
	{
		PORTA->PCR[pinNumber] |= (uint32_t)(mode << PORT_PCR_MUX);
	}else if (pGPIOx == GPIOB)
	{
		PORTB->PCR[pinNumber] |= (uint32_t)(mode << PORT_PCR_MUX);
	}else if (pGPIOx == GPIOC)
	{
		PORTC->PCR[pinNumber] |= (uint32_t)(mode << PORT_PCR_MUX);
	}else if (pGPIOx == GPIOD)
	{
		PORTD->PCR[pinNumber] |= (uint32_t)(mode << PORT_PCR_MUX);
	}
}

void DisPUPD (GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
	if (pGPIOx == GPIOA)
	{
		PORTA->PCR[pinNumber] &= ~(1U << PORT_PCR_PE);
	}else if (pGPIOx == GPIOB)
	{
		PORTB->PCR[pinNumber] &= ~(1U << PORT_PCR_PE);
	}else if (pGPIOx == GPIOC)
	{
		PORTC->PCR[pinNumber] &= ~(1U << PORT_PCR_PE);
	}else if (pGPIOx == GPIOD)
	{
		PORTD->PCR[pinNumber] &= ~(1U << PORT_PCR_PE);
	}
}

void setModeIT (GPIO_RegDef_t *pGPIOx, uint8_t modeIT, uint8_t pinNumber)
{
		if (pGPIOx == GPIOA)
	{
		PORTA->PCR[pinNumber] |= (uint32_t)(modeIT << PORT_PCR_IRQ);
	}else if (pGPIOx == GPIOB)
	{
		PORTB->PCR[pinNumber] |= (uint32_t)(modeIT << PORT_PCR_IRQ);
	}else if (pGPIOx == GPIOC)
	{
		PORTC->PCR[pinNumber] |= (uint32_t)(modeIT << PORT_PCR_IRQ);
	}else if (pGPIOx == GPIOD)
	{
		PORTD->PCR[pinNumber] |= (uint32_t)(modeIT << PORT_PCR_IRQ);
	}
}

void GPIO_Init (GPIO_Handle_t *pGPIOHandle)
{
	/* Enable clock  */
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);
	
	/* Set pin as GPIO */
	SetPinGPIO(pGPIOHandle->pGPIOx, pGPIOHandle->GPIO_PinConfig.GPIO_PinAlterFunMode, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	/* Disable PUPD */
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == DISABLE)
	{
		DisPUPD(pGPIOHandle->pGPIOx, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}else
	{
		/* do anything */
	}
	
	pGPIOHandle->pGPIOx->PDOR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	
	/* Set pin as Output */
	if(pGPIOHandle->GPIO_PinConfig.GPIO_InOut == OUT_PUT)
	{
		pGPIOHandle->pGPIOx->PDDR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->PDDR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}else
	{
		pGPIOHandle->pGPIOx->PDDR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		/*pGPIOHandle->pGPIOx->PDIR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); */
	}
	
	/*Set mode interrupt*/
	if (pGPIOHandle->GPIO_PinConfig.GPIO_Interrupt == ENABLE)
	{
		if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_L0)
		{
			setModeIT (pGPIOHandle->pGPIOx, IT_L0, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_RE)
		{
			setModeIT (pGPIOHandle->pGPIOx, IT_RE, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_FE)
		{
			setModeIT (pGPIOHandle->pGPIOx, IT_FE, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_RFE)
		{
			setModeIT (pGPIOHandle->pGPIOx, IT_RFE, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_ModeIT == IT_L1)
		{
			setModeIT (pGPIOHandle->pGPIOx, IT_L1, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
	}else
	{
		setModeIT (pGPIOHandle->pGPIOx, 0, pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	
}

/*
 * Data read and write
 */
void GPIO_WriteToOutput (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber,uint8_t EnOrDe)
{
	if (EnOrDe == ENABLE)
	{
		pGPIOx->PDOR |= (1 << PinNumber);
	}else
	{
		pGPIOx->PDOR &= ~(1 << PinNumber);
	}
}
void GPIO_TogglePin (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->PDOR ^= (1 << PinNumber);
}

uint8_t ReadFromInput (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t) ((pGPIOx->PDIR >> PinNumber) & 0x01);
	return value;
}

/*
 * IRQ Configuration and ISR handling (whenever interrupt occurs so they user application can call this function in order to process that interrupt
 */
void GPIO_IRQInterruptConfig (uint8_t IRQNumber, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
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

void GPIO_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t IPRx = IRQNumber / 4;	/* div */
	uint8_t IPRx_section = IRQNumber % 4; /* mod */
	*(NVIC_PR_BASE_ADDR + IPRx) |= (uint32_t)IRQPriority << ((8U * IPRx_section) + 6);
}

void GPIO_IRQHandling (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	if (pGPIOx == GPIOA)
	{
		PORTA->PCR[PinNumber] |= (1 << PORT_PCR_ISF);
	}else if (pGPIOx == GPIOB)
	{
		PORTB->PCR[PinNumber] |= (1 << PORT_PCR_ISF);
	}else if (pGPIOx == GPIOC)
	{
		PORTC->PCR[PinNumber] |= (1 << PORT_PCR_ISF);
	}else if (pGPIOx == GPIOD)
	{
		//if (PORTD->PCR[PinNumber] &= (1 << PORT_PCR_ISF))
		PORTD->PCR[PinNumber] |= (1 << PORT_PCR_ISF);
	}
}

uint8_t checkLed (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->PDOR >> PinNumber) & 0x01);
	return value;
}
