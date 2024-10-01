/*
 * S32K1xx_spi_driver.h
 *
 *  Created on: Sep 29, 2023
 *      Author: Diep Pham
 */

#ifndef SPI_H_
#define SPI_H_

#include "S32K1xx.h"

typedef struct
{
	uint8_t SPI_ClockSource;
	uint8_t SPI_DeviceMode;
	uint8_t SPI_PCS;
	uint8_t SPI_DFF;
	uint8_t SPI_CPHA;
	uint8_t SPI_CPOL;
	uint8_t SPI_LSBF;
	uint8_t SPI_Prescale;
	uint8_t SPI_LengTx;
	uint8_t SPI_LengRx;
	uint8_t SPI_SCK;
	uint8_t RESERVE;
} SPI_Config_t;

typedef struct
{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;
} SPI_Handle_t;

/*
 * SPI Mode
 */
#define SPI_MASTER 						1U
#define SPI_SLAVE 						0U

/*
 * SPI PCS
 */
#define SPI_PCS0 							0U
#define SPI_PCS1 							1U
#define SPI_PCS2 							2U
#define SPI_PCS3 							3U
							
/*
 * SPI DFF
 */
#define SPI_DFF_8BITS 				7U
#define SPI_DFF_16BITS 				15U

/*
 * SPI CPHA
 */
#define SPI_CPHA_HIGH 				1U
#define SPI_CPHA_LOW 					0U

/*
 * SPI CPOL
 */
#define SPI_CPOL_HIGH 				1U
#define SPI_CPOL_LOW 					0U

/*
 * SPI Prescale
 */
#define SPI_PRESCALE_DIV1 		0U
#define SPI_PRESCALE_DIV2 		1U
#define SPI_PRESCALE_DIV4 		2U
#define SPI_PRESCALE_DIV8 		3U
#define SPI_PRESCALE_DIV16 		4U
#define SPI_PRESCALE_DIV32 		5U
#define SPI_PRESCALE_DIV42 		6U
#define SPI_PRESCALE_DIV128 	7U

/*
 * SPI LSB
 */
#define SPI_LSB_MSBF 					0U
#define SPI_LSB_LSBF 					1U

/*
 * SPI Leng Word
 */
#define SPI_LENG_1W 					0U
#define SPI_LENG_2W 					1U
#define SPI_LENG_3W 					2U
#define SPI_LENG_4W 					3U

void SPI_PeriClockControl(SPI_Handle_t *pSPIHandle, uint8_t EnOrDi);
uint32_t SetSCK(SPI_Handle_t *pSPIHandle);

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t Enordi);

void SPI_SendData(SPI_RegDef_t *pSPIx, uint16_t pTxBuffer);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer);

#endif
