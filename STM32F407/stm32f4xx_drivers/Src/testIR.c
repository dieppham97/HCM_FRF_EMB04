/*
 * testIR.c
 *
 *  Created on: Nov 6, 2024
 *      Author: diepp
 */

#include <stdint.h>

#define TIM2	0x40000000U
#define GPIOA	0x40020000U
#define EXTI	0x40013C00U
#define RCC		0x40023800U
#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100)


void Init_GPIO (void)
{
	/* Enable clock GPIOA */
	*(uint32_t*)(RCC + 0x30)	|= (1 << 0);

	/* Configure PA1 in output mode */
	*(uint32_t*)(GPIOA)			|= (1 << 2);		/* Set mode for PA2 - Output */

	/* PA0 input from sensor */
}

//void TIM2_ms_delay (uint32_t delay)
//{
//	/* 1. Enable Clock Timer 2*/
//	*(uint32_t*)(RCC + 0x40)	|= (1 << 0);
//
//	*(uint32_t*)(TIM2 + 0x28) = 16000-1;	//Set the clock frequency to 1KHz
//	*(uint32_t*)(TIM2 + 0x2C) = delay;		//Get required delay from user
//
//	/* CNT */
//	*(uint32_t*)(TIM2 + 0x24) = 0;
//
//	/* Start the Timer */
//	*(uint32_t*)(TIM2) |= 1;
//	while(!(*(uint32_t*)(TIM2 + 0x10) & 0x1));
//	*(uint32_t*)(TIM2 + 0x10) &= ~(0x1);
//}

void EXTI0_Init (void)
{
	*(uint32_t*)(EXTI) |= (1 << 0);
	*(uint32_t*)(EXTI + 0x0C) |=  1;
}

void NVIC_Init (void)
{
	*NVIC_ISER0 |= (1 << 6);
}

void EXTI0_IRQHandler (void)
{
	*(uint32_t*)(GPIOA + 0x14) ^= (1 << 1);
	*(uint32_t*)(EXTI + 0x14) |=  1;
}

int main()
{
	Init_GPIO();
	EXTI0_Init();
	NVIC_Init();
	while(1){
	}
}
