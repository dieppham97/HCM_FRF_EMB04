#include "S32K1xx.h"
#include <stdio.h>
#include <string.h>

/*
*	UART0_RX: PTA2, PTB0, PTC2
* UART0_TX: PTA3, PTB1, PTC3
* ALT6: PTA2, PTA3
* ALT2: PTB1, PTB0
* ALT4: PTC2, PTC3
* CLOCK SOURCE: SOSCDIV2, SIRCDIV2, FIRCDIV2, SPLLDIV2
*/

static char msg[1024] = "asdfdsasfadsaf";
static char rx_buf[1024];

static UART_Handle_t Uart1_Handle;

void Clock_Init (void);
void USART1_Init(void);
void USART1_GPIOInit(void);
void GPIO_ButtonInit(void);
void LPUART1_RxTx_IRQHandler (void);
	
int main (void)
{
	UART_IRQInterruptConfig(33, ENABLE);
	Clock_Init();
	
	USART1_GPIOInit();
	
	USART1_Init();
	GPIO_ButtonInit();

	while(1)
	{
		//while (!ReadFromInput(GPIOC, GPIO_PIN_NO_13));
		
		//UART_SendData(&Uart1_Handle, (uint8_t*)msg, strlen(msg));
	}
	
}

void Clock_Init (void)
{
	SourceClock_Config_t Uart1_Clock;
	Uart1_Clock.DIV = DIV2;
	Uart1_Clock.Soure = Source_FIRC;
	Uart1_Clock.FIRC_Config.FIRC_DIV2 = DIV_1;
	
	SCG_Init(Uart1_Clock);
}

void USART1_Init(void)
{
	Uart1_Handle.pUARTx = UART1;
	Uart1_Handle.USART_Config.UART_ClockSource = Source_FIRC;
	Uart1_Handle.USART_Config.UART_MODE = UART_MODE_TXRX;
	Uart1_Handle.USART_Config.UART_BAUD = UART_BAUD_9600;
	Uart1_Handle.USART_Config.UART_StopBit = UART_STOPBITS_1;
	Uart1_Handle.USART_Config.UART_ParityBit = UART_PARITY_DISABLE;
	Uart1_Handle.USART_Config.UART_WordLength = UART_WORDLEN_8BITS;
	Uart1_Handle.USART_Config.UART_OverSampling = UART_OSR16;
	Uart1_Handle.USART_Config.UART_ITMODE = UART_ITMODE_RX;
	
	UART_Init(&Uart1_Handle);
}

void USART1_GPIOInit(void)
{
	GPIO_Handle_t Uart1_Gpio;
	
	Uart1_Gpio.pGPIOx = GPIOC;
	Uart1_Gpio.GPIO_PinConfig.GPIO_PinAlterFunMode = 2;
	Uart1_Gpio.GPIO_PinConfig.GPIO_InOut = OUT_PUT;
	Uart1_Gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPD_DE;
	Uart1_Gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	Uart1_Gpio.GPIO_PinConfig.GPIO_Interrupt = DISABLE;
	
	/* UART0_TX */
	GPIO_Init(&Uart1_Gpio);
	
	/* UART0_RX */
	Uart1_Gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&Uart1_Gpio);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GpioLed;
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GpioLed.GPIO_PinConfig.GPIO_PinAlterFunMode = GPIO;
	GpioLed.GPIO_PinConfig.GPIO_InOut = OUT_PUT;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPD_DE;
	GPIO_Init(&GpioLed);
}

void LPUART1_RxTx_IRQHandler (void)
{
	UART_ReceiveData(&Uart1_Handle, (uint8_t*)rx_buf, 1);
	UART_IRQHandling(&Uart1_Handle);
}

void USART_ApplicationEventCallback(UART_Handle_t *pUSARTHandle, uint8_t event)
{
	uint8_t led = rx_buf[0];
	if(event == UART_ITMODE_RX)
   {
			if (led == '1')
		{
			GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, DISABLE);
		}else if (led == '0')
		{
			GPIO_WriteToOutput(GPIOD, GPIO_PIN_NO_15, ENABLE);
		}

   }
	
}
