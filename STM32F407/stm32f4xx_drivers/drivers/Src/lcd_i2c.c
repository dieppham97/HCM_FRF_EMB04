/*
 * lcd_i2c.c
 *
 *  Created on: Oct 27, 2024
 *      Author: diepp
 */

#ifndef SRC_LCD_I2C_C_
#define SRC_LCD_I2C_C_

#include "lcd_i2c.h"

static void mdelay(uint32_t cnt);
static void udelay(uint32_t cnt);

void lcd_send_cmmand (uint8_t cmd)
{
	uint8_t data_u, data_l;
	uint8_t data[4];
	data_u = (cmd & 0xf0);
	data_l = ((cmd << 4) & 0xf0);
	data[0] = data_u | 0x0C;  //en=1, rs=0
	data[1] = data_u | 0x08;  //en=0, rs=0
	data[2] = data_l | 0x0C;  //en=1, rs=0
	data[3] = data_l | 0x08;  //en=0, rs=0
	I2C_MasterSendData(&I2C1_Handle, (uint8_t*) data, 4, SLAVE_ADDRESS_LCD, I2C_ENABLE_SR);
}


void lcd_send_data (uint8_t data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = data_u | 0x0D;  //en=1, rs=1
	data_t[1] = data_u | 0x09;  //en=0, rs=1
	data_t[2] = data_l | 0x0D;  //en=1, rs=1
	data_t[3] = data_l | 0x09;  //en=0, rs=1
	I2C_MasterSendData(&I2C1_Handle, (uint8_t*) data_t, 4, SLAVE_ADDRESS_LCD, I2C_ENABLE_SR);
}

void lcd_send_string(char *message)
{
	do
	{
		lcd_send_data((uint8_t)*message++);
	}
	while (*message != '\0');
}

/**
  *   Set Lcd to a specified location given by row and column information
  *   Row Number (1 to 2)
  *   Column Number (1 to 16) Assuming a 2 X 16 characters display
  */
void lcd_put_cursor(uint8_t row, uint8_t column)
{
	column--;
	switch(row)
	{
	case 1:
		/* Set cursor to 1st row address and add index*/
		column |= 0x80;
		break;
	case 2:
		/* Set cursor to 2nd row address and add index*/
		column |= 0xC0;
		break;
	default:
		break;
	}
	lcd_send_cmmand(column);
}

void init_lcd (void)
{

	mdelay(40);

	lcd_send_cmmand(0x30);

	mdelay(5);

	lcd_send_cmmand(0x30);

	udelay(150);
	lcd_send_cmmand(0x30);

	lcd_send_cmmand(0x20);

	//display initialize
	//function set command
	lcd_send_cmmand(0x28);	// Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)

	//display ON and cursor ON
	lcd_send_cmmand(0x0E);

	//display clear
	lcd_send_cmmand(0x1);

	//entry mode set
	lcd_send_cmmand(0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)

	lcd_send_cmmand(0x0C);
}

static void mdelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 1000); i++);
}

static void udelay(uint32_t cnt)
{
	for(uint32_t i =0; i < (cnt * 1); i++);
}


#endif /* SRC_LCD_I2C_C_ */
