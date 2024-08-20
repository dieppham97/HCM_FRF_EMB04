#include "gpio.h"

static _vo uint8_t tick_count = 0;
static _vo uint8_t buttonState = 0;
static _vo uint8_t prevButtonState = 0;
static _vo uint8_t ledOn = 0;

void PORTC_IRQHandler(void);
void Init_SysTick(void);
void SysTick_Handler(void);

	
/* RGB */
int main (void)
{
	/* led red */
	GPIO_Handle_t GpioLed;
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GpioLed.GPIO_PinConfig.GPIO_PinAlterFunMode = GPIO;
	GpioLed.GPIO_PinConfig.GPIO_InOut = OUT_PUT;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPD_DE;
	
	initGPIO(&GpioLed);

	/* led blue */
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	initGPIO(&GpioLed);
	
	/* led green */
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_16;
	initGPIO(&GpioLed);
	
	/* button */
	GPIO_Handle_t Button;
	Button.pGPIOx = GPIOC;
	Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	Button.GPIO_PinConfig.GPIO_InOut = IN_PUT;
	Button.GPIO_PinConfig.GPIO_PinAlterFunMode = GPIO;
	Button.GPIO_PinConfig.GPIO_Interrupt = ENABLE;
	Button.GPIO_PinConfig.GPIO_ModeIT = IT_L1;
	
	initGPIO(&Button);
	
	
	GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_16, ENABLE);
	//GPIO_IRQPriorityConfig (IRQ_PORTC, 1);
	GPIO_IRQInterruptConfig (IRQ_PORTC, ENABLE);
	Init_SysTick();
	

	while (1);
}

void PORTC_IRQHandler(void)
{
	GPIO_IRQHandling (GPIOC, GPIO_PIN_NO_13);
	buttonState = ReadFromInput(GPIOC, GPIO_PIN_NO_13);
	if (buttonState && !prevButtonState)
	{
		ledOn = !ledOn;
	}
	prevButtonState = buttonState;
}

void Init_SysTick(void)
{
	/* Disable Systick during setup */
	SYSTICK->CSR = 0;
	
	/* Reload new value */
	SYSTICK->RVR = ((SYSTICK_TIMER_CLK / 10) - 1);
	
	/* Set SysTick VAL = 0*/
	SYSTICK->CVR = 0;
	
	/* Enable SysTick */
	SYSTICK->CSR |= (1 << SYSTICK_CSR_EN);
	SYSTICK->CSR |= (1 << SYSTICK_CSR_TICKINT);
	SYSTICK->CSR |= (1 << SYSTICK_CSR_CLKSOURCE);
}

/*
******************************************************************************
Function name   :   SysTick_Handler
Description     :   Implemt ISR for SysTick
******************************************************************************
*/
void SysTick_Handler(void) 
{
	static uint8_t led_state = 0;
	if (ledOn) 
	{
		tick_count++;
		if (tick_count >= 5)
		{
			tick_count = 0;
			led_state++;
			switch (led_state)
			{
				case 1:
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_16, ENABLE);
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, DISABLE);
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_0, ENABLE);
					break;
				case 2:
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_16, ENABLE);
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, ENABLE);
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_0, DISABLE);
					break;
				case 3:
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_16, DISABLE);
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, ENABLE);
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_0, ENABLE);
					break;
				default:
					led_state = 0;
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_16, ENABLE);
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, ENABLE);
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_0, ENABLE);
				break;
			}
		}
	} else {
		GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_16, ENABLE);
		GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, ENABLE);
		GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_0, ENABLE);
    }
}

