/*
 * CAN_Node1.c
 *
 *  Created on: Oct 28, 2024
 *      Author: diepp
 */
#include <stdint.h>
#include "can.h"

void TIME4_delay_ms(uint16_t delay)
{
	*(uint32_t *)(RCC_BASEADDR + 0x40) |= (1 << 2); /* Enable TIME 4 */
	TIM4->PSC = (16000 - 1);
	TIM4->ARR = (delay);
	TIM4->CNT = 0;
	TIM4->CR1 |= (1 << 0);
}

void Init_GPIOA(void)
{
	/* Enable GPIOA */
	*(uint32_t *)(RCC_BASEADDR + 0x30) |= (1 << 0);

	/* Configure GPIOA */
	GPIOA->MODER |= (1 << 8); /* Select PA4 output */
}

void Init_CANPin(void)
{
	*(uint32_t *)(RCC_BASEADDR + 0x30) |= (1 << 3); /* Enable GPIOD */
	GPIO_RegDef_t *CANPin;
	CANPin = GPIOD;
	CANPin->MODER |= (2 << 0) | (2 << 2);	 /* Set mode of pin */
	CANPin->AFR[0] |= (9 << 0) | (9 << 4); /* Set alternate 9 */
}

void Init_CAN(void)
{
	*(uint32_t *)(RCC_BASEADDR + 0x40) |= (1 << 25); /* Enable CAN1 */

	/* 1. Setting baudrate and configuring CAN1 */
	/* Reset CAN */
	CAN1->MCR |= (1 << 15); 						/* Set bit RESET in MCR */

	/* While RESET complete*/
	while ((CAN1->MCR >> 15) & 0x1);

	/* Set mode initialize */
	CAN1->MCR |= (1 << 0);							/* Set INRQ bit in MCR to request initialize */

	/* Wait Initialize complete INAK_MSR */
	while (!(CAN1->MSR & 0x1));

	/* Exit mode sleep */
	CAN1->MCR &= ~(1 << 1); 						/* Clear bit SLEEP in MCR */

	/* Check leave SLEEP mode */
	while ((CAN1->MSR & (1 << 1)) != 0);

	/* Disable Auto retransmission */
	//CAN1->MCR |= (1 << 4);

	/* Set bit timing */
	CAN1->BTR = 0x001c0001; 						/*	Bit rate : 					500 kbit/s
													 *	BRP (baudrate prescale): 	2
													 *	Time Segment 1:				13
													 *	Time Segment 2:				2
													 */

	/* 2.Configuring the Filter */
	/* Set mode initialize filter */
	CAN1->FMR |= (1 << 0); 							/* Set bit FINIT in FMR */

	/* Disable all filters */
	CAN1->FA1R = 0; 								/* Disable all filter */

	/* Select scale for channel filter */
	CAN1->FS1R |= (1 << 0); 						/* Select Filter 0 */

	/* Set mode filter (mask or list) */
	CAN1->FM1R |= (1 << 0); 						/* Select List mode*/

	/* Set filter bank */
	CAN1->FxRi[0].FR1 = (0x111 << 21);
	CAN1->FxRi[0].FR2 = (0x222 << 21);

	/* Select FIFO in FFAR1 */
	CAN1->FFA1R = 0; 								/* Select FIFO 0 */

	/* Enable filter */
	CAN1->FA1R |= (1 << 0); 						/* Active Filter 0 */

	/* Exit mode initialize filter */
	CAN1->FMR &= ~(1 << 0);

	/* 3. Setting Transmission */
	// Setting the Std. ID
	CAN1->TI0R = (0x111 << 21);

	// Setting Datalength
	CAN1->TDT0R = 1;

	/* Exit mode Initialize */
	CAN1->MCR &= ~(1 << 0);

	/* Wait to exit intialize */
	while ((CAN1->MSR & 0x1) != 0);
}

void CAN_Tx_DATA(uint8_t msg)
{
	CAN1->TDL0R = msg;

	// Request for transmission
	CAN1->TI0R |= (1 << 0); /* Set bit TXRQ to sent */
}

uint8_t CAN_Rx(void)
{
	/* Check pending FIFO has selected in Filter */
	while (!(*(uint32_t *)(CAN1_BASEADDR + 0x0C) & 0x3))
		; /* Check FIFO0 pending */

	// Read the data from the FIFO0 mailbox from Mailbox Data low register
	uint8_t RxD = (*(uint32_t *)(CAN1_BASEADDR + 0x1B8) & 0xFF);

	/* Release FIFO */
	*(uint32_t *)(CAN1_BASEADDR + 0x0C) |= (1 << 5);

	return RxD;
}

int main(void)
{
	Init_CANPin();
	Init_CAN();
	TIME4_delay_ms(1000);
	Init_GPIOA();
	uint8_t k = 0;
	while (1)
	{
		CAN_Tx_DATA(k);
		k += 1;
		if (k > 4)
			k = 0;
		while (!(TIM4->SR & 0x1));
		TIM4->SR &= ~(1 << 0);
		GPIOA->ODR ^= (1 << 4);
	}
}
