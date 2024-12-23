#include "stm32f407xx.h"
//#include "lcd_i2c.h"
#include "lcd.h"

//I2C_Handle_t I2C1_Handle;
//#define MY_ADDR 0x6D
//
//
//void I2C1_GPIOInits(void)
//{
//	GPIO_Handle_t I2CPins;
//
//	I2CPins.pGPIOx = GPIOB;
//	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
//	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
//	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
//	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
//
//	//SCL
//	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
//	GPIO_Init(&I2CPins);
//
//	//SDA
//	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
//	GPIO_Init(&I2CPins);
//
//
//}
//
//void I2C1_Init(void)
//{
//	I2C1_Handle.pI2Cx = I2C1;
//	I2C1_Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
//	I2C1_Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
//	I2C1_Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
//	I2C1_Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
//
//	I2C_Init(&I2C1_Handle);
//}




int main(void)
{

	lcd_init();
	lcd_print_string("Di metro choi khong");
	lcd_set_cursor(2, 4);
	lcd_print_string("Dihi bao");

	while(1);

}
