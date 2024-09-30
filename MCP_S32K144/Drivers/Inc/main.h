/*
 * main.h
 *
 *  Created on: Sep 29, 2023
 *      Author: Diep Pham
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "max7219.h"
#include "TOUCH1.h"
#include "TOUCH2.h"
// #include "S32K1xx_gpio_driver.h"
// #include "S32K1xx_spi_driver.h"
#include "S32K1xx_lpit_driver.h"
#include "S32K1xx_adc_driver.h"
#include "S32K1xx_uart_driver.h"
#include "Clocks_and_Modes.h"

#define ELEC1_TOUCHED_LIMIT_VALUE 3500
#define ELEC1_DIST_CHARGE_DELAY 100
#define ELEC2_TOUCHED_LIMIT_VALUE 3000
#define ELEC2_DIST_CHARGE_DELAY 100

MAX7219_date_t cur_date, check_date;
MAX7219_time_t cur_time, check_time, alarm_time;
uint8_t DayInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint8_t SW2, SW2_Status, SW3, SW3_Status;
char rx_buf[10];
char alarm_buf[9];
uint8_t index, sys_tick_count, data_request, alarm;
uint8_t delay, IntensityTouch, Touched1, ADC_Pe;

UART_Handle_t Uart1_Handle;

/*
 * SPI1:
 *	ALT3: PTB17_PCS3,	PTB16_OUT,	PTB15_IN, PTB14_SCK
 *	CLOCK SOURCE: SOSCDIV2, SIRCDIV2, FIRCDIV2, SPLLDIV2
 *
 * UART1:
 * 	ALT2: PTC7_TX, PTC6_RXD
 * 	CLOCK SOURCE: SIRCDIV2, SIRCDIV4, SIRCDIV8, SIRCDIV16
 * 	MODE: UART_MODE_TXRX
 * 	BAUD: UART_BAUD_9600
 * 	STOP BIT: UART_STOPBITS_1
 * 	PARITY: UART_PARITY_DISABLE
 * 	WORD LENGTH: UART_WORDLEN_8BITS
 * 	OVER SAMPLING: UART_OSR16
 * 	INTERRUPT MODE: UART_ITMODE_RX
 *
 * LPIT0:
 * 	ALT3: PTC1_LPIT0_CH3
 * 	MODE: Periodic_Counter
 * 	INTERRUPT: ENABLE
 * 	CHANNEL: CHANNEL3
 * 	VALUE TIME: 1000
 */

/*******************************************************************************
 * @fn				- Clock_Init
 *
 * @brief			- This function initializes the system clock configuration
 *
 * @param[in]		- None
 *
 * @return			- None
 *
 * @Note			- This function sets up the Fast Internal Reference Clock (FIRC)
 *                    as the system clock source
 */
void Clock_Init(void)
{
  SourceClock_Config_t Clock;

  Clock.DIV = DIV2;                    // Set the clock divider to 2
  Clock.Soure = Source_FIRC;           // Select FIRC as the clock source
  Clock.FIRC_Config.FIRC_DIV2 = DIV_1; // Set FIRC divider 2 to divide by 1

  // Initialize the System Clock Generator (SCG) with the configured settings
  SCG_Init(Clock);
}

/*******************************************************************************
 * @fn				- SPI1_Init
 *
 * @brief			- This function initializes and configures the SPI1 peripheral
 *
 * @param[in]		- None
 *
 * @return			- None
 *
 * @Note			- This function sets up SPI1 for communication with external devices,
 *                    specifically configured for use with the MAX7219 LED display driver
 */
void SPI1_Init(void)
{
  /* Initialize SPI1 configuration */
  SPI_Handle_t SPI1_Handle;
  SPI1_Handle.pSPIx = SPI1;

  /* Configure SPI1 settings */
  SPI1_Handle.SPIConfig.SPI_ClockSource = PCS_FIRC;       /* Set clock source to Fast Internal Reference Clock (FIRC) */
  SPI1_Handle.SPIConfig.SPI_DeviceMode = SPI_MASTER;      /* Configure as SPI master (controller) */
  SPI1_Handle.SPIConfig.SPI_DFF = SPI_DFF_16BITS;         /* Set data frame format to 16 bits, suitable for MAX7219 */
  SPI1_Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;          /* Set clock phase to low (sample on leading edge) */
  SPI1_Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;          /* Set clock polarity to low (idle state is low) */
  SPI1_Handle.SPIConfig.SPI_LSBF = SPI_LSB_LSBF;          /* Set Least Significant Bit First mode */
  SPI1_Handle.SPIConfig.SPI_Prescale = SPI_PRESCALE_DIV2; /* Set prescaler to divide clock by 2 */
  SPI1_Handle.SPIConfig.SPI_LengTx = SPI_LENG_1W;         /* Set transfer length to 1 word (16 bits) */
  SPI1_Handle.SPIConfig.SPI_SCK = 8;                      /* Set SCK baud rate scaler to 8 */
  SPI1_Handle.SPIConfig.SPI_PCS = SPI_PCS3;               /* Set Peripheral Chip Select to PCS3 (Chip Select 3) */

  /* Initialize SPI1 with the configured settings */
  SPI_Init(&SPI1_Handle);
}

/*******************************************************************************
 * @fn				- SPI1_GPIOInit
 *
 * @brief			- This function initializes the GPIO pins for SPI1 communication
 *
 * @param[in]		- None
 *
 * @return			- None
 *
 * @Note			- This function configures GPIOB pins 14, 15, 16, and 17 for SPI1 functionality
 */
void SPI1_GPIOInit(void)
{
  GPIO_Handle_t SPI1_Gpio;

  /* Configure GPIOB for SPI1 */
  SPI1_Gpio.pGPIOx = GPIOB;
  /* Set alternate function mode to 3 for SPI1 */
  SPI1_Gpio.GPIO_PinConfig.GPIO_PinAlterFunMode = 3;

  /* SPI_OUT (MOSI - Master Out Slave In) */
  SPI1_Gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_16;
  GPIO_Init(&SPI1_Gpio);

  /* SPI_IN (MISO - Master In Slave Out) */
  SPI1_Gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
  GPIO_Init(&SPI1_Gpio);

  /* SPI_PSC3 (Chip Select) */
  SPI1_Gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_17;
  GPIO_Init(&SPI1_Gpio);

  /* SPI_SCK (Serial Clock) */
  SPI1_Gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
  GPIO_Init(&SPI1_Gpio);
}

/*******************************************************************************
 * @fn				- MAX7219_Init
 *
 * @brief			- This function initializes the MAX7219 LED display driver
 *                    with default settings
 *
 * @param[in]		- pSPIx: Pointer to the SPI peripheral used for communication
 *
 * @return			- None
 *
 * @Note			- This function configures the MAX7219 for full decode mode,
 *                    lowest intensity, all digits active, and normal operation
 */
void MAX7219_Init(SPI_RegDef_t *pSPIx)
{
  /* Configure MAX7219 settings */
  MAX7219_Config_t Max7219;

  Max7219.Decode_Mode = DECODE_FULL; /* Set full decode mode for all digits */
  Max7219.Intensity_Mode = INTEN_1;  /* Set display intensity to level 1 (lowest) */
  IntensityTouch = INTEN_1;          /* Update global variable for current intensity */
  Max7219.ScanLimit_Mode = SCAN_0_7; /* Set scan limit to display all 8 digits */
  Max7219.ShutDown_Mode = SHUTD_ON;  /* Turn on the display (normal operation) */

  /* Initialize the MAX7219 with the configured settings */
  Max7219_Init(Max7219, pSPIx); /* Call to external function to apply settings */
}

/*******************************************************************************
 * @fn				- SW2_ButtonInit
 *
 * @brief			- This function initializes and configures the GPIO pin for SW2 button
 *                    with interrupt capability
 *
 * @param[in]		- None
 *
 * @return			- None
 *
 * @Note			- This function sets up SW2 button on GPIOC pin 12 with rising edge
 *                    interrupt enabled
 */
void SW2_ButtonInit(void)
{
  GPIO_Handle_t SW2_Button;
  SW2_Button.pGPIOx = GPIOC;                                 /* Configure GPIO port C for SW2 button */
  SW2_Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12; /* Set pin number to 12 for SW2 button */
  SW2_Button.GPIO_PinConfig.GPIO_InOut = IN_PUT;             /* Configure pin as input */
  SW2_Button.GPIO_PinConfig.GPIO_PinAlterFunMode = GPIO;     /* Set alternate function mode to GPIO */
  SW2_Button.GPIO_PinConfig.GPIO_Interrupt = ENABLE;         /* Enable interrupt for this pin */
  SW2_Button.GPIO_PinConfig.GPIO_ModeIT = IT_RE;             /* Set interrupt mode to rising edge (IT_RE: Interrupt Trigger on Rising Edge) */
  GPIO_Init(&SW2_Button);                                    /* Initialize the GPIO pin with the above configuration */
}

/*******************************************************************************
 * @fn				- SW3_ButtonInit
 *
 * @brief			- This function initializes and configures the GPIO pin for SW3 button
 *                    with interrupt capability
 *
 * @param[in]		- None
 *
 * @return			- None
 *
 * @Note			- This function sets up SW3 button on GPIOC pin 13 with rising edge
 *                    interrupt enabled
 */
void SW3_ButtonInit(void)
{
  GPIO_Handle_t SW3_Button;
  SW3_Button.pGPIOx = GPIOC;                                 /* Configure GPIO port C for SW3 button */
  SW3_Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13; /* Set pin number to 13 for SW3 button */
  SW3_Button.GPIO_PinConfig.GPIO_InOut = IN_PUT;             /* Configure pin as input */
  SW3_Button.GPIO_PinConfig.GPIO_PinAlterFunMode = GPIO;     /* Set alternate function mode to GPIO */
  SW3_Button.GPIO_PinConfig.GPIO_Interrupt = ENABLE;         /* Enable interrupt for this pin */
  SW3_Button.GPIO_PinConfig.GPIO_ModeIT = IT_RE;             /* Set interrupt mode to rising edge */
  GPIO_Init(&SW3_Button);                                    /* Initialize the GPIO pin with the above configuration */
}

/*******************************************************************************
 * @fn				- Init_SysTick
 *
 * @brief			- This function initializes and configures the SysTick timer
 *                    for generating periodic interrupts
 *
 * @param[in]		- None
 *
 * @return			- None
 *
 * @Note			- The function sets up SysTick to generate an interrupt every 10ms
 *                    (100 times per second) using the processor clock
 */
void Init_SysTick(void)
{
  /* Pointer to SysTick Reload Value Register (SRVR) */
  uint32_t *pSRVR = (uint32_t *)0xE000E014;
  /* Pointer to SysTick Control and Status Register (SCSR) */
  uint32_t *pSCSR = (uint32_t *)0xE000E010;

  /* Calculation of reload value for 10ms interrupt */
  uint32_t count_value = ((SYSTICK_TIMER_CLK / 100) - 1);
  /* Note: SYSTICK_TIMER_CLK is assumed to be the processor clock frequency */

  /* Clear the value of SRVR (SysTick Reload Value Register) */
  *pSRVR &= ~(0x00FFFFFFFF);
  /* Note: This operation sets all bits of SRVR to 0 */

  /* Load the calculated value into SRVR */
  *pSRVR |= count_value;

  /* Enable SysTick and configure its settings */
  *pSCSR |= (1 << 1); /* Enable SysTick exception request */
  *pSCSR |= (1 << 2); /* Set clock source to processor clock */
  *pSCSR |= (1 << 0); /* Enable the SysTick counter */
  /* Note: These operations set specific bits in the SCSR without affecting others */
}

/*******************************************************************************
 * @fn				- USART1_Init
 *
 * @brief			- This function initializes and configures the USART1 (UART1) peripheral
 *                    for communication
 *
 * @param[in]		- None
 *
 * @return			- None
 *
 * @Note			- This function sets up UART1 for receive-only mode with specific
 *                    communication parameters and interrupt configuration
 */
void USART1_Init(void)
{
  /* Initialize UART1 configuration */
  Uart1_Handle.pUARTx = UART1;
  Uart1_Handle.USART_Config.UART_ClockSource = Source_FIRC;       /* Set clock source to Fast Internal Reference Clock (FIRC) */
  Uart1_Handle.USART_Config.UART_MODE = UART_MODE_ONLY_RX;        /* Configure UART for receive-only mode */
  Uart1_Handle.USART_Config.UART_BAUD = UART_BAUD_9600;           /* Set baud rate to 9600 bits per second */
  Uart1_Handle.USART_Config.UART_StopBit = UART_STOPBITS_1;       /* Configure for 1 stop bit */
  Uart1_Handle.USART_Config.UART_ParityBit = UART_PARITY_DISABLE; /* Disable parity bit for error checking */
  Uart1_Handle.USART_Config.UART_WordLength = UART_WORDLEN_8BITS; /* Set word length to 8 bits */
  Uart1_Handle.USART_Config.UART_OverSampling = UART_OSR16;       /* Set oversampling rate to 16 (improves noise immunity) */
  Uart1_Handle.USART_Config.UART_ITMODE = UART_ITMODE_RX;         /* Enable receive interrupt mode for asynchronous data handling */

  UART_Init(&Uart1_Handle); /* Initialize UART with the configured settings */
}

/*******************************************************************************
 * @fn              - USART1_GPIOInit
 *
 * @brief           - This function initializes the GPIO pins for UART1 communication
 *
 * @param[in]       - None
 *
 * @return          - None
 *
 * @Note            - This function configures the GPIO pins for UART1 TX (PC7) and RX (PC6)
 */
void USART1_GPIOInit(void)
{
  GPIO_Handle_t Uart1_Gpio;

  /* Configure GPIO for UART1 */
  Uart1_Gpio.pGPIOx = GPIOC;
  Uart1_Gpio.GPIO_PinConfig.GPIO_PinAlterFunMode = 2;           /* Set alternate function mode to 2 for UART */
  Uart1_Gpio.GPIO_PinConfig.GPIO_InOut = OUT_PUT;               /* Set as output for TX */
  Uart1_Gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPD_DE; /* Disable pull-up/pull-down */
  Uart1_Gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;     /* Use pin 7 for TX */
  Uart1_Gpio.GPIO_PinConfig.GPIO_Interrupt = DISABLE;           /* Disable interrupts for this pin */

  /* UART1_TX */
  GPIO_Init(&Uart1_Gpio); /* Initialize TX pin */

  /* UART1_RX */
  Uart1_Gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6; /* Use pin 6 for RX */
  GPIO_Init(&Uart1_Gpio);                                   /* Initialize RX pin */
}

/*******************************************************************************
 * @fn              - LPIT0_Init
 *
 * @brief           - This function initializes the Low Power Interrupt Timer 0 (LPIT0)
 *                    with specific configuration settings
 *
 * @param[in]       - None
 *
 * @return          - None
 *
 * @Note            - Configures LPIT0 Channel 3 for periodic interrupts every 500ms
 *                    using the Low Power Oscillator (LPO) at 128kHz as the clock source
 */
void LPIT0_Init(void)
{
  /* Initialize LPIT0 (Low Power Interrupt Timer) configuration */
  LPIT_PinConfig_t TC3;
  TC3.Source = PCS_LPO128;     /* Set source clock to LPO128 (Low Power Oscillator 128kHz) */
  TC3.Mode = Periodic_Counter; /* Set timer mode to Periodic Counter */
  TC3.Interrupt = ENABLE;      /* Enable timer interrupt */
  TC3.Channel = CHANNEL3;      /* Use Channel 3 for this timer */
  TC3.ValueTime = 500;         /* Set timer period to 500ms */

  /* Enable peripheral clock for LPIT */
  LPIT_PeriClockControl(TC3.Source, ENABLE);

  /* Initialize LPIT with the configured settings */
  LPIT_Init(TC3);

  /* Configure and enable LPIT interrupt */
  LPIT_IRQInterruptConfig(LPIT_IT3, ENABLE); /* Enable LPIT interrupt for Timer Channel 3 */
}

void Init_Peripheral(void)
{
  /* Clock Config */
  Clock_Init();

  /* SysTick Config */
  Init_SysTick();

  /* LPIT0 Config */
  LPIT0_Init();

  /* USART1 Config */
  USART1_GPIOInit();
  USART1_Init();
  UART_IRQInterruptConfig(33, ENABLE);

  /* SPI1 Config */
  SPI1_GPIOInit();
  SPI1_Init();

  /* SW2 and SW3 Config */
  SW2_ButtonInit();
  SW3_ButtonInit();
  GPIO_IRQInterruptConfig(IRQ_PORTC, ENABLE);

  /* ADC0 Config */
  ADC_Init();
  PCC_PORTC_EN();

  /* Send SPI to MAX7219 */
  SPI_PeripheralControl(SPI1, ENABLE);
  MAX7219_Init(SPI1);
  SPI_PeripheralControl(SPI1, DISABLE);
}

/*******************************************************************************
 * @fn				- Check_SW2
 *
 * @brief			- This function checks the status of SW2 and sends appropriate
 *                    shutdown commands to the SPI1 peripheral
 *
 * @param[in]		- None
 *
 * @return			- None
 *
 * @Note			- SW2_Status is assumed to be a global variable that holds
 *                    the current state of SW2
 */
void Check_SW2(void)
{
  uint16_t temp;

  if (SW2_Status == 1)
  {
    // When SW2 is pressed (active high), turn off shutdown mode
    temp = (SHUT_DOWN | SHUTD_OFF);
  }
  else // SW2_Status == 0
  {
    // When SW2 is not pressed, turn on shutdown mode
    temp = (SHUT_DOWN | SHUTD_ON);
  }

  // Send the shutdown command to SPI1
  SPI_SendData(SPI1, temp);
}

/*******************************************************************************
 * @fn				- Check_SW3
 *
 * @brief			- This function checks the status of SW3 and displays either
 *                    the current time or date on the display
 *
 * @param[in]		- None
 *
 * @return			- None
 *
 * @Note			- SW3_Status is assumed to be a global variable that holds
 *                    the current state of SW3
 */
void Check_SW3(void)
{
  // Check the status of SW3 and display either time or date accordingly
  if (SW3_Status == 0)
  {
    // When SW3 is not pressed (assuming active high), display the current time
    DISPLAY_Time(SPI1, cur_time.seconds, cur_time.minutes, cur_time.hours);
  }
  else if (SW3_Status == 1)
  {
    // When SW3 is pressed, display the current date
    DISPLAY_Date(SPI1, cur_date.day, cur_date.month, cur_date.year);
  }
}

/*******************************************************************************
 * @fn              - String_To_Time
 *
 * @brief           - This function converts a string representation of time to
 *                    numeric values and stores them in the check_time structure
 *
 * @param[in]       - rx_time: Pointer to a character array containing the time string
 *                    in the format "HH:MM:SS"
 * @param[in]       - EnOrDi: Enable or Disable flag
 *
 * @return          - None
 *
 * @Note            - The function only processes the time if EnOrDi is ENABLE
 */
void String_To_Time(char *rx_time, uint8_t EnOrDi)
{
  if (EnOrDi == ENABLE)
  {
    // Convert hours: Extract first two characters, convert to integers
    check_time.hours = ((rx_time[0] - '0') * 10 + (rx_time[1] - '0'));

    // Convert minutes: Extract characters at index 3 and 4, convert to integers
    check_time.minutes = ((rx_time[3] - '0') * 10 + (rx_time[4] - '0'));

    // Convert seconds: Extract characters at index 6 and 7, convert to integers
    check_time.seconds = ((rx_time[6] - '0') * 10 + (rx_time[7] - '0'));
  }
}

/*******************************************************************************
 * @fn              - String_To_Date
 *
 * @brief           - This function converts a string representation of date to
 *                    numeric values and stores them in the check_date structure
 *
 * @param[in]       - rx_date: Pointer to a character array containing the date string
 *                    in the format "DD/MM/YYYY"
 * @param[in]       - EnOrDi: Enable or Disable flag
 *
 * @return          - None
 *
 * @Note            - The function only processes the date if EnOrDi is ENABLE
 */
void String_To_Date(char *rx_date, uint8_t EnOrDi)
{
  if (EnOrDi == ENABLE)
  {
    // Convert day: Extract first two characters, convert to integers
    check_date.day = ((rx_date[0] - '0') * 10 + (rx_date[1] - '0'));

    // Convert month: Extract characters at index 3 and 4, convert to integers
    check_date.month = ((rx_date[3] - '0') * 10 + (rx_date[4] - '0'));

    // Convert year: Extract characters at index 6, 7, 8, and 9, convert to integers
    // This operation might be less intuitive, so here's a breakdown:
    // (rx_date[6] - '0') * 1000: Convert thousands place and multiply by 1000
    // (rx_date[7] - '0') * 100: Convert hundreds place and multiply by 100
    // (rx_date[8] - '0') * 10: Convert tens place and multiply by 10
    // (rx_date[9] - '0'): Convert ones place
    // The sum of these gives the full year
    check_date.year = ((rx_date[6] - '0') * 1000 + (rx_date[7] - '0') * 100 +
                       (rx_date[8] - '0') * 10 + (rx_date[9] - '0'));
  }
}

/*******************************************************************************
 * @fn              - Leap_Year
 *
 * @brief           - This function determines if a given year is a leap year
 *
 * @param[in]       - year: The year to check (as a 16-bit unsigned integer)
 *
 * @return          - 1 if the year is a leap year, 0 otherwise
 *
 * @Note            - A year is a leap year if it is divisible by 4 but not by 100,
 *                    or if it is divisible by 400
 */
uint8_t Leap_Year(uint16_t year)
{
  // Check if the year is a leap year
  // A year is a leap year if:
  // 1. It is divisible by 4 but not by 100, OR
  // 2. It is divisible by 400
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    return 1; // It's a leap year
  return 0;   // It's not a leap year
}

/*******************************************************************************
 * @fn              - Set_Time
 *
 * @brief           - This function updates the current time and date, handling
 *                    overflow for seconds, minutes, hours, days, months, and years.
 *                    It also accounts for leap years.
 *
 * @param[in]       - None
 *
 * @return          - None
 *
 * @Note            - This function assumes global variables cur_time and cur_date
 *                    are defined and accessible.
 */
void Set_Time(void)
{
  // Check if the current year is a leap year and update February's days accordingly
  if (Leap_Year(cur_date.year))
  {
    DayInMonth[1] = 29;
  }
  else
  {
    DayInMonth[1] = 28;
  }
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
  if (cur_date.day > DayInMonth[cur_date.month - 1])
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

/*******************************************************************************
 * @fn              - Check_ValidData
 *
 * @brief           - This function validates the format of a received string
 *                    representing either a time or date input.
 *
 * @param[in]       - rx_string: Pointer to the received string to be validated
 *
 * @return          - 1 if the string is valid, 0 otherwise
 *
 * @Note            - The function expects a 10-character string in the format:
 *                    "HH.MM.SS" or "HH-MM-SS" for time
 *                    "DD.MM.YYYY" or "DD-MM-YYYY" for date
 *                    Where H, M, S, D, Y are digits (0-9)
 */
uint8_t Check_ValidData(char *rx_string)
{
  for (uint8_t i = 0; i < 10; i++)
  {
    if (i == 2)
    {
      // Allow '.' or '-' at index 2 (separator between hours and minutes, or day and month)
      if (rx_string[i] == '.' || rx_string[i] == '-')
        continue;
    }
    else if (i == 5)
    {
      // Allow '.', '-', 'A', or 'a' at index 5
      if (rx_string[i] == '.' || rx_string[i] == '-' || rx_string[i] == 'A' || rx_string[i] == 'a')
        continue;
    }
    else if (rx_string[i] > 57 || rx_string[i] < 48)
    {
      // Check if character is not a digit (ASCII values 48-57)
      // If any character at other positions is not a digit, return 0 (invalid)
      return 0;
    }
  }
  // If all checks pass, return 1 (valid)
  return 1;
}

/*******************************************************************************
 * @fn              - Check_ReceiveData
 *
 * @brief           - This function validates and processes received data for
 *                    time, date, or alarm settings. It checks the format and
 *                    updates the corresponding variables if the input is valid.
 *
 * @param[in]       - None
 *
 * @return          - None
 *
 * @Note            - This function assumes global variables cur_time, cur_date,
 *                    check_time, check_date, alarm_time, and rx_buf are defined
 *                    and accessible.
 */
void Check_ReceiveData(void)
{
  // Validate the received data format
  if (Check_ValidData(rx_buf))
  {
    // Check for time input (format: HH-MM-SS)
    if (rx_buf[2] == '-' && rx_buf[5] == '-')
    {
      String_To_Time(rx_buf, ENABLE);
      // Validate hour, minute, and second values
      if (check_time.hours > 23)
      {
        Display_Error(SPI1, ERROR_TIME_HOURS);
      }
      else if (check_time.minutes > 59)
      {
        Display_Error(SPI1, ERROR_TIME_MINUTES);
      }
      else if (check_time.seconds > 59)
      {
        Display_Error(SPI1, ERROR_TIME_SECONDS);
      }
      else if (data_request > 8) // Check if data request is within valid range
      {
        Display_Error(SPI1, ERROR_FORMAT);
      }
      else
      {
        // Update current time if all checks pass
        cur_time.hours = check_time.hours;
        cur_time.minutes = check_time.minutes;
        cur_time.seconds = check_time.seconds;
      }
    }
    // Check for date input (format: DD.MM.YYYY)
    else if (rx_buf[2] == '.' && rx_buf[5] == '.')
    {
      String_To_Date(rx_buf, ENABLE);
      // Validate day, month values
      if (check_date.day > DayInMonth[cur_date.month - 1])
      {
        Display_Error(SPI1, ERROR_DATE_DAY);
      }
      else if (check_date.month > 12)
      {
        Display_Error(SPI1, ERROR_DATE_MONTH);
      }
      else if (data_request > 10) // Check if data request is within valid range
      {
        Display_Error(SPI1, ERROR_FORMAT);
      }
      else
      {
        // Update current date if all checks pass
        cur_date.day = check_date.day;
        cur_date.month = check_date.month;
        cur_date.year = check_date.year;
      }
    }
    // Check for alarm time input (format: HH-MMA or HH-MMa)
    else if (rx_buf[2] == '-' && (rx_buf[5] == 'A' || rx_buf[5] == 'a'))
    {
      if (data_request > 6) // Check if data request is within valid range
      {
        Display_Error(SPI1, ERROR_FORMAT);
      }
      String_To_Time(rx_buf, ENABLE);
      // Update alarm time and set alarm flag
      alarm_time.hours = check_time.hours;
      alarm_time.minutes = check_time.minutes;
      alarm = 1; // Set alarm flag
    }
  }
  else
  {
    // Display error if the input format is invalid
    Display_Error(SPI1, ERROR_FORMAT);
  }
}

void Potentiometer(void)
{
  ADC_channel_convert(12);
  while (!ADC_conversion_complete())
    ;
  ADC_Pe = ADC_Channel_Read();
}

uint8_t TouchSense1(void)
{
  uint8_t touch = 0;
  uint32_t chargeDistributionPeriodTmp = ELEC1_DIST_CHARGE_DELAY;

  // Configure touch button (and EGS) electrodes floating
  ElectrodeFloat1();

  // Distribute Electrode and Cext charge
  ChargeDistribution1();

  // Delay to distribute charge
  while (chargeDistributionPeriodTmp)
  {
    chargeDistributionPeriodTmp--;
  }

  // Start Cext voltage ADC conversion
  ADC_channel_convert(ELEC1_ADC_CHANNEL);

  // Redistribute Electrode and Cext charge
  ChargeRedistribution1();

  // Equivalent voltage digitalization
  // Wait for conversion complete flag
  while (ADC_conversion_complete() == 0)
  {
  }

  // Compare with the Touched reference.
  if (ADC_Channel_Read() < ELEC1_TOUCHED_LIMIT_VALUE)
  {
    touch = 1;
  }
  return touch;
}

#endif /* MAIN_H_ */