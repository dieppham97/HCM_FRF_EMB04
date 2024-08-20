#include "LPIT.h"

void LPIT_PeriClockControl(uint8_t selectClock, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		switch (selectClock)
		{
			case 1:
				PCC_LPIT_Clock1();
				PCC_LPIT_EN();
				break;
			case 2:
				PCC_LPIT_Clock2();
				PCC_LPIT_EN();			
				break;
			case 3:
				PCC_LPIT_Clock3();	
				PCC_LPIT_EN();
				break;
			case 4:
				PCC_LPIT_Clock4();	
				PCC_LPIT_EN();
				break;
			case 5:
				PCC_LPIT_Clock5();	
				PCC_LPIT_EN();
				break;
			case 6:
				PCC_LPIT_Clock6();	
				PCC_LPIT_EN();
				break;
			case 7:
				PCC_LPIT_Clock7();	
				PCC_LPIT_EN();
				break;
			default:
				break;				
		}
	} else
	{
		PCC_LPIT_DI();
	}
}

void ChannelClock(LPIT_PinConfig_t LPIT_PinConfig)
{
	switch (LPIT_PinConfig.Channel)
	{
		case 0:
		{
			/* Config channel */
			LPIT->TCTRL0 |= ((uint32_t)LPIT_PinConfig.Mode << TCR_MODE);
			/* Enable time channel */
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

void LPIT_Init(LPIT_PinConfig_t LPIT_PinConfig)
{
	/* Enable clock Module */ 
	LPIT->MCR |= (1 << LPIT_MCR);		
	
	/* Run debug mode */
	LPIT->MCR |= (1 << LPIT_DBG_EN);

	/* Interrupt enable */
	if (LPIT_PinConfig.Interrupt == ENABLE)
	{
		LPIT->MIER |= (1 << LPIT_PinConfig.Channel);
	}	
	
	/* Set time value */
	switch (LPIT_PinConfig.Channel)
	{
		case 0:
		{
			/* Config channel */
			LPIT->TVAL0 = ((LPIT_PinConfig.ValueTime * (LPO128_CLK/1000))-1);
			break;
		}
		case 1:
		{
			LPIT->TVAL1 = ((LPIT_PinConfig.ValueTime * (LPO128_CLK/1000))-1);
			break;
		}
		case 2:
		{
			LPIT->TVAL2 = ((LPIT_PinConfig.ValueTime * (LPO128_CLK/1000))-1);
			break;
		}
		case 3:
		{
			LPIT->TVAL3 = ((LPIT_PinConfig.ValueTime * (LPO128_CLK/1000))-1);
			break;
		}
	}

	/*Config and Enable channel */
	ChannelClock(LPIT_PinConfig);
}

void LPIT_IRQInterruptConfig (uint8_t IRQNumber, uint8_t EnOrDi)
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

uint32_t GetValueLPIT (void)
{
	uint32_t value = 0;
	value = (uint32_t)(LPIT->CVAL0);
	return value;
}

void LPIT_IRQHandling (uint8_t Channel)
{
	LPIT->MSR |= (1 << Channel);
}
