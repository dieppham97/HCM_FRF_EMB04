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

static char msg[1024] = "h";

static UART_Handle_t Uart1_Handle;

void Clock_Init (void);
void USART1_Init(void);
void USART1_GPIOInit(void);
void GPIO_ButtonInit(void);
	
int main (void)
{
	Clock_Init();
	
	USART1_GPIOInit();
	
	USART1_Init();

	while(1)
	{
		//while (!ReadFromInput(GPIOC, GPIO_PIN_NO_13));
		UART_SendData(&Uart1_Handle, (uint8_t*)msg, strlen(msg));
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
	GPIO_Handle_t Button;
	Button.pGPIOx = GPIOC;
	Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	Button.GPIO_PinConfig.GPIO_InOut = IN_PUT;
	Button.GPIO_PinConfig.GPIO_PinAlterFunMode = GPIO;
	
	GPIO_Init(&Button);
}
