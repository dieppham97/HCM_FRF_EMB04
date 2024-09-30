#ifndef TOUCH2_H_
#define TOUCH2_H_

#include "S32K1xx.h"

/*
 * PCR defines
 */
 
// Configure pin as GPIO, clear ISF, ISF disabled, ALT1=GPIO,
// high drive strength, disable pulls, fast slew rate
#define PCR_GPIO2	0x01000140

// Configure pin as analog input
#define PCR_ANA2	0x00000000

/*
 * Electrode 2 (SW8)
 */
#define ELEC2_ADC_CHANNEL     8
#define ELEC2_ELEC_GPIO_PIN   2U
#define ELEC2_CEXT_GPIO_PIN   0U
#define ELEC2_PORT_MASK       (1U << ELEC2_ELEC_GPIO_PIN) | (1U << ELEC2_CEXT_GPIO_PIN)

void ChargeDistribution2 (void)
{
	// Drive electrode GPIO low
	GPIOC->PCOR = 1 << ELEC2_ELEC_GPIO_PIN;

	// Configure electrode pin as GPIO to precharge electrode
	PORTC->PCR[ELEC2_ELEC_GPIO_PIN] = PCR_GPIO2;

	// Drive Cext GPIO high
	GPIOC->PSOR = 1 << ELEC2_CEXT_GPIO_PIN;

	// Configure Cext pin as GPIO to precharge Cext
	PORTC->PCR[ELEC2_CEXT_GPIO_PIN] = PCR_GPIO2;

	// Configure Electrode and Cext pins as outputs at the same time
	GPIOC->PDDR = ((GPIOC->PDDR) | (ELEC2_PORT_MASK));
}

void ChargeRedistribution2 (void)
{
	// Configure Electrode and Cext pins as inputs at the same time
	GPIOC->PDDR = ((GPIOC->PDDR) & (~(ELEC2_PORT_MASK)));
}

void ElectrodeFloat2 (void)
{
	// Configure Electrode and Cext pins as inputs at the same time
	GPIOC->PDDR = ((GPIOC->PDDR) & (~(ELEC2_PORT_MASK)));
}

#endif
