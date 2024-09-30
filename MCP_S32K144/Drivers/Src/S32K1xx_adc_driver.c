/*
 * S32K1xx_adc_driver.c
 *
 *  Created on: Sep 29, 2023
 *      Author: Diep Pham
 */

#include "S32K1xx_adc_driver.h"

/*******************************************************************************
 * @fn              - ADC_Init
 *
 * @brief           - This function initializes the ADC0 module with specific
 *                    configurations for clock source, resolution, and other
 *                    settings.
 *
 * @param[in]       - None
 *
 * @return          - None
 *
 * @Note            - This function assumes that the necessary clock and PCC
 *                    configurations are already set up.
 */
void ADC_Init(void)
{
	uint32_t temp = 0;

	// Configure ADC0 clock source to FIRC (Fast IRC)
	PCC_ADC0_FIRCCLK();

	// Enable ADC0 module
	PCC_ADC0_EN();

	// Disable all channels (set to 0x1F)
	ADC0->SC1[0] |= ADC_SC1_ADCH_MASK;

	// Configure ADC0 settings
	temp |= (CFG1_ADICLK << ADC_CFG1_ADICLK);		// Set ADC clock source
	temp |= (CFG1_ADIV_DIV1 << ADC_CFG1_ADIV);	// Set clock divider to 1
	temp |= (CFG1_MODE_12BIT << ADC_CFG1_MODE); // Set 12-bit conversion mode

	// Apply configuration to ADC0 CFG1 register
	ADC0->CFG1 = temp;

	// Set ADC0 CFG2 register (sample)
	ADC0->CFG2 = 12;

	ADC0->SC2 = 0;

	ADC0->SC3 = 0;
}

/*******************************************************************************
 * @fn              - ADC_channel_convert
 *
 * @brief           - This function configures the ADC0 module to convert the specified channel.
 *                    It clears the current channel selection and sets the new channel.
 *
 * @param[in]       - channel: The ADC channel number to be converted (uint16_t)
 *
 * @return          - None
 *
 * @Note            - This function assumes that the ADC0 module has been properly initialized.
 */
void ADC_channel_convert(uint16_t channel)
{
	// Clear the current channel selection
	ADC0->SC1[0] &= ~ADC_SC1_ADCH_MASK;

	// Set the channel for conversion
	ADC0->SC1[0] = (channel);
}

/*******************************************************************************
 * @fn              - ADC_conversion_complete
 *
 * @brief           - This function checks if the ADC conversion is complete by
 *                    reading the Conversion Complete (COCO) flag in the ADC0
 *                    Status and Control Register 1 (SC1[0]).
 *
 * @param[in]       - None
 *
 * @return          - uint8_t: 1 if conversion is complete, 0 otherwise
 *
 * @Note            - This function assumes that ADC0 has been properly initialized
 *                    and a conversion has been started.
 */
uint8_t ADC_conversion_complete(void)
{
	/*
	 * Read the Conversion Complete (COCO) flag from ADC0->SC1[0]
	 * COCO is the 7th bit (bit 7) of SC1[0] register
	 * Shift right by 7 to move COCO to the least significant bit
	 * AND with 0x1 to isolate the COCO bit
	 */
	return ((ADC0->SC1[0] >> 7) & 0x1);
}

/*******************************************************************************
 * @fn              - ADC_Channel_Read
 *
 * @brief           - This function reads the result of the ADC conversion from
 *                    the ADC0 module and converts it to a voltage value in mV.
 *
 * @param[in]       - None
 *
 * @return          - uint32_t: The converted voltage value in mV (0-5000 mV range)
 *
 * @Note            - This function assumes that the ADC0 module has been properly
 *                    initialized and a conversion has been completed.
 */
uint32_t ADC_Channel_Read(void)
{
	uint32_t adc_result = 0;

	/* Read the conversion result from ADC0->R[0] register */
	/* For software trigger mode, R[0] is used to store the result */
	adc_result = ADC0->R[0];

	/* Convert ADC result to mV for 0-5 V range */
	/* Formula explanation:
	 * - ADC resolution is 12-bit, so max value is 0xFFF (4095)
	 * - Multiply by 5000 (max mV) and divide by 0xFFF to get mV
	 * - Simplified to (5000 * adc_result) / 0xFFF
	 * - Further optimized to (15 * adc_result) / 0xFFF to avoid potential overflow
	 */
	return (uint32_t)((15 * adc_result) / 0xFFF);
}
