/*
 * 007spi_txonly_andruino.c
 *
 *  Created on: Apr 1, 2024
 *      Author: diepp
 */

#include "stm32f407xx.h"
#include "max7219.h"
#include <string.h>
#include <stdio.h>

#define SYSTICK_TIM_CLK	16000000UL

/*
 * PB4 --> SPI2_NSS
 * PB5 --> SPI2_SCLK
 * PB6 --> SPI2_MISO
 * PB7 --> SPI2_MOSI
 * ALT function mode: 5
 */


static MAX7219_date_t cur_date;
static MAX7219_time_t cur_time;
static uint8_t DayInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static uint8_t SW2_Status, SW2, SW2_PRE;

void delay(void)
{
	for(uint32_t i = 0; i < 10000/2; i++);
}

void SPI1_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOA;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;				//AF5
	SPIPins.GPIO_PinConfig.GPIO_PinOPType =	GPIO_OP_TYPE_PP;			//Open Drain is not required for SPI, but I2C need it
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_4;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_Init(&SPIPins);
}



void SPI1_Init(void)
{
	SPI_Handle_t SPI1handle;

	SPI1handle.pSPIx = SPI1;
	SPI1handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI1handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI1handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV4;		//generates sclk of 1MHz
	SPI1handle.SPIConfig.SPI_DFF = SPI_DFF_16BITS;
	SPI1handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI1handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI1handle.SPIConfig.SPI_SSM = SPI_SSM_DI;		//software slave management enabled for NSS pin

	SPI_Init(&SPI1handle);
}

void SW2_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&GPIOBtn);
}

void MAX7219_Init (SPI_RegDef_t *pSPIx)
{
	MAX7219_Config_t Max7219;

	Max7219.Decode_Mode = DECODE_FULL;
	Max7219.Intensity_Mode = INTEN_9;
	Max7219.ScanLimit_Mode = SCAN_0_7;
	Max7219.ShutDown_Mode = SHUT_ON;

	Max7219_Init(Max7219, pSPIx);
}

void Init_SysTick(void)
{
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;
	/* Calculation of reload value */
	uint32_t count_value = ((SYSTICK_TIM_CLK/100) - 1);

	/* Clear the value of SVR */
	*pSRVR &= ~(0x00FFFFFFFF);

	/* Load the value in to SVR */
	*pSRVR |= count_value;

	/* Enable SysTick */
	*pSCSR |= (1 << 1);		/* Enable SysTick exception request */
	*pSCSR |= (1 << 2);		/* Indicate the clock source: processor clock */
	*pSCSR |= (1 << 0);		/* Enable the counter */
}

void String_To_Time (char* rx_time,uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		cur_time.hours = ((rx_time[0] - '0')*10 + (rx_time[1] - '0'));

		cur_time.minutes = ((rx_time[3] - '0')*10 + (rx_time[4] - '0'));

		cur_time.seconds = ((rx_time[6] - '0')*10 + (rx_time[7] - '0'));
	}
}

void String_To_Date(char* rx_date,uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		cur_date.day = ((rx_date[0] - '0')*10 + (rx_date[1] - '0'));

		cur_date.month = ((rx_date[3] - '0') * 10 + (rx_date[4] - '0'));

		cur_date.year = ((rx_date[6] - '0') * 1000 + (rx_date[7] - '0') * 100 +
            		(rx_date[8] - '0') * 10 + (rx_date[9] - '0'));
	}
}
uint8_t Leap_Year(uint16_t year)
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
		return 1;
	return 0;
}
void Set_Time(void)
{
	if (cur_time.seconds > 59)
	{
		cur_time.seconds = 0;
		cur_time.minutes++;
	}
	if (cur_time.minutes > 59)
	{
		cur_time.minutes = 0;
		cur_time.hours++;
	}
	if (cur_time.hours > 23)
	{
		cur_time.hours = 0;
		cur_date.day++;
	}
	if(Leap_Year(cur_date.year))
		{
			DayInMonth[1] = 29;
		}else
		{
			DayInMonth[1] = 28;
		}
		if(cur_date.day > DayInMonth[cur_date.month])
		{
			cur_date.day = 1;
			cur_date.month++;
		}
		if (cur_date.month > 12)
		{
			cur_date.month = 1;
			cur_date.year++;
		}

}




void Check_SW2(void)
{

	if (SW2_Status == 0)
	{
		DISPLAY_Time(SPI1, cur_time.seconds, cur_time.minutes, cur_time.hours);
	}else if (SW2_Status == 1)
	{
		DISPLAY_Date(SPI1, cur_date.day, cur_date.month, cur_date.year);
	}
/*
	 if (SW2_Status == 1)
	{
		uint16_t temp = (SHUT_DOWN | SHUT_OFF);
		SPI_SendData(SPI1, temp);
	}
	else if (SW2_Status == 0)
	{
		uint16_t temp = (SHUT_DOWN | SHUT_ON);
		SPI_SendData(SPI1, temp);
	}*/

}

int main(void)
{
	char rx_buf[8] = "10-10-10";
	cur_date.day = 14;
	cur_date.month = 9;
	cur_date.year = 1997;

	/* Init systick */
	Init_SysTick();
	SW2_ButtonInit();
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);

	//this function is used to initialize the GPIO pins to behave as SIP2 pins
	SPI1_GPIOInits();

	//this function is uBsed to initialize the SPI peripheral parameters
	SPI1_Init();

	SPI_SSOEConfig(SPI1,ENABLE);

	SPI_PeripheralControl(SPI1, ENABLE);
	MAX7219_Init(SPI1);
	SPI_PeripheralControl(SPI1, DISABLE);
	if(0)
	String_To_Date(rx_buf, DISABLE);
	//DISPLAY_Date(SPI1, cur_date.day, cur_date.month, cur_date.year);
	while(1){
		Set_Time();
		SPI_PeripheralControl(SPI1, ENABLE);
		Check_SW2();
		SPI_PeripheralControl(SPI1, DISABLE);
	}
	return 0;
}

void SysTick_Handler(void)
{
	cur_time.seconds++;
}

void EXTI0_IRQHandler(void)
{
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	SW2 = GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0);
	if (SW2 && !SW2_PRE)
	{
		SW2_Status = !SW2_Status;
	}
	SW2_PRE = SW2;
}
