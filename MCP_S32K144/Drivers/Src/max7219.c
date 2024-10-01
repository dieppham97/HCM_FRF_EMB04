#include "max7219.h"

extern uint8_t delay;

void Max7219_Init(MAX7219_Config_t pMax7219, SPI_RegDef_t *pSPIx)
{
  uint16_t temp = 0;
  /* Shut down mode */
  temp = (SHUT_DOWN | pMax7219.ShutDown_Mode);
  SPI_SendData(pSPIx, temp);

  /* Display test */
  SPI_SendData(pSPIx, 0x0F00U);

  /* Decode mode*/
  temp = (DECODE | pMax7219.Decode_Mode);
  SPI_SendData(pSPIx, temp);

  /* Intensity */
  temp = (INTENSITY | pMax7219.Intensity_Mode);
  SPI_SendData(pSPIx, temp);

  /* Scan limit*/
  temp = (SCAN_LIMIT | pMax7219.ScanLimit_Mode);
  SPI_SendData(pSPIx, temp);
}

/*******************************************************************************
 * @fn              - DIGITx_Init
 *
 * @brief           - This function initializes a specific digit on the MAX7219 LED display
 *
 * @param[in]       - pSPIx: Pointer to the SPI peripheral used for communication
 * @param[in]       - digit: The digit position to initialize (DIGIT0 to DIGIT7)
 * @param[in]       - value: The value to display (0-9, '-', 'E', 'H', 'L', 'P', or any other for blank)
 * @param[in]       - point: Whether to enable the decimal point (POINT_EN or POINT_DI)
 *
 * @return          - None
 *
 * @Note            - This function sets up a single digit on the MAX7219 LED display
 *                    It supports numbers 0-9 and some letters, with optional decimal point
 */
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

/*******************************************************************************
 * @fn              - DISPLAY_Date
 *
 * @brief           - This function displays the date on a 7-segment display using SPI communication
 *
 * @param[in]       - pSPI: Pointer to the SPI peripheral register structure
 * @param[in]       - day: Day of the month (1-31)
 * @param[in]       - month: Month of the year (1-12)
 * @param[in]       - year: Year (e.g., 2023)
 *
 * @return          - None
 *
 * @Note            - The date is displayed in the format: DD.MM.YYYY
 *                    The function uses DIGITx_Init to set each digit of the display
 */
void DISPLAY_Date(SPI_RegDef_t *pSPI, uint8_t day, uint8_t month, uint16_t year)
{
  uint8_t month_d1, month_d2;
  uint8_t day_d1, day_d2;
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

  if (month < 10)
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

/*******************************************************************************
 * @fn              - DISPLAY_Time
 *
 * @brief           - This function displays the time on a 7-segment display using SPI communication
 *
 * @param[in]       - pSPI: Pointer to the SPI peripheral register structure
 * @param[in]       - sec: Seconds (0-59)
 * @param[in]       - min: Minutes (0-59)
 * @param[in]       - hour: Hours (0-23)
 *
 * @return          - None
 *
 * @Note            - The time is displayed in the format: HH-MM-SS
 *                    The function uses DIGITx_Init to set each digit of the display
 */
void DISPLAY_Time(SPI_RegDef_t *pSPI, uint8_t sec, uint8_t min, uint8_t hour)
{
  uint8_t sec_d1, sec_d2;
  uint8_t min_d1, min_d2;
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

/*******************************************************************************
 * @fn              - Toggle_Display
 *
 * @brief           - This function toggles the display on and off for a specified number of times
 *
 * @param[in]       - pSPIx: Pointer to the SPI peripheral used for communication
 * @param[in]       - toggle: Number of times to toggle the display
 *
 * @return          - None
 *
 * @Note            - This function uses the SHUT_DOWN register to turn the display on and off
 *                    It waits for a delay between each toggle to make the effect visible
 */
void Toggle_Display(SPI_RegDef_t *pSPIx, uint8_t toggle)
{
  uint16_t temp_Display = 0;
  for (int i = 0; i < toggle; i++)
  {
    // Turn off the display
    temp_Display = (SHUT_DOWN | 0x00);
    SPI_SendData(pSPIx, temp_Display);
    while (delay < 1)
      ;
    if (delay >= 1)
    {
      delay = 0;
    }
    // Turn on the display
    temp_Display = (SHUT_DOWN | 0x01);
    SPI_SendData(pSPIx, temp_Display);
    while (delay < 1)
      ;
    if (delay >= 1)
    {
      delay = 0;
    }
  }
}

/*******************************************************************************
 * @fn              - Display_Error
 *
 * @brief           - This function displays an error message on the MAX7219 LED display
 *
 * @param[in]       - pSPIx: Pointer to the SPI peripheral used for communication
 * @param[in]       - error: The error code to display
 *
 * @return          - None
 *
 * @Note            - This function displays different error messages based on the error code
 *                    It uses specific LED segments to represent different types of errors
 *                    After displaying the error, it toggles the display and resets to decode mode
 */
void Display_Error(SPI_RegDef_t *pSPIx, uint8_t error)
{
  uint16_t temp = 0;
  SPI_SendData(pSPIx, 0x0900);
  /* DIGIT 0 */
  temp = (DIGIT0 | 0);
  SPI_SendData(pSPIx, temp);
  /* DIGIT 2 */
  temp = (DIGIT2 | LED_G);
  SPI_SendData(pSPIx, temp);
  if (error < ERROR_DATE_MONTH)
  {
    /* DIGIT 3 */
    temp = (DIGIT3 | LED_B | LED_C);
    SPI_SendData(pSPIx, temp);
    if (error == ERROR_TIME_HOURS)
    {
      /* DIGIT 1 */
      temp = (DIGIT1 | LED_B | LED_C);
      SPI_SendData(pSPIx, temp);
    }
    else if (error == ERROR_TIME_MINUTES)
    {
      /* DIGIT 1 */
      temp = (DIGIT1 | LED_A | LED_B | LED_G | LED_E | LED_D);
      SPI_SendData(pSPIx, temp);
    }
    else if (error == ERROR_TIME_SECONDS)
    {
      /* DIGIT 1 */
      temp = (DIGIT1 | LED_A | LED_B | LED_G | LED_C | LED_D);
      SPI_SendData(pSPIx, temp);
    }
  }
  else if (error == ERROR_FORMAT)
  {
    /* DIGIT 1 */
    temp = (DIGIT1 | 0);
    SPI_SendData(pSPIx, temp);
    /* DIGIT 2 */
    temp = (DIGIT2 | 0);
    SPI_SendData(pSPIx, temp);
    /* DIGIT 3 */
    temp = (DIGIT3 | LED_A | LED_B | LED_G | LED_C | LED_D);
    SPI_SendData(pSPIx, temp);
  }
  else
  {
    /* DIGIT 3 */
    temp = (DIGIT3 | LED_A | LED_B | LED_G | LED_E | LED_D);
    SPI_SendData(pSPIx, temp);
    if (error == ERROR_DATE_MONTH)
    {
      /* DIGIT 1 */
      temp = (DIGIT1 | LED_A | LED_B | LED_G | LED_E | LED_D);
      SPI_SendData(pSPIx, temp);
    }
    else if (error == ERROR_DATE_DAY)
    {
      /* DIGIT 1 */
      temp = (DIGIT1 | LED_B | LED_C);
      SPI_SendData(pSPIx, temp);
    }
  }

  /* DIGIT 4 */
  temp = (DIGIT4 | LED_F | LED_E | LED_A);
  SPI_SendData(pSPIx, temp);
  /* DIGIT 5 */
  temp = (DIGIT5 | LED_F | LED_E | LED_A);
  SPI_SendData(pSPIx, temp);
  /* DIGIT 6 */
  temp = (DIGIT6 | LED_G | LED_A | LED_F | LED_E | LED_D);
  SPI_SendData(pSPIx, temp);
  /* DIGIT 7 */
  temp = (DIGIT7 | 0);
  SPI_SendData(pSPIx, temp);
  /* Delay */
  Toggle_Display(pSPIx, 3);
  /* Back decode 0-7  */
  temp = (DECODE | 0xFF);
  SPI_SendData(pSPIx, temp);
}
