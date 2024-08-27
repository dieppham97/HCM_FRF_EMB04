#ifndef _GPIO_H
#define _GPIO_H

#include "S32K1xx.h"

typedef struct 
{
	uint8_t GPIO_PinAlterFunMode;
	uint8_t GPIO_InOut;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinPuPdType;
	uint8_t GPIO_PinNumber;
	uint8_t	GPIO_Interrupt;
	uint8_t	GPIO_ModeIT;
}GPIO_PinConfig_t;

/*
*Pins of GPIOx
*/
#define GPIO_PIN_NO_0			0
#define GPIO_PIN_NO_1			1
#define GPIO_PIN_NO_2			2
#define GPIO_PIN_NO_3			3
#define GPIO_PIN_NO_4			4
#define GPIO_PIN_NO_5			5
#define GPIO_PIN_NO_6			6
#define GPIO_PIN_NO_7			7
#define GPIO_PIN_NO_8			8
#define GPIO_PIN_NO_9			9
#define GPIO_PIN_NO_10		10
#define GPIO_PIN_NO_11		11
#define GPIO_PIN_NO_12		12
#define GPIO_PIN_NO_13		13
#define GPIO_PIN_NO_14		14
#define GPIO_PIN_NO_15		15
#define GPIO_PIN_NO_16		16
#define GPIO_PIN_NO_17		17

/*
*AlterFunMode (PCRx.MUX)
*/
#define GPIO							1

/*
*InOut put
*/
#define IN_PUT						0
#define OUT_PUT						1

/*
*enable/ disable mode pullup pull down
*/
#define GPIO_PUPD_DE			0
#define GPIO_PUPD_EN			1

/*
*choice pullup-pull down
*/
#define GPIO_PD						0
#define GPIO_PU						1

/*
*Modes interrupt
*/
#define IT_L0							8
#define IT_RE							9
#define IT_FE							10
#define IT_RFE						11
#define IT_L1							12

typedef struct
{
	GPIO_RegDef_t 		*pGPIOx;
	GPIO_PinConfig_t	GPIO_PinConfig;
}GPIO_Handle_t;

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);
void SetPinGPIO (GPIO_RegDef_t *pGPIOx, uint8_t mode, uint8_t pinNumber);
void DisPUPD (GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);
void setModeIT (GPIO_RegDef_t *pGPIOx, uint8_t modeIT, uint8_t pinNumber);
void GPIO_Init (GPIO_Handle_t *pGPIOHandle);

/*
 * Data read and write
 */
void GPIO_WriteToOutput (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber,uint8_t EnOrDe);
void GPIO_TogglePin (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint8_t ReadFromInput (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and ISR handling (whenever interrupt occurs so they user application can call this function in order to process that interrupt
 */
void GPIO_IRQInterruptConfig (uint8_t IRQNumber, uint8_t EnOrDi);
void GPIO_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

uint8_t checkLed (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

#endif
