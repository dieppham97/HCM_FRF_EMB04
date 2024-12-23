/*
 * 007spi_txonly_andruino.c
 *
 *  Created on: Apr 1, 2024
 *      Author: diepp
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

/*
 * PB12 --> SPI2_NSS
 * PB13 --> SPI2_SCLK
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * ALT function mode: 5
 */


/* LED 7 */


#define LED_B					7
#define LED_A					5
#define LED_G					1//
#define LED_F					3

#define LED_POINT				6//
#define LED_C					4
#define LED_D					2//
#define LED_E					0



void delay(void)
{
	for(uint32_t i = 0; i < 1000000/2; i++);
}


void SPI3_GPIOInits(void)
{
	GPIO_Handle_t SPI3Pins;
	SPI3Pins.pGPIOx = GPIOC;
	SPI3Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPI3Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 6;				//AF5
	SPI3Pins.GPIO_PinConfig.GPIO_PinOPType =	GPIO_OP_TYPE_PP;			//Open Drain is not required for SPI, but I2C need it
	SPI3Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPI3Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPI3Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(&SPI3Pins);

	//MOSI
	SPI3Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPI3Pins);

	//MISO
	SPI3Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
	GPIO_Init(&SPI3Pins);


	GPIO_Handle_t SPI3PinNss;
	SPI3PinNss.pGPIOx = GPIOA;
	SPI3PinNss.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPI3PinNss.GPIO_PinConfig.GPIO_PinAltFunMode = 6;				//AF5
	SPI3PinNss.GPIO_PinConfig.GPIO_PinOPType =	GPIO_OP_TYPE_PP;			//Open Drain is not required for SPI, but I2C need it
	SPI3PinNss.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPI3PinNss.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	SPI3PinNss.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_4;
	GPIO_Init(&SPI3PinNss);
}


void SPI3_Init(void)
{
	SPI_Handle_t SPI3handle;

	SPI3handle.pSPIx = SPI3;
	SPI3handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI3handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_SLAVE;
	//SPI3handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI3handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI3handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI3handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI3handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI3handle.SPIConfig.SPI_SSM = SPI_SSM_DI;		//software slave management enabled for NSS pin

	SPI_Init(&SPI3handle);
}

void GPIO_LEDInit(void)
{
	GPIO_Handle_t GpioLedD;

	//this is led gpio configuration
	GpioLedD.pGPIOx = GPIOD;

	GpioLedD.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLedD.GPIO_PinConfig.GPIO_PinAltFunMode = 0;
	GpioLedD.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLedD.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLedD.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;


	//GPIO_PeriClockControl(GPIOD, ENABLE);

	GpioLedD.GPIO_PinConfig.GPIO_PinNumber = LED_A;
	GPIO_Init(&GpioLedD);

	GpioLedD.GPIO_PinConfig.GPIO_PinNumber = LED_B;
	GPIO_Init(&GpioLedD);

	GpioLedD.GPIO_PinConfig.GPIO_PinNumber = LED_C;
	GPIO_Init(&GpioLedD);

	GpioLedD.GPIO_PinConfig.GPIO_PinNumber = LED_D;
	GPIO_Init(&GpioLedD);

	GpioLedD.GPIO_PinConfig.GPIO_PinNumber = LED_E;
	GPIO_Init(&GpioLedD);

	GpioLedD.GPIO_PinConfig.GPIO_PinNumber = LED_F;
	GPIO_Init(&GpioLedD);

	GpioLedD.GPIO_PinConfig.GPIO_PinNumber = LED_G;
	GPIO_Init(&GpioLedD);

	GpioLedD.GPIO_PinConfig.GPIO_PinNumber = LED_POINT;
	GPIO_Init(&GpioLedD);
}




void Setup_LED (uint8_t *rx)
{
	GPIOD->ODR = 0;
	uint8_t index = 0;
	uint32_t temp = 0;

	uint8_t c[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','*','#'};
	for (uint32_t i = 0; i <16; i++)
	{
		//if(strcmp(rx, &c[i]) == 0)
		if (*rx == c[i])
		{
			index = i;
			break;
		}

	}
	switch (index)
	{
		case 0:
		{
			temp =	 (1 << LED_B) | (1 << LED_C)
					|(1 << LED_E) | (1 << LED_F)
					|(1 << LED_A) | (1 << LED_D);
			break;
		}
		case 1:
		{
			temp =   (1 << LED_B) | (1 << LED_C);
			break;
		}
		case 2:
		{
			temp =	 (1 << LED_B) | (1 << LED_A)
					|(1 << LED_G) | (1 << LED_E) | (1 << LED_D);
			break;
		}
		case 3:
		{
			temp =  (1 << LED_A) | (1 << LED_B) | (1 << LED_C)|
					(1 << LED_D) | (1 << LED_G);
			break;
		}
		case 4:
		{
			temp =  (1 << LED_B) | (1 << LED_C)|
					(1 << LED_F) | (1 << LED_G);
			break;
		}
		case 5:
		{
			temp =  (1 << LED_A) | (1 << LED_F) | (1 << LED_C)|
					(1 << LED_D) | (1 << LED_G);
			break;
		}
		case 6:
		{
			temp =  (1 << LED_A) | (1 << LED_F) | (1 << LED_C)|
					(1 << LED_D) | (1 << LED_G) | (1 << LED_E);
			break;
		}
		case 7:
		{
			temp =  (1 << LED_A) | (1 << LED_B) | (1 << LED_C);
			break;
		}
		case 8:
		{
			temp =  (1 << LED_A) | (1 << LED_F) | (1 << LED_C)|
					(1 << LED_D) | (1 << LED_G) | (1 << LED_E)| (1 << LED_B);
			break;
		}
		case 9:
		{
			temp =  (1 << LED_A) | (1 << LED_F) | (1 << LED_C)| (1 << LED_B) |
					(1 << LED_D) | (1 << LED_G);
			break;
		}
		case 10:
		{
			temp =  (1 << LED_A) | (1 << LED_F) | (1 << LED_C)|
					(1 << LED_G) | (1 << LED_E) | (1 << LED_POINT);
			break;
		}
		case 11:
		{
			temp =  (1 << LED_A) | (1 << LED_F) | (1 << LED_C)| (1 << LED_B) |
					(1 << LED_D) | (1 << LED_G) | (1 << LED_E)| (1 << LED_POINT);
			break;
		}
		case 12:
		{
			temp =  (1 << LED_A) | (1 << LED_C)| (1 << LED_B)| (1 << LED_POINT);
			break;
		}
		case 13:
		{
			temp =  (1 << LED_A) | (1 << LED_F) | (1 << LED_C) | (1 << LED_B) |
					(1 << LED_D) | (1 << LED_E) | (1 << LED_POINT);
			break;
		}case 14:
		{
			temp =  (1 << LED_B) | (1 << LED_G) | (1 << LED_E)| (1 << LED_POINT);
			break;
		}
		default:
		{
			temp =  (1 << LED_F) | (1 << LED_C)| (1 << LED_G) | (1 << LED_POINT);
			break;
		}
	}
	GPIOD->ODR = temp;
}

int main(void)
{



	GPIO_LEDInit();

	//this function is used to initialize the GPIO pins to behave as SIP2 pins
	SPI3_GPIOInits();
//
	//this function is used to initialize the SPI peripheral parameters
	SPI3_Init();

	uint8_t rx_buff;

	while(1)
	{
		SPI_PeripheralControl(SPI3, ENABLE);

		SPI_ReceiveData(SPI3, &rx_buff, 1);
		while(SPI_GetFlagStatus(SPI3,SPI_BUSY_FLAG) );
		SPI_PeripheralControl(SPI3, DISABLE);
		Setup_LED(&rx_buff);
		delay();
		//
	}

	return 0;
}

