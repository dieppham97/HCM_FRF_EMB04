#include "gpio.h"

void delay (void);
void PORTC_IRQHandler(void);

int main (void)
{
	GPIO_Handle_t GpioLed;
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GpioLed.GPIO_PinConfig.GPIO_PinAlterFunMode = GPIO;
	GpioLed.GPIO_PinConfig.GPIO_InOut = OUT_PUT;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPD_DE;
	
	initGPIO(&GpioLed);
	GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, ENABLE);
	GPIO_Handle_t Button;
	Button.pGPIOx = GPIOC;
	Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	Button.GPIO_PinConfig.GPIO_InOut = IN_PUT;
	Button.GPIO_PinConfig.GPIO_PinAlterFunMode = GPIO;
	
	initGPIO(&Button);
	
	uint8_t ledState = 0;
	uint8_t buttonState = 0;
	uint8_t prevButtonState = 0;

	while (1)
		{
			
			buttonState = ReadFromInput(GPIOC, GPIO_PIN_NO_13);
			if (buttonState == 1 && prevButtonState == 0)
				{
					ledState = !ledState;
				}
			if (ledState == 1)
        {
					GPIO_TogglePin(GPIOD, GPIO_PIN_NO_15);
					delay();
        }else
        {
					GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, DISABLE);
        }
				prevButtonState = buttonState;
    }
}

void delay (void)
{
	for(uint32_t i = 0; i < 500000; i++);
}

void PORTC_IRQHandler(void)
{
	
}