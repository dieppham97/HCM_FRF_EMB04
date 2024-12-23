
#include "stm32f407xx.h"

#ifndef RO
    #define RO const
    #define RW
	#define WO const
    #define IO volatile
    #define DM
#endif  //! #ifndef RO
static volatile uint32_t delay_counter;

#define FCORE_MHZ 8											/*Chu dong dien vao nha*/
#define CLOCK_FCORE (FCORE_MHZ * 1000000) /* STM32F1: */
#define RELOAD_Time (0.001)								/* 1000 times reload */
#define RELOAD_VALUE (CLOCK_FCORE * RELOAD_Time)

typedef union{
	uint32_t REG;
	struct {
		RW uint32_t 	ENA		: 		1;
		RW uint32_t 	TICKINT		: 		1;
		RW uint32_t 	CLKSOURCE	:			1;
		RO uint32_t 	RES01			: 		13;
		RW uint32_t 	COUNTLAG	: 		1;
	  RO uint32_t 	RES02			:			15;
	} FIELD; 				//SysTick Control and Status Register, SYST_CSR, ARMv7-M Arch... page 621
} SYST_CSR_REG_t;
/*--------------------------------------------------------*/

typedef union{
	uint32_t REG;
	struct {
		RW uint32_t 	RELOAD		: 			24;
		RO uint32_t 	RES01			: 			8;
	} FIELD; 				//SysTick Reload Value Register, SYST_RVR on page B3-622
} SYST_RVR_REG_t;
/*--------------------------------------------------------*/

typedef union{
	uint32_t REG;
	struct {
		RW uint32_t 	CURRENT		: 			32;
	} FIELD; 				//SysTick Current Value Register, SYST_CVR on page B3-622
} SYST_CVR_REG_t;
/*--------------------------------------------------------*/

typedef union{
	uint32_t REG;
	struct {
		RW 	uint32_t 	TENMS			: 		24;
		RO	uint32_t 	RES01			: 		6;
		RW 	uint32_t 	SKEW			: 		1;
		RW 	uint32_t 	NOREF			: 		1;
	} FIELD; 				//SysTick Calibration value Register, SYST_CALIB on page B3-623
} SYST_CALIB_REG_t;
/*--------------------------------------------------------*/

typedef struct {
	IO RW 	SYST_CSR_REG_t			SYST_CSR;		//SysTick Control and Status Register
	IO RW 	SYST_RVR_REG_t			SYST_RVR;		//SysTick Reload Value Register
	IO RW 	SYST_CVR_REG_t			SYST_CVR;		//SysTick Current Value Register
	IO RO 	SYST_CALIB_REG_t		SYST_CALIB;	//SysTick Calibration value Register
}SYSTICK_t;

#define SYSTICK_BASE 					(0xE000E010)
#define SYSTICK							((SYSTICK_t *) SYSTICK_BASE)


void delay_ms(unsigned int ms_second) // 1s
{
	SYSTICK->SYST_CSR.FIELD.ENA = 1; /*BIT_SET = 0x1u */
	SYSTICK->SYST_CSR.FIELD.TICKINT = 1;
//	SYSTICK->SYST_CSR.FIELD.CLKSOURCE = 1u;
	SYSTICK->SYST_RVR.FIELD.RELOAD = ((uint32_t)(RELOAD_VALUE - 1)); // 0.002s. reload 72x10^3/3610^6
	delay_counter = ms_second;
	while (delay_counter > 0)
	{
	}
	SYSTICK->SYST_CSR.FIELD.ENA = 0;
}
void SysTick_Handler(void)
{
	if (delay_counter > 0)
	{
		delay_counter--;
	}
	if (delay_counter == 1)
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}

int main(void)
{
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;

	GPIO_PeriClockControl(GPIOD, ENABLE);

	GPIO_Init(&GpioLed);

	while(1)
	{
		delay_ms(1000);
	}
	return 0;
}
