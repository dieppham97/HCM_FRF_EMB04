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




void delay(void)
{
	for(uint32_t i = 0; i < 1000000/2; i++);
}

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;				//AF5
	SPIPins.GPIO_PinConfig.GPIO_PinOPType =	GPIO_OP_TYPE_PP;			//Open Drain is not required for SPI, but I2C need it
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}



void SPI2_Init(void)
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	//SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_SLAVE;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;		//generates sclk of 1MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;		//software slave management enabled for NSS pin

	SPI_Init(&SPI2handle);
}

int main(void)
{

	//this function is used to initialize the GPIO pins to behave as SIP2 pins
	SPI2_GPIOInits();
//
	//this function is used to initialize the SPI peripheral parameters
	SPI2_Init();
	SPI_SSIConfig(SPI2,ENABLE);
	//SPI_SSOEConfig(SPI2, ENABLE);

	uint8_t t[10] = {'1','2','3','4','5','6','7','8','9','0'};
	uint8_t tx_buff;
	uint8_t i = 0;
	while(1)
	{
		tx_buff = t[i];
		SPI_PeripheralControl(SPI2, ENABLE);
		SPI_SendData(SPI2,&tx_buff,1);
		while( SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG) );
		SPI_PeripheralControl(SPI2, DISABLE);
		i++;
		if (i > 10)
			i = 0;

		delay();

	}

	return 0;
}

