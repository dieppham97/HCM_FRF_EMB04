#include "S32K1xx.h"

static _vo uint8_t hours 	 =	0;
static _vo uint8_t minutes =	0;
static _vo uint8_t seconds = 	0;

void LPIT0_Ch2_IRQHandler (void);
void LPIT0_Ch3_IRQHandler(void);

int main (void)
{
	/*
	GPIO_Handle_t GpioLed;
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber 			= GPIO_PIN_NO_15;
	GpioLed.GPIO_PinConfig.GPIO_PinAlterFunMode = GPIO;
	GpioLed.GPIO_PinConfig.GPIO_InOut 					= OUT_PUT;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl 	= GPIO_PUPD_DE;
	
	initGPIO(&GpioLed);*/
	
		/* led blue */
//	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
//	initGPIO(&GpioLed);
	
	/* led green */
//	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_16;
//	initGPIO(&GpioLed);
	
	/*
	LPIT_PinConfig_t TC2;
	TC2.Source 		= PCS_LPO128;
	TC2.Mode 			= Periodic_Counter;
	TC2.Interrupt = ENABLE;
	TC2.Channel		= CHANNEL2;
	TC2.ValueTime = 500;
	
	LPIT_IRQInterruptConfig(LPIT_IT2, ENABLE);
	LPIT_PeriClockControl(TC2.Source, ENABLE);
	LPIT_Init(TC2);*/
	

	
	LPIT_PinConfig_t TC3;
	TC3.Source = PCS_LPO128;
	TC3.Mode = Periodic_Counter;
	TC3.Interrupt = ENABLE;
	TC3.Channel = CHANNEL3;
	TC3.ValueTime = 1000;
	
	LPIT_IRQInterruptConfig(LPIT_IT3, ENABLE);
	LPIT_PeriClockControl(TC3.Source, ENABLE);
	LPIT_Init(TC3);
	
	
	
	while(1);
	
}

void LPIT0_Ch2_IRQHandler (void)
{
	LPIT_IRQHandling(CHANNEL2);
	GPIO_TogglePin(GPIOD, GPIO_PIN_NO_15);
}


void LPIT0_Ch3_IRQHandler(void)
{
	LPIT_IRQHandling(CHANNEL3);
	
	seconds++;
	if(seconds >= 60)
	{
		//GPIO_TogglePin(GPIOD, GPIO_PIN_NO_16);
		seconds = 0;
		minutes++;
		if(minutes >= 60)
		{
			//GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_0, DISABLE);
			minutes = 0;
			hours++;
			if(hours >= 24)
			{
				//GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, DISABLE);
				hours = 0;
			}
		}
	}
	
}
