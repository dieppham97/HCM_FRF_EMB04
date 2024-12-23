
#include "stm32f407xx.h"

#define HIGH 1
#define LOW 0
#define BTN_PRESS LOW
#include <string.h>

void delay(void)
{
	for (uint32_t i = 0; i < 500000/2; i++);
}

int main(void)
{

	GPIO_Handle_t GpioLed, GPIOBtn;
	memset(&GpioLed,0,sizeof(GpioLed));
	memset(&GPIOBtn,0,sizeof(GPIOBtn));

	//this is led gpio configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD, ENABLE);

	GPIO_Init(&GpioLed);

	//this is button gpio configuration
	GPIOBtn.pGPIOx = GPIOD;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOD, ENABLE);

	GPIO_Init(&GPIOBtn);

	GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_NO_12, GPIO_PIN_RESET);

	//IRQ configurations
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRI15);	//EXTI9_5: 23, 23/4=5 -> check IPR5
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);	//enable the interrupt

	while(1);

	return 0;
}

void EXTI9_5_IRQHandler(void)
{
	delay();	//200ms
	GPIO_IRQHandling(GPIO_PIN_NO_5);	//xu ly ngat tai chan 5/ clear the pending event from exti line
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}
