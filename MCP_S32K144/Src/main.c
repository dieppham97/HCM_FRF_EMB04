/*
 * main.c
 *
 *  Created on: Sep 29, 2023
 *      Author: Diep Pham
 */
#include "main.h"

int main(void)
{
	uint16_t tempIntensi = 0;
	cur_date.day = 1;
	cur_date.month = 1;
	cur_date.year = 1971;

	/* Init peripheral */
	Init_Peripheral();

	while (1)
	{
		Set_Time();
		SPI_PeripheralControl(SPI1, ENABLE);
		if (SW3 == 1)
		{
			SW3 = 0;
			SW3_Status = !SW3_Status;
		}
		Check_SW3();
		if (SW2 == 1)
		{
			SW2 = 0;
			SW2_Status = !SW2_Status;
		}
		Check_SW2();
		Potentiometer();
		tempIntensi = (INTENSITY | ADC_Pe);
		SPI_SendData(SPI1, tempIntensi);

#if 0
		if (TouchSense1())
		{
			IntensityTouch++;
			if (IntensityTouch > 15)
			{
				IntensityTouch = 15;
			}
			tempIntensi = (INTENSITY | IntensityTouch);
			SPI_SendData(SPI1, tempIntensi);
		}
#endif
		if (data_request != 0)
		{
			Check_ReceiveData();
			data_request = 0;
		}
		if (alarm)
		{
			if ((alarm_time.hours == cur_time.hours) && (alarm_time.minutes == cur_time.minutes))
			{
				for (uint8_t i = 0; i < 5; i++)
				{
					Toggle_Display(SPI1, 1);
				}
				alarm = 0;
			}
		}
		SPI_PeripheralControl(SPI1, DISABLE);
	}
}

void LPIT0_Ch3_IRQHandler(void)
{
	LPIT->MSR |= (1 << CHANNEL3);
	delay++;
}

void LPUART1_RxTx_IRQHandler(void)
{
	if (UART1->STAT & (1u << 20))
	{
		data_request = index;
		UART1->STAT |= (1u << 20);
		index = 0;
	}
	/*For Receive*/
	/*Wait for buffer is full*/
	if (data_request == 0)
	{
		while (!(UART1->STAT & (1u << 21)))
		{
		}
		rx_buf[index] = (char)UART1->DATA;
		index++;
	}
}

void PORTC_IRQHandler(void)
{
	SW3 = ReadFromInput(GPIOC, GPIO_PIN_NO_13);
	SW2 = ReadFromInput(GPIOC, GPIO_PIN_NO_12);

	GPIO_IRQHandling(GPIOC, GPIO_PIN_NO_12);
	GPIO_IRQHandling(GPIOC, GPIO_PIN_NO_13);
}

void SysTick_Handler(void)
{
	sys_tick_count++;
	if (sys_tick_count == 100)
	{
		cur_time.seconds++;
		sys_tick_count = 0;
	}
}
