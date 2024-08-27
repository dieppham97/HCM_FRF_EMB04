#include "Clocks_and_Modes.h"

uint32_t SOSCDIV1, SOSCDIV2;
uint32_t SIRCDIV1, SIRCDIV2;
uint32_t FIRCDIV1, FIRCDIV2;
uint32_t SPLLDIV1, SPLLDIV2;
static uint32_t V_CO, SPLL_CLK;
static uint32_t DIV_VALUE[8] = {0,1,2,4,8,16,32,64};


void SOSC_Init (SOSC_Config_t SOSC_Handle, uint8_t DIV)
{
	uint32_t temp = 0;
	if (DIV == DIV1)
	{
		temp |= ((uint32_t)SOSC_Handle.SOSC_DIV1 << SOSCDIV_DIV1);
	} else
	{
		temp |= ((uint32_t)SOSC_Handle.SOSC_DIV2 << SOSCDIV_DIV2);
	}
	SCG->SOSCDIV = temp;
	temp = 0;
	temp |= ((uint32_t)SOSC_Handle.SOSC_Range << SOSCCFG_RANGE);
	temp |= ((uint32_t)SOSC_Handle.SOSC_ExOrX << SOSCCFG_EREFS);
	SCG->SOSCCFG = temp;
	
	if (SOSC_Handle.SOSC_ExOrX == SOSC_INTERNAL)
	{
		SOSCDIV1 = SOSC_8M / DIV_VALUE[SOSC_Handle.SOSC_DIV1];
		SOSCDIV2 = SOSC_8M / DIV_VALUE[SOSC_Handle.SOSC_DIV2];
	}
	
	while (SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK);		//check unlock
	SCG_SOSC_EN();	
	
	while (!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK));	//CHECK VALID	
}

void SIRC_Init (SIRC_Config_t SIRC_Handle, uint8_t DIV)
{
	SCG_SIRC_DI();
	
	uint32_t temp = 0;
	if (DIV == DIV1)
	{
		temp |= ((uint32_t)SIRC_Handle.SIRC_DIV1 << SIRCDIV_DIV1);
	} else
	{
		temp |= ((uint32_t)SIRC_Handle.SIRC_DIV2 << SIRCDIV_DIV2);
	}
	SCG->SIRCDIV = temp;
	SCG->SIRCCFG = ((uint32_t)SIRC_Handle.SIRC_Range << SIRCCFG_RANGE);
	
	if (SIRC_Handle.SIRC_Range == SIRC_RANGE_2M)
	{
		SIRCDIV1 = SIRC_2M / DIV_VALUE[SIRC_Handle.SIRC_DIV1];
		SIRCDIV2 = SIRC_2M / DIV_VALUE[SIRC_Handle.SIRC_DIV2];
	}else
	{
		SIRCDIV1 = SIRC_8M / DIV_VALUE[SIRC_Handle.SIRC_DIV1];
		SIRCDIV2 = SIRC_8M / DIV_VALUE[SIRC_Handle.SIRC_DIV2];
	}
	
	while (SCG->SIRCCSR & SCG_SIRCCSR_LK_MASK);		//check unlock
	SCG_SIRC_EN();
}

void FIRC_Init (FIRC_Config_t FIRC_Handle, uint8_t DIV)
{
	SCG_FIRC_DI();
	uint32_t temp = 0;
	if (DIV == DIV1)
	{
		temp |= ((uint32_t)FIRC_Handle.FIRC_DIV1 << FIRCDIV_DIV1);
	} else
	{
		temp |= ((uint32_t)FIRC_Handle.FIRC_DIV2 << FIRCDIV_DIV2);
	}
	SCG->FIRCDIV = temp;
	
	FIRCDIV1 = FIRC_48M / DIV_VALUE[FIRC_Handle.FIRC_DIV1];
	FIRCDIV2 = FIRC_48M / DIV_VALUE[FIRC_Handle.FIRC_DIV2];
	
	while (SCG->FIRCCSR & SCG_FIRCCSR_LK_MASK);		//check unlock
	SCG_FIRC_EN();
}

void SPLL_Init (SPLL_Config_t SPLL_Handle, uint8_t DIV)
{
	while (SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK);
	SCG_SPLL_DI();
	uint32_t temp = 0;
	if (DIV == DIV1)
	{
		temp |= ((uint32_t)SPLL_Handle.SPLL_DIV1 << SPLLDIV_DIV1);
	} else
	{
		temp |= ((uint32_t)SPLL_Handle.SPLL_DIV2 << SPLLDIV_DIV2);
	}
	temp |= ((uint32_t)SPLL_Handle.SPLL_MULT << SPLLCFG_MULT);
	temp |= ((uint32_t)SPLL_Handle.SPLL_PreDiv << SPLLCFG_PREDIV);
	temp |= ((uint32_t)SPLL_Handle.SPLL_Source << SPLLCFG_SOURCE);
	SCG->SPLLCFG = temp;
	
	if(SPLL_Handle.SPLL_Source == SPLL_SOURCE_SOSC)
	{
		SCG->SOSCCFG = (1 << SOSCCFG_EREFS);
		V_CO = ((SPLL_SOSC_8M / (1 + SPLL_Handle.SPLL_PreDiv)) * (SPLL_Handle.SPLL_MULT + 16));
		SPLL_CLK = (V_CO/2);
		SPLLDIV1 = SPLL_CLK /  DIV_VALUE[SPLL_Handle.SPLL_DIV1];
		SPLLDIV1 = SPLL_CLK /  DIV_VALUE[SPLL_Handle.SPLL_DIV1];
	}else if (SPLL_Handle.SPLL_Source == SPLL_SOURCE_FIRC)
	{
		V_CO = ((SPLL_FIRC_48M / (1 + SPLL_Handle.SPLL_PreDiv)) * (SPLL_Handle.SPLL_MULT + 16));
		SPLL_CLK = (V_CO/2);
		SPLLDIV1 = SPLL_CLK /  DIV_VALUE[SPLL_Handle.SPLL_DIV1];
		SPLLDIV1 = SPLL_CLK /  DIV_VALUE[SPLL_Handle.SPLL_DIV1];
	}
	
	while (SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK);
	SCG_SPLL_EN();
	
	while (!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK));	
}

void SCG_Init (SourceClock_Config_t Source_Handle)
{
	if (Source_Handle.Soure == Source_SOSC)
	{
		SOSC_Init(Source_Handle.SOSC_Config, Source_Handle.DIV);
	} else if(Source_Handle.Soure == Source_SIRC)
	{
		SIRC_Init (Source_Handle.SIRC_Config, Source_Handle.DIV);
	} else if(Source_Handle.Soure == Source_FIRC)
	{
		FIRC_Init (Source_Handle.FIRC_Config, Source_Handle.DIV);
	} else if(Source_Handle.Soure == Source_SPLL)
	{
		SPLL_Init (Source_Handle.SPLL_Config, Source_Handle.DIV);
	}
}


#if 0
void SourceSOSC_8Mhz (void)
{
	SCG->SOSCDIV |= (DIV_1 << SOSCDIV_DIV1);
	SCG->SOSCDIV |= (DIV_1 << SOSCDIV_DIV2);
	SCG->SOSCCFG |= (SOSC_INTERNAL << SOSCCFG_EREFS);
	SCG->SOSCCFG &= ~(3U << SOSCCFG_RANGE);
	SCG->SOSCCFG |= (SOSC_MEDIUM << SOSCCFG_RANGE);
	
	while (SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK);		//check unlock
	SCG_SOSC_EN();
	
	while (!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK));	//CHECK VALID
}

void SourcePLL (void)
{
	while (SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK);
	SCG_SPLL_DI();
	SCG->SPLLDIV |= (DIV_4 << SPLLDIV_DIV2);
	
	/* 	The SPLL_CLK = (VCO_CLK)/2 
			VCO_CLK = SPLL_SOURCE/(PREDIV + 1) X (MULT + 16)
	*/
	SCG->SPLLCFG = (SPLL_PREDIV1 << SPLLCFG_PREDIV) 
			| (SPLL_MULT40 << SPLLCFG_MULT);		/* VCO = 320 Mhz, SPLLCLK = 160MHz*/
	
	while (SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK);
	SCG_SPLL_EN();
	
	while (!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK));

}
#endif

void RunNormalMode_80MHz(void)
{
	/* Run mode */
	SCG->RCCR = ((SCS_PLL << RCCR_SCS)	| (1 << RCCR_DIVCORE) 
						| (1 << RCCR_DIVBUS) 
						| (2 << RCCR_DIVSLOW));
	while (((SCG->CSR & 0xF000000) >> 24) != 6);
}
