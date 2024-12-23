/*
 * test_HC_SR04.c
 *
 *  Created on: Nov 4, 2024
 *      Author: diepp
 */


#include <stdint.h>

#define TIM2	0x40000000U
#define TIM4	0x40000800U
#define GPIOA	0x40020000U
#define RCC		0x40023800U
#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100)

uint32_t pw = 0;	//pulse width
uint32_t lc = 0;	//last capture counter value
uint32_t cc = 0;	//current captured counter value
uint32_t sp = 0;	//signal polarity
double time, dist;

void Init_GPIO (void)
{
	/* Enable clock GPIOA */
	*(uint32_t*)(RCC + 0x30)	|= (1 << 0);

	/* Configure PA0 in alternate function */
	*(uint32_t*)(GPIOA)			|= (2 << 0);
	*(uint32_t*)(GPIOA + 0x20)	|= (1 << 0);


	/* Configure PA5 in output mode */
	*(uint32_t*)(GPIOA)			|= (1 << 10) | (1 << 2);		/* Set mode for PA2 - Output */
//	*(uint32_t*)(GPIOA + 0x08)	|= (2 << 4);		/* Set high speed PA2 */
}

void TIM2_Capture (void)
{
	/* 1. Enable Clock Timer 2*/
	*(uint32_t*)(RCC + 0x40)	|= (1 << 0);

	/* 2. Set CC1S as 01 to map IC1 to TI1 and configure CC1 as input */
	*(uint32_t*)(TIM2 + 0x18) 	= (1 << 0);

	/* 3. Disable sampling, since we need every event capture */
	*(uint32_t*)(TIM2 + 0x18) 	&= ~(0xF << 4);

	/* 4. Set the polarity of the signal for the triggering capture */
	// CC2NP/CC2P is set as 11, for sensitivity to both rising and falling edge
	*(uint32_t*)(TIM2 + 0x20) 	|= (1 << 1) | (1 << 3);

	/* 5. Disable the input pre-scaler to capture at each valid transition */
	//Writing 00 to IC2PS bits
	*(uint32_t*)(TIM2 + 0x18) 	&= ~(3 << 2);

	/* 6. Enable the capture from the counter into Capture register */
	*(uint32_t*)(TIM2 + 0x20) 	|= (1 << 1);

	/* 7. Enable the interrupt */
	*(uint32_t*)(TIM2 + 0x0C) 	|= (1 << 1);

	/* 8. Enable the counter */
	*(uint32_t*)(TIM2)			|= 1;
}

void NVIC_Init (void)
{
	*NVIC_ISER0 |= (1 << 28);
}

void TIM2_IRQHandler(void)
{
	if ((*(uint32_t*)(TIM2 + 0x10) & 0x2) != 0)
	{
		cc = *(uint32_t*)(TIM2 + 0x34);		//read the capture value
		sp = 1 - sp;						//toggle the polarity flag

		if (sp == 0)						//Calculate only when signal is low
		{
			pw = cc - lc;					//Calculate the pulse-width
		}

		lc = cc;							//update the last captured value

		if((*(uint32_t*)(TIM2 + 0x10) & 0x1) != 0)	//Check if overflow has taken place
		{
			*(uint32_t*)(TIM2 + 0x10) &= ~(1 << 0);	//Clear UIF flag to prevent re-entering
		}
	}
}

void TIM4_us_delay (uint32_t value)
{
	*(uint32_t*)(RCC + 0x40) |= (1 << 2);	//Start the clock for the timer peripheral

	/* ARR */
	*(uint32_t*)(TIM4 + 0x2C) = (uint32_t)(value / 0.0625);	//Total period of the timer

	/* CNT */
	*(uint32_t*)(TIM4 + 0x24) = 0;

	/* Start the Timer */
	*(uint32_t*)(TIM4) |= 1;
	while(!(*(uint32_t*)(TIM4 + 0x10) & 0x1));
	*(uint32_t*)(TIM4 + 0x10) &= ~(0x1);
}

int main ()
{
	Init_GPIO();
	TIM2_Capture();
	NVIC_Init();
	while (1)
	{
		/* 1. Sending 10us pluse to */
		*(uint32_t*)(GPIOA + 0x18)	&= 0x00000000;		/* PA5 is low */
		TIM4_us_delay(2);
		*(uint32_t*)(GPIOA + 0x18)	|= 0x00000020;		/* PA5 is high */
		TIM4_us_delay(10);
		*(uint32_t*)(GPIOA + 0x18)	|= 0x00200000;		/* PA5 is low */

		/* 2. Converting the gathered data into distance in cm */
		if (pw > 0)
		{
			time = pw *(0.0625 * 0.000001);
			dist = ((time * 340)/2) * 100;
		}
		if (dist != 0)
		{
			*(uint32_t*)(GPIOA + 0x18)	|= (1 << 1);		/* PA1 is high */
		}else
		{
			*(uint32_t*)(GPIOA + 0x18)	|= (1 << 17);		/* PA1 is low */
		}
	}
}

