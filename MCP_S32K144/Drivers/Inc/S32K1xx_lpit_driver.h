#ifndef _LPIT_H
#define _LPIT_H

#include "S32K1xx.h"

typedef struct
{
	uint8_t Source;
	uint8_t Mode;
	uint8_t Channel;
	uint8_t Interrupt;
	uint32_t ValueTime;
}LPIT_PinConfig_t;

/* Mode in PCS select source */
#define PCS_SOSCDIV2								1U
#define PCS_SIRCDIV2								2U
#define PCS_FIRCDIV2								3U
#define PCS_SPLLDIV2								6U
#define PCS_LPO128									7U

/* Source clock */
#define LPO128_CLK									128000U

/* Mode in TCR select mode*/
#define Periodic_Counter						0U
#define Dual_Periodic_16bit					1U
#define	Trigger_Accumulator					2U
#define Trigger_Input_Capture				3U

/* Channel */
#define CHANNEL0										0U
#define CHANNEL1										1U
#define CHANNEL2										2U
#define CHANNEL3										3U

void LPIT_PeriClockControl(uint8_t selectClock, uint8_t EnOrDi);
void ChannelClock(LPIT_PinConfig_t LPIT_PinConfig);
void LPIT_Init(LPIT_PinConfig_t LPIT_PinConfig);

void LPIT_IRQInterruptConfig (uint8_t IRQNumber, uint8_t EnOrDi);

uint32_t GetValueLPIT (void);

void LPIT_IRQHandling (uint8_t Channel);

#endif
