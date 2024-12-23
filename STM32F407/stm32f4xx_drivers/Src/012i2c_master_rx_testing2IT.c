/*
 * 010i2c_master_tx_testing.c
 *
 *  Created on: Apr 14, 2024
 *      Author: diepp
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

//Flag variable
uint8_t rxComplt = RESET;
//rcv buffer
uint8_t rcv_buf[32];

#define MY_ADDR 0x61

#define SLAVE_ADDR	0x68

void delay(void)
{
	for(uint32_t i = 0; i < 500000/2; i++);
}

I2C_Handle_t I2C1_Handle;

//rcv buffer
uint8_t rcv_buf[32];


/*
 * PB6 -> SCL
 * PB7 -> SDA
 *
 */


void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	//SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);


}

void I2C1_Init(void)
{
	I2C1_Handle.pI2Cx = I2C1;
	I2C1_Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1_Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1_Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1_Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1_Handle);
}


void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn, GpioLed;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

	//this is led gpio configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD, ENABLE);

	GPIO_Init(&GpioLed);
}

int main(void)
{
	uint8_t commandcode;

	uint8_t len;

	GPIO_ButtonInit();

	//i2c pin init
	I2C1_GPIOInits();

	//i2c peripheral configuration
	I2C1_Init();

	//I2C IRQ configurations
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);			//use the interrupt so need enable IRQ number
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	//enable the i2c peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	//ack bit is made 1 after pe = 1
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

	//wait for button press
	while(1)
	{
		//wait till button is pressed
		while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0))

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = 0x51;

		while (I2C_MasterSendDataIT(&I2C1_Handle, &commandcode, 1, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

		while (I2C_MasterReceiveDataIT(&I2C1_Handle, &len, 1, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

		commandcode = 0x51;
		while (I2C_MasterSendDataIT(&I2C1_Handle, &commandcode, 1, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
		while (I2C_MasterReceiveDataIT(&I2C1_Handle, rcv_buf, len, SLAVE_ADDR, I2C_DISABLE_SR) != I2C_READY);

		rxComplt = RESET;

		//wait till rx complete
		while(rxComplt != SET);

		rcv_buf[len+1] = '\0';

		//printf("Data: %s,rcv_buf);
		rxComplt = RESET;
	}
}


void I2C_EV_IRQHandler (void)
{
	I2C_EV_IRQHandling(&I2C1_Handle);
}



void I2C1_ER_IRQHandler (void)
{
	I2C_EV_IRQHandling(&I2C1_Handle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
	if(AppEv == I2C_EV_TX_CMPLT)
	{
		printf("Tx is complete\n");
	}else if(AppEv == I2C_EV_RX_CMPLT)
	{
		printf("Rx is complete\n");
		rxComplt = SET;
	}else if(AppEv == I2C_ERROR_AF)
	{
		printf("Error: Ack Failure\n");
		//in master ack failure happens when slave fails toi send ack for the byte
		//sent from the master
		I2C_CloseSendData(pI2CHandle);

		//generate the stop condition to release the bus
		I2C_GenerateStopCondition(I2C1);

		//hang in infinite loop
		while (1);
	}
}
