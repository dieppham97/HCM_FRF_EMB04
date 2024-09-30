#ifndef TOUCH1_H_
#define TOUCH1_H_

#include "S32K1xx.h"
#include "S32K1xx_gpio_driver.h"

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

void ChargeDistribution1 (void);
void ChargeRedistribution1 (void);
void ElectrodeFloat1 (void);
#endif
