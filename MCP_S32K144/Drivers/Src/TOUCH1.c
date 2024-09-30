#include "TOUCH1.h"


void ChargeDistribution1 (void)
{
	// Drive electrode GPIO low
	GPIOC->PCOR = (1 << ELEC1_ELEC_GPIO_PIN);

	// Configure electrode pin as GPIO to precharge electrode
	PORTC->PCR[ELEC1_ELEC_GPIO_PIN] = PCR_GPIO1;

	// Drive Cext GPIO high
	GPIOC->PSOR = (1 << ELEC1_CEXT_GPIO_PIN);

	// Configure Cext pin as GPIO to precharge Cext
	PORTC->PCR[ELEC1_CEXT_GPIO_PIN] = PCR_GPIO1;

	// Configure Electrode and Cext pins as outputs at the same time
	GPIOC->PDDR = ((GPIOC->PDDR) | (ELEC1_PORT_MASK));
}

void ChargeRedistribution1 (void)
{
	// Configure Electrode and Cext pins as inputs at the same time
	GPIOC->PDDR = ((GPIOC->PDDR) & (~(uint32_t)(ELEC1_PORT_MASK)));
}

void ElectrodeFloat1 (void)
{
	// Configure Electrode and Cext pins as inputs at the same time
	GPIOC->PDDR = ((GPIOC->PDDR) & (~(uint32_t)(ELEC1_PORT_MASK)));
}
