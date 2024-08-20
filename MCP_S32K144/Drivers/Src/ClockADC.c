#include "ClockADC.h"

void SourceSOSC_8Mhz (void)
{
	SCG->SOSCDIV |= (DIV_1 << SOSCDIV_DIV1);
	SCG->SOSCDIV |= (DIV_1 << SOSCDIV_DIV2);
	SCG->SOSCCFG |= (INTERNAL << SOSCCFG_EREFS);
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
	SCG->SPLLCFG = (SPLL_PREDIV1 << SPLLCFG_PREDIV) | (SPLL_MULT40 << SPLLCFG_MULT);		/* VCO = 320 Mhz, SPLLCLK = 160MHz*/
	
	while (SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK);
	SCG_SPLL_EN();
	
	while (!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK));

}

void RunNormalMode_80MHz(void)
{
	SCG->SIRCDIV |= (1 << SIRCDIV_DIV2);
	/* Run mode */
	SCG->RCCR = ((SCS_PLL << RCCR_SCS) |(1 << RCCR_DIVCORE) | (1 << RCCR_DIVBUS) | (2 << RCCR_DIVSLOW));
/*	SCG->RCCR |= (1 << RCCR_DIVCORE);
	SCG->RCCR |= (1 << RCCR_DIVBUS);
	SCG->RCCR |= (2 << RCCR_DIVSLOW);*/
	
	while (((SCG->CSR & 0xF000000) >> 24) != 6);

}

void ADC_Init (void)
{
	PCC_ADC0_SPLLCLK();
	/* Enable ADC0 */
	PCC_ADC0_EN();
	
	ADC0->SC1[0] |= ADC_SC1_ADCH_MASK;	//disable channel + IT
	
	ADC0->CFG1 |= (CFG1_ADICLK << ADC_CFG1_ADICLK);
	ADC0->CFG1 |= (CFG1_ADIV_DIV1 << ADC_CFG1_ADIV);
	ADC0->CFG1 |= (CFG1_MODE_12BIT << ADC_CFG1_MODE);
	
	ADC0->CFG2 = 12;
	
	ADC0->SC2 = 0;
	
	ADC0->SC3 = 0;
}

void ADC_channel_convert( uint16_t channel)
{
	ADC0->SC1[0] &= ~ADC_SC1_ADCH_MASK;
	ADC0->SC1[0] = channel;
}

uint8_t ADC_conversion_complete (void)
{
	/* Conversion Flag Status [COCO] */
	return ((ADC0->SC1[0] & 0x80u) >> 7u);
}

uint32_t ADC_Channel_Read(void)
{
	uint32_t adc_result = 0;
	adc_result = ADC0->R[0];			/* For SW trigger mode, R[0] is used */
	return (uint32_t)((5000 * adc_result) / 0xFFF);	/* Convert result to mV for 0-5 V range */
}
