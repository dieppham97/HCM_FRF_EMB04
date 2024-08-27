#ifndef UART_H_
#define UART_H_

#include "S32K1xx.h"

extern uint32_t SOSCDIV2;
extern uint32_t SIRCDIV2;
extern uint32_t FIRCDIV2;
extern uint32_t SPLLDIV2;


/*
*	UART0_RX: PTA2, PTB0, PTC2
* UART0_TX: PTA3, PTB1, PTC3
*/

typedef struct
{
	uint8_t		UART_ClockSource;
	uint8_t		UART_MODE;
	uint32_t	UART_BAUD;
	uint8_t		UART_StopBit;
	uint8_t		UART_ParityBit;
	uint8_t		UART_WordLength;
	uint8_t 	UART_OverSampling;
	uint8_t		UART_ITMODE;
}UART_Config_t;

typedef struct
{
	UART_RegDef_t *pUARTx;
	UART_Config_t	 USART_Config;
}UART_Handle_t;

/*
 *@USART_Mode
 *Possible options for USART_Mode				(TE bit and RE bit in CTRL)
 */	
#define UART_MODE_ONLY_TX					0
#define UART_MODE_ONLY_RX 				1
#define UART_MODE_TXRX 						2


/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define UART_BAUD_1200						1200
#define UART_BAUD_2400						2400
#define UART_BAUD_9600						9600
#define UART_BAUD_19200 					19200
#define UART_BAUD_38400 					38400
#define UART_BAUD_57600 					57600
#define UART_BAUD_115200 					115200
#define UART_BAUD_230400 					230400
#define UART_BAUD_460800 					460800
#define UART_BAUD_921600 					921600
#define UART_BAUD_2M 							2000000
#define UART_BAUD_3M 							3000000

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits		(Stop bit in baudrate)
 */
#define UART_STOPBITS_1	  				0
#define UART_STOPBITS_2	  				1

/*
 *@USART_WordLength
 *Possible options for USART_WordLength			(M bit in CTRL)
 */
#define UART_WORDLEN_8BITS 				0
#define UART_WORDLEN_9BITS  			1

/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl	(PF,PE in CTRL)
 */
#define UART_PARITY_EN_ODD   			2
#define UART_PARITY_EN_EVEN  			1
#define UART_PARITY_DISABLE   		0

/*
 *@USART_OverSampling
 *Possible options for USART_OverSampling
 */
#define UART_OSR4   							3
#define UART_OSR5  								4
#define UART_OSR6   							5
#define UART_OSR7   							6
#define UART_OSR8   							7
#define UART_OSR9   							8
#define UART_OSR10  							9
#define UART_OSR11  							10
#define UART_OSR12  							11
#define UART_OSR13  							12
#define UART_OSR14 								13
#define UART_OSR15  							14
#define UART_OSR16  							15
#define UART_OSR17  							16
#define UART_OSR18  							17
#define UART_OSR19  							18
#define UART_OSR20  							19
#define UART_OSR21  							20
#define UART_OSR22  							21
#define UART_OSR23  							22
#define UART_OSR24  							23
#define UART_OSR25  							24
#define UART_OSR26  							25
#define UART_OSR27  							26
#define UART_OSR28  							27
#define UART_OSR29								28
#define UART_OSR30  							29
#define UART_OSR31  							30
#define UART_OSR32  							31

/*
 *@USART_ITMODE
 *Possible options for interrupt mode
 */
#define UART_ITMODE_TC						1
#define UART_ITMODE_RX						2

/*
 * Application states
 */
#define UART_BUSY_IN_RX 					1
#define UART_BUSY_IN_TX 					2
#define UART_READY 								0




void UART_PeriClockControl(UART_Handle_t *pUARTHandl, uint8_t EnOrDi);
void UART_SetBaudRate(UART_Handle_t *pUARTHandl);

void UART_Init(UART_Handle_t *pUARTHandl);
void UART_Deinit(void);

void UART_SendData(UART_Handle_t *pUARTHandl, uint8_t *pTxBuffer, uint32_t Len);
void UART_ReceiveData(UART_Handle_t *pUARTHandl, uint8_t *pTxBuffer, uint32_t Len);
uint8_t UART_SendDataIT(UART_Handle_t *pUARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t UART_ReceiveDataIT(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len);

void UART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void UART_IRQHandling(UART_Handle_t *pUSARTHandle);

void USART_ApplicationEventCallback(UART_Handle_t *pUSARTHandle,uint8_t event);

#endif
