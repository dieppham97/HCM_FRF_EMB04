

/*
 * Timer 2
 * PA0 - CH1 - AF1
 */

#include <stdint.h>

#define AHB1PERIPH_BASEADDR   0x40020000U
#define APB1PERIPH_BASEADDR   0x40000000U
#define GPIOA_BASEADDR        (AHB1PERIPH_BASEADDR)  // GPIOA
#define TIM2_BASEADDR         (APB1PERIPH_BASEADDR)  // Timer 2
#define RCC_BASEADDR          (0x40023800U)  // RCC

void Init_GPIO_TIMER(void)
{
    *(uint32_t*)(RCC_BASEADDR + 0x30) |= (1<<0);             // Enable GPIOA
    *((uint32_t*)(GPIOA_BASEADDR + 0x00)) |= (2 << 0);         // Set Mode: Alternate Function
    *((uint32_t*)(GPIOA_BASEADDR + 0x20)) |= (1 << 0);         // Select Alternate Function
}

void Init_Timer(void)
{
    *(uint32_t*)(RCC_BASEADDR + 0x40) |= (1 << 0);				// Enable Timer2 clock
//    *(uint32_t*)(TIM2_BASEADDR + 0x28) = 15;					// Set PSC
//    *(uint32_t*)(TIM2_BASEADDR + 0x2C) = 20000-1;               // Set ARR
      *(uint32_t*)(TIM2_BASEADDR + 0x28) = 1;					// Set PSC
      *(uint32_t*)(TIM2_BASEADDR + 0x2C) = 5000-1;               // Set ARR
    *(uint32_t*)(TIM2_BASEADDR + 0x20) = 1;						// Config channel 1
    *(uint32_t*)(TIM2_BASEADDR + 0x18) = (6 << 4) | (1 << 3);	// Set PWM mode
    //*(uint32_t*)(TIM2_BASEADDR + 0x14) = (1 << 0);
    *(uint32_t*)(TIM2_BASEADDR) |= (1 << 7) | (1 << 0);			// Enable Timer
}

int main(void)
{
    Init_GPIO_TIMER();
    Init_Timer();

    while (1)
    {
    	for (uint32_t i = 1000; i < 4500; i = i + 100)
    	{
    		if (i == 4500)
    		{
    			i = 1000;
    		}
    		 // Set vị trí servo 0->500, 1500->90, 2500->180
    		for (uint32_t j = 0; j < 100; j++)
    		{
    			*(uint32_t*)(TIM2_BASEADDR + 0x34) = i;
    		}
    	}

    }

}

