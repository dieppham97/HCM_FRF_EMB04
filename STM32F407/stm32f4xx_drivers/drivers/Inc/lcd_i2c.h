/*
 * lcd_i2c.h
 *
 *  Created on: Oct 27, 2024
 *      Author: diepp
 */

#ifndef HEADER_LCD_I2C_H_
#define HEADER_LCD_I2C_H_

#include "stm32f407xx_i2c_driver.h"

#define SLAVE_ADDRESS_LCD 0x27

extern I2C_Handle_t I2C1_Handle;

void init_lcd (void);
void lcd_send_command (uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_send_string(char*);
void lcd_put_cursor(uint8_t row, uint8_t column);


#endif /* HEADER_LCD_I2C_H_ */
