#ifndef TOUCH1_H_
#define TOUCH1_H_

#include "S32K1xx.h"

/*
 * PCR defines
 */
 
// Configure pin as GPIO, clear ISF, ISF disabled, ALT1=GPIO,
// high drive strength, disable pulls, fast slew rate
#define PCR_GPIO1	0x01000140

// Configure pin as analog input
#define PCR_ANA1	0x00000000

/*
 * Electrode 1 (SW7)
 */
#define ELEC1_ADC_CHANNEL     11U
#define ELEC1_ELEC_GPIO_PIN   1U
#define ELEC1_CEXT_GPIO_PIN   3U
#define ELEC1_PORT_MASK       (1U << ELEC1_ELEC_GPIO_PIN) | (1U << ELEC1_CEXT_GPIO_PIN)

static inline void ChargeDistribution1 (void)
{
	// Drive electrode GPIO low
	GPIOC->PCOR = 1 << ELEC1_ELEC_GPIO_PIN;

	// Configure electrode pin as GPIO to precharge electrode
	PORTC->PCR[ELEC1_ELEC_GPIO_PIN] = PCR_GPIO1;

	// Drive Cext GPIO high
	GPIOC->PSOR = 1 << ELEC1_CEXT_GPIO_PIN;

	// Configure Cext pin as GPIO to precharge Cext
	PORTC->PCR[ELEC1_CEXT_GPIO_PIN] = PCR_GPIO1;

	// Configure Electrode and Cext pins as outputs at the same time
	GPIOC->PDDR = ((GPIOC->PDDR) | (ELEC1_PORT_MASK));
}

static inline void ChargeRedistribution1 (void)
{
	// Configure Electrode and Cext pins as inputs at the same time
	GPIOC->PDDR = ((GPIOC->PDDR) & (~(ELEC1_PORT_MASK)));
}

static inline void ElectrodeFloat1 (void)
{
	// Configure Electrode and Cext pins as inputs at the same time
	GPIOC->PDDR = ((GPIOC->PDDR) & (~(ELEC1_PORT_MASK)));
}

#endif
