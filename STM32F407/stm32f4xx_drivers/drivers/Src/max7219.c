/*
 * max7219.c
 *
 *  Created on: Sep 13, 2024
 *      Author: diepp
 */

#include "max7219.h"

void Max7219_Init(MAX7219_Config_t pMax7219, SPI_RegDef_t *pSPIx)
{
  uint16_t temp = 0;
  /* Shut down mode */
  temp = (SHUT_DOWN | pMax7219.ShutDown_Mode);
  SPI_SendData(pSPIx, temp);

  /* Display test */
  temp = 0;
  SPI_SendData(pSPIx, 0x0F00U);

  /* Decode mode*/
  temp = 0;
  temp = (DECODE | pMax7219.Decode_Mode);
  SPI_SendData(pSPIx, temp);

  /* Intensity */
  temp = 0;
  temp = (INTENSITY | pMax7219.Intensity_Mode);
  SPI_SendData(pSPIx, temp);

  /* Scan limit*/
  temp = 0;
  temp = (SCAN_LIMIT | pMax7219.ScanLimit_Mode);
  SPI_SendData(pSPIx, temp);
}

/// @brief
/// @param digit
/// @param value
void DIGITx_Init(SPI_RegDef_t *pSPIx, uint16_t digit, uint8_t value, uint8_t point)
{
  uint16_t temp = 0;
  switch (value)
  {
  case 0:
    /* code */
    temp |= (digit | CHAR_0);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 1:
    temp |= (digit | CHAR_1);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 2:
    temp |= (digit | CHAR_2);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 3:
    temp |= (digit | CHAR_3);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 4:
    temp |= (digit | CHAR_4);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 5:
    temp |= (digit | CHAR_5);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 6:
    temp |= (digit | CHAR_6);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 7:
    temp |= (digit | CHAR_7);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 8:
    temp |= (digit | CHAR_8);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 9:
    temp |= (digit | CHAR_9);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case '-':
    temp |= (digit | CHAR__);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 'E':
  case 'e':
    temp |= (digit | CHAR_E);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 'H':
  case 'h':
    temp |= (digit | CHAR_H);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 'L':
  case 'l':
    temp |= (digit | CHAR_L);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  case 'P':
  case 'p':
    temp |= (digit | CHAR_P);
    if (point == POINT_EN)
    {
      temp |= CHAR_POINT;
    }
    break;
  default:
    temp |= (digit | CHAR_BLANK);
    break;
  }
  SPI_SendData(pSPIx, temp);
}

void DISPLAY_Date(SPI_RegDef_t *pSPI, uint8_t day, uint8_t month, uint16_t year)
{
  /* Set year */
  uint8_t year_d1, year_d2, year_d3, year_d4;
  year_d1 = (year / 1000);
  year_d2 = ((year / 100) % 10);
  year_d3 = ((year / 10) % 10);
  year_d4 = (year % 10);
  DIGITx_Init(pSPI, DIGIT0, year_d4, POINT_DI);
  DIGITx_Init(pSPI, DIGIT1, year_d3, POINT_DI);
  DIGITx_Init(pSPI, DIGIT2, year_d2, POINT_DI);
  DIGITx_Init(pSPI, DIGIT3, year_d1, POINT_DI);

  /* Set Month */
  uint8_t month_d1, month_d2;
  if (day < 10)
  {
    month_d1 = 0;
    month_d2 = (month % 10);
  }
  else
  {
    month_d1 = (month / 10);
    month_d2 = (month % 10);
  }
  DIGITx_Init(pSPI, DIGIT4, month_d2, POINT_EN);
  DIGITx_Init(pSPI, DIGIT5, month_d1, POINT_DI);

  /* Set day */
  uint8_t day_d1, day_d2;
  if (day < 10)
  {
    day_d1 = 0;
    day_d2 = (day % 10);
  }
  else
  {
    day_d1 = (day / 10);
    day_d2 = (day % 10);
  }

  DIGITx_Init(pSPI, DIGIT6, day_d2, POINT_EN);
  DIGITx_Init(pSPI, DIGIT7, day_d1, POINT_DI);
}

void DISPLAY_Time(SPI_RegDef_t *pSPI, uint8_t sec, uint8_t min, uint8_t hour)
{
  /* Set hour */
  uint8_t hour_d1, hour_d2;
  if (hour < 10)
  {
    hour_d1 = 0;
    hour_d2 = (hour % 10);
  }
  else
  {
    hour_d1 = (hour / 10);
    hour_d2 = (hour % 10);
  }
  DIGITx_Init(pSPI, DIGIT7, hour_d1, POINT_DI);
  DIGITx_Init(pSPI, DIGIT6, hour_d2, POINT_DI);
  DIGITx_Init(pSPI, DIGIT5, '-', POINT_DI);

  /* Set min */
  uint8_t min_d1, min_d2;
  if (min < 10)
  {
    min_d1 = 0;
    min_d2 = (min % 10);
  }
  else
  {
    min_d1 = (min / 10);
    min_d2 = (min % 10);
  }
  DIGITx_Init(pSPI, DIGIT4, min_d1, POINT_DI);
  DIGITx_Init(pSPI, DIGIT3, min_d2, POINT_DI);
  DIGITx_Init(pSPI, DIGIT2, '-', POINT_DI);

  /* Set sec */
  uint8_t sec_d1, sec_d2;
  if (sec < 10)
  {
    sec_d1 = 0;
    sec_d2 = (sec % 10);
  }
  else
  {
    sec_d1 = (sec / 10);
    sec_d2 = (sec % 10);
  }
  DIGITx_Init(pSPI, DIGIT1, sec_d1, POINT_DI);
  DIGITx_Init(pSPI, DIGIT0, sec_d2, POINT_DI);
}

void Display_Fault(SPI_RegDef_t *pSPIx, uint8_t Appli)
{
  uint16_t temp = 0;
  SPI_SendData(SPI1, 0x0900);
  /* Display format */
  if (Appli == FAULT_FORMAT)
  {
	  /* DIGIT 4 */
	  temp = (DIGIT1 | LED_G);
	  SPI_SendData(SPI1, temp);
	    /* DIGIT 4 */
	    temp = (DIGIT2 | LED_F | LED_E | LED_D);
	    SPI_SendData(SPI1, temp);
	    /* DIGIT 5 */
	    temp = (DIGIT3 | LED_F | LED_E | LED_B | LED_C | LED_D);
	    SPI_SendData(SPI1, temp);
	    /* DIGIT 6 */
	    temp = (DIGIT4 | LED_F | LED_E | LED_A | LED_G | LED_C | LED_B);
	    SPI_SendData(SPI1, temp);
	    /* DIGIT 7 */
	    temp = (DIGIT5 | LED_F | LED_E | LED_A | LED_G);
	    SPI_SendData(SPI1, temp);
  }
}
