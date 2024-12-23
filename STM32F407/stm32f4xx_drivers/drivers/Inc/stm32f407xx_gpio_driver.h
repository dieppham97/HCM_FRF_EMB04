/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Mar 17, 2024
 *      Author: diepp
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

typedef struct
{
	uint8_t GPIO_PinNumber;				/*!< Possible values from @GPIO_PIN_NUMBERS>*/
	uint8_t GPIO_PinMode;				/*!< Possible values from @GPIO_PIN_MODES >*/
	uint8_t GPIO_PinSpeed;				/*!< Possible values from @GPIO_PIN_SPEED >*/
	uint8_t GPIO_PinPuPdControl;		/*!< Possible values from @GPIO_PIN_PUPD  >*/
	uint8_t GPIO_PinOPType;				/*!< Possible values from @GPIO_PIN_OutPut >*/
	uint8_t GPIO_PinAltFunMode;			//this pin be determined by Pin Mode
}GPIO_PinConfig_t;

/*
 *
 * This is a Handle structure for a GPIIO pin
 */

typedef struct
{
	//pointer to hold the base address of the GPIO peripheral
	GPIO_RegDef_t *pGPIOx;				/*!< This holds the base address of the GPIO port to which the pin belongs >*/
	GPIO_PinConfig_t GPIO_PinConfig;	/*!< This holds GPIO pin configuration settings >*/

}GPIO_Handle_t;


/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin possible numbers
 */
#define GPIO_PIN_NO_0		0
#define GPIO_PIN_NO_1		1
#define GPIO_PIN_NO_2		2
#define GPIO_PIN_NO_3		3
#define GPIO_PIN_NO_4		4
#define GPIO_PIN_NO_5		5
#define GPIO_PIN_NO_6		6
#define GPIO_PIN_NO_7		7
#define GPIO_PIN_NO_8		8
#define GPIO_PIN_NO_9		9
#define GPIO_PIN_NO_10		10
#define GPIO_PIN_NO_11		11
#define GPIO_PIN_NO_12		12
#define GPIO_PIN_NO_13		13
#define GPIO_PIN_NO_14		14
#define GPIO_PIN_NO_15		15


/*
 * @GPIO_PIN_MODES
 * GPIO pin possible modes
 */
#define GPIO_MODE_IN		0			//00		non-interrupt
#define GPIO_MODE_OUT		1			//01		non-interrupt
#define GPIO_MODE_ALTFN		2			//10		non-interrupt
#define GPIO_MODE_ANALOG	3			//11		non-interrupt
#define GPIO_MODE_IT_FT		4			//IT stands for input falling edge, falling se nhan tin hieu vi tu 1 ->0	interrupt
#define GPIO_MODE_IT_RT		5			//			interrupt
#define GPIO_MODE_IT_RFT	6			//			interrupt


/*
 * @GPIO_PIN_OutPut
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP		0			//Push-Pull
#define GPIO_OP_TYPE_OD		1			//Open-drain


/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible output speeds
 */
#define GPIO_SPEED_LOW		0
#define GPIO_SPEED_MEDIUM	1
#define GPIO_SPEED_FAST		2
#define GPIO_SPEED_HIGH		3

/*
 * @GPIO_PIN_PUPD
 * GPIO pin pull up and pull down configuration macros
 */
#define GPIO_NO_PUPD		0
#define GPIO_PIN_PU			1
#define GPIO_PIN_PD			2


/*******************************************************************************************
 * 								APIs supported by this driver
 * 			For more information about the APIs check the function definitions
 ******************************************************************************************/

/*
 * Peripheral Clock setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);	//chọn thành phần cần cấu hình bật hoặc tắt

/*
 * Init and De-init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);		//mean sending that register back to its reset state ỏ reset value


/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);		//need 16 pin so use uint16_t
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);		//value is 1 or 0
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and ISR handling (whenever interrupt occurs so they user application can call this function in order to process that interrupt
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQPriorrity, uint32_t IRQNumber);
void GPIO_IRQHandling(uint8_t PinNumber);






#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
