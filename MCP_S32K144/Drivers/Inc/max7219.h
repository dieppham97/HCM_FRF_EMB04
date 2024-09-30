#ifndef MAX7219_H_
#define MAX7219_H_

#include "S32K1xx_spi_driver.h"

typedef struct max7219
{
  /* data */
  uint8_t Decode_Mode;    /* Select decode */
  uint8_t Intensity_Mode; /* Select light intensity */
  uint8_t ScanLimit_Mode; /* Select numbers */
  uint8_t ShutDown_Mode;  /* Select shutdown */
} MAX7219_Config_t;

typedef struct
{
  uint8_t day;
  uint8_t month;
  uint16_t year;
} MAX7219_date_t;

typedef struct
{
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
} MAX7219_time_t;

#define DIGIT0 (1 << 8)
#define DIGIT1 (2 << 8)
#define DIGIT2 (3 << 8)
#define DIGIT3 (4 << 8)
#define DIGIT4 (5 << 8)
#define DIGIT5 (6 << 8)
#define DIGIT6 (7 << 8)
#define DIGIT7 (8 << 8)
#define DECODE (9 << 8)
#define INTENSITY (10 << 8)
#define SCAN_LIMIT (11 << 8)
#define SHUT_DOWN (12 << 8)

#define CHAR_0 0U
#define CHAR_1 1U
#define CHAR_2 2U
#define CHAR_3 3U
#define CHAR_4 4U
#define CHAR_5 5U
#define CHAR_6 6U
#define CHAR_7 7U
#define CHAR_8 8U
#define CHAR_9 9U
#define CHAR__ 10U
#define CHAR_E 11U
#define CHAR_H 12U
#define CHAR_L 13U
#define CHAR_P 14U
#define CHAR_BLANK 15U
#define CHAR_POINT (1 << 7)

/* No Decode */
#define LED_A (1U << 6)
#define LED_B (1U << 5)
#define LED_C (1U << 4)
#define LED_D (1U << 3)
#define LED_E (1U << 2)
#define LED_F (1U << 1)
#define LED_G (1U << 0)
#define LED_POINT (1U << 7)

/* Decode mode*/
#define DECODE_NO 0U
#define DECODE_0 1U
#define DECODE_0_3 15U
#define DECODE_FULL 255U

/* Intensity mode*/
#define INTEN_1 0U
#define INTEN_3 1U
#define INTEN_5 2U
#define INTEN_7 3U
#define INTEN_9 4U
#define INTEN_11 5U
#define INTEN_13 6U
#define INTEN_15 7U
#define INTEN_17 8U
#define INTEN_19 9U
#define INTEN_21 10U
#define INTEN_23 11U
#define INTEN_25 12U
#define INTEN_27 13U
#define INTEN_29 14U
#define INTEN_31 15U

/* Scan limit mode */
#define SCAN_0 0U
#define SCAN_0_1 1U
#define SCAN_0_2 2U
#define SCAN_0_3 3U
#define SCAN_0_4 4U
#define SCAN_0_5 5U
#define SCAN_0_6 6U
#define SCAN_0_7 7U

/* Shut down mode*/
#define SHUTD_OFF 0U
#define SHUTD_ON 1U

/* Point */
#define POINT_DI 0U
#define POINT_EN 1U

/* Error */
#define ERROR_TIME_HOURS 1U
#define ERROR_TIME_MINUTES 3U
#define ERROR_TIME_SECONDS 3U
#define ERROR_DATE_MONTH 4U
#define ERROR_DATE_DAY 5U
#define ERROR_FORMAT 6U

void Max7219_Init(MAX7219_Config_t pMax7219, SPI_RegDef_t *pSPIx);

void DIGITx_Init(SPI_RegDef_t *pSPIx, uint16_t digit, uint8_t value, uint8_t point);

void DISPLAY_Date(SPI_RegDef_t *pSPI, uint8_t day, uint8_t month, uint16_t year);
void DISPLAY_Time(SPI_RegDef_t *pSPI, uint8_t sec, uint8_t min, uint8_t hour);

void Max7219_Init(MAX7219_Config_t pMax7219, SPI_RegDef_t *pSPIx);

void DIGITx_Init(SPI_RegDef_t *pSPIx, uint16_t digit, uint8_t value, uint8_t point);

void DISPLAY_Date(SPI_RegDef_t *pSPI, uint8_t day, uint8_t month, uint16_t year);
void DISPLAY_Time(SPI_RegDef_t *pSPI, uint8_t sec, uint8_t min, uint8_t hour);
void Toggle_Display(SPI_RegDef_t *pSPIx, uint8_t toggle);
void Display_Fault(SPI_RegDef_t *pSPIx);
void Display_Error(SPI_RegDef_t *pSPIx, uint8_t error);

#endif
