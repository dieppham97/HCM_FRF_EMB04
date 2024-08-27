#ifndef CLOCK_H_
#define CLOCK_H_

#include "S32K1xx.h"


typedef	struct 
{
	uint8_t SOSC_DIV1;
	uint8_t SOSC_DIV2;
	uint8_t SOSC_Range;
	uint8_t SOSC_ExOrX;
} SOSC_Config_t;

typedef struct {
	uint8_t SIRC_DIV1;
	uint8_t SIRC_DIV2;
	uint8_t SIRC_Range;
} SIRC_Config_t;

typedef struct {
	uint8_t FIRC_DIV1;
	uint8_t FIRC_DIV2;
} FIRC_Config_t;

typedef struct {
	uint8_t SPLL_Source;
	uint8_t SPLL_DIV1;
	uint8_t SPLL_DIV2;
	uint8_t SPLL_MULT;
	uint8_t SPLL_PreDiv;
	
} SPLL_Config_t;

typedef struct
{
	uint8_t DIV;
	uint8_t Soure;
	SOSC_Config_t	SOSC_Config;
	SIRC_Config_t	SIRC_Config;
	FIRC_Config_t	FIRC_Config;
	SPLL_Config_t SPLL_Config;
}SourceClock_Config_t;

#define DIV1									0
#define DIV2									1

#define Source_SOSC								1
#define Source_SIRC								2
#define Source_FIRC								3
#define Source_SPLL								4

/* DIV */
#define DIV_1									1
#define DIV_2									2
#define DIV_4									3
#define DIV_8									4
#define DIV_16									5
#define DIV_32									6
#define DIV_64									7

/* SOSC_CFG Range */
#define SOSC_RANGE_MEDIUM							2
#define SOSC_RANGE_HIGH								3

/* SOSC Source */
#define SOSC_EXTERNAL								0
#define SOSC_INTERNAL								1

/* SIRC_CFR Range */

#define SIRC_RANGE_2M								0
#define SIRC_RANGE_8M								1

/* SPLL_CFR MULT */
#define SPLL_MULT16								0
#define SPLL_MULT17								1
#define SPLL_MULT18								2
#define SPLL_MULT19								3
#define SPLL_MULT20								4
#define SPLL_MULT21								5
#define SPLL_MULT22								6
#define SPLL_MULT23								7
#define SPLL_MULT24								8
#define SPLL_MULT25								9
#define SPLL_MULT26								10
#define SPLL_MULT27								11
#define SPLL_MULT28								12
#define SPLL_MULT29								13
#define SPLL_MULT30								14
#define SPLL_MULT31								15
#define SPLL_MULT32								16
#define SPLL_MULT33								17
#define SPLL_MULT34								18
#define SPLL_MULT35								19
#define SPLL_MULT36								20
#define SPLL_MULT37								21
#define SPLL_MULT38								22
#define SPLL_MULT39								23
#define SPLL_MULT40								24
#define SPLL_MULT41								25
#define SPLL_MULT42								26
#define SPLL_MULT43								27
#define SPLL_MULT44								28
#define SPLL_MULT45								29
#define SPLL_MULT46								30
#define SPLL_MULT47								31

/* SPLL_CFR PREDIV */
#define SPLL_PREDIV1								0
#define SPLL_PREDIV2								1
#define SPLL_PREDIV3								2
#define SPLL_PREDIV4								3
#define SPLL_PREDIV5								4
#define SPLL_PREDIV6								5
#define SPLL_PREDIV7								6
#define SPLL_PREDIV8								7

/* SPLL_CFR SOURCE */
#define SPLL_SOURCE_SOSC							0
#define SPLL_SOURCE_FIRC							1

#define SOSC_8M									8000000
#define SIRC_8M									8000000
#define SIRC_2M									2000000
#define FIRC_48M								48000000
#define SPLL_SOSC_8M								8000000
#define SPLL_FIRC_48M								48000000

void SOSC_Init (SOSC_Config_t SOSC_Handle, uint8_t DIV);
void SIRC_Init (SIRC_Config_t SIRC_Handle, uint8_t DIV);
void FIRC_Init (FIRC_Config_t FIRC_Handle, uint8_t DIV);
void SPLL_Init (SPLL_Config_t SPLL_Handle, uint8_t DIV);
void SCG_Init  (SourceClock_Config_t Source_Handle);

void SourceSOSC_8Mhz (void);
void SourcePLL (void);
void RunNormalMode_80MHz(void);

#endif
