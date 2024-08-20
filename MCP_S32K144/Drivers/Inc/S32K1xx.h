#ifndef _S32K1xx_H
#define _S32K1xx_H

#include <stdint.h>

#define DISABLE 0
#define ENABLE 	1
#define _vo			volatile

/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 */
#define NVIC_ISER0 							((_vo uint32_t*)0xE000E100)
#define NVIC_ISER1							((_vo uint32_t*)0xE000E104)
#define NVIC_ISER2 							((_vo uint32_t*)0xE000E108)
#define NVIC_ISER3 							((_vo uint32_t*)0xE000E10C)
#define NVIC_ISER4 							((_vo uint32_t*)0xE000E110)
#define NVIC_ISER5 							((_vo uint32_t*)0xE000E114)

/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 */
#define NVIC_ICER0							((_vo uint32_t*)0xE000E180)
#define NVIC_ICER1							((_vo uint32_t*)0xE000E184)
#define NVIC_ICER2							((_vo uint32_t*)0xE000E188)
#define NVIC_ICER3							((_vo uint32_t*)0xE000E18C)
#define NVIC_ICER4							((_vo uint32_t*)0xE000E190)
#define NVIC_ICER5							((_vo uint32_t*)0xE000E194)

/*
 * ARM Cortex Mx Processor Priority Register Address Calculation
 */

#define NVIC_PR_BASE_ADDR				((_vo uint32_t*)0xE000E400)

/*
 *	SCG Clock
 */
typedef struct
{
	_vo uint32_t 	VERID;	
	_vo uint32_t	PARAM;
//		uint32_t	RESERVED0[2];
			uint8_t 	RESERVED_0[8];
	_vo uint32_t 	CSR;
	_vo uint32_t	RCCR;
	_vo uint32_t	VCCR;
	_vo uint32_t	HCCR;
	_vo uint32_t	CLKOUTCNFG;
//		uint32_t	RESERVED1[55];
			uint8_t 	RESERVED_1[220];
	_vo uint32_t	SOSCCSR;
	_vo uint32_t	SOSCDIV;
	_vo uint32_t	SOSCCFG;
//		uint32_t	RESERVED2[61];
			uint8_t 	RESERVED_2[244];
	_vo uint32_t	SIRCCSR;
	_vo uint32_t	SIRCDIV;
	_vo uint32_t	SIRCCFG;
//		uint32_t	RESERVED3[61];
			uint8_t 	RESERVED_3[244];
	_vo uint32_t	FIRCCSR;
	_vo uint32_t	FIRCDIV;	
	_vo uint32_t	FIRCCFG;
//		uint32_t	RESERVED4[189];
			uint8_t 	RESERVED_4[756];
	_vo uint32_t	SPLLCSR;
	_vo uint32_t	SPLLDIV;	
	_vo uint32_t	SPLLCFG;		
}SCG_RegDef_t;

#define SCG_BASEADDR						0x40064000u
#define SCG											((SCG_RegDef_t*)SCG_BASEADDR)
#define SCG_CSR_SCS_MASK        0xF000000u
#define SCG_SOSCCSR_LK_MASK			0x800000u
#define SCG_SOSCCSR_SOSCVLD_MASK	0x1000000u
#define SCG_SPLLCSR_LK_MASK			0x800000u
#define SCG_SPLLCSR_SPLLVLD_MASK	0x1000000u

#define RCCR_SCS								24
#define RCCR_DIVCORE						16
#define RCCR_DIVBUS							4
#define RCCR_DIVSLOW						0

#define SOSCDIV_DIV1						0
#define SOSCDIV_DIV2						8
#define SOSCCFG_EREFS						2
#define SOSCCFG_RANGE						4
#define SCG_SOSC_EN()						(SCG->SOSCCSR |= (1 << 0))

#define SIRCDIV_DIV1						0
#define SIRCDIV_DIV2						8

#define FIRCDIV_DIV1						0
#define FIRCDIV_DIV2						8

#define SPLLDIV_DIV1						0
#define SPLLDIV_DIV2						8
#define SCG_SPLL_EN()						(SCG->SPLLCSR |= (1 << 0))
#define SCG_SPLL_DI()						(SCG->SPLLCSR &= ~(1U << 0))
#define SPLLCFG_PREDIV					8
#define SPLLCFG_MULT						16

/* Divide clock */
#define DIV_1										1
#define DIV_2										2
#define DIV_4										3
#define DIV_8										4
#define DIV_16									5
#define DIV_32									6
#define DIV_64									7

/* Select source SOSC */
#define EXTERNAL								0
#define INTERNAL								1

/* Select range SOSC */
#define SOSC_LOW								1
#define SOSC_MEDIUM							2
#define SOSC_HIGH								3

#define SOSCCSR_LK							23
#define SOSCCSR_VLD							24
#define SPLLCSR_LK							SOSCCSR_LK
#define SPLLCSR_VLD							SOSCCSR_VLD

/* PREDIV in SPLL*/
#define SPLL_PREDIV1						0
#define SPLL_PREDIV2						1
#define SPLL_PREDIV3						2
#define SPLL_PREDIV4						3
#define SPLL_PREDIV5						4
#define SPLL_PREDIV6						5
#define SPLL_PREDIV7						6
#define SPLL_PREDIV8						7

/* MULT in SPLL */
#define SPLL_MULT16							0
#define SPLL_MULT17							1
#define SPLL_MULT18							2
#define SPLL_MULT19							3
#define SPLL_MULT20							4
#define SPLL_MULT21							5
#define SPLL_MULT22							6
#define SPLL_MULT23							7
#define SPLL_MULT24							8
#define SPLL_MULT25							9
#define SPLL_MULT26							10
#define SPLL_MULT27							11
#define SPLL_MULT28							12
#define SPLL_MULT29							13
#define SPLL_MULT30							14
#define SPLL_MULT31							15
#define SPLL_MULT32							16
#define SPLL_MULT33							17
#define SPLL_MULT34							18
#define SPLL_MULT35							19
#define SPLL_MULT36							20
#define SPLL_MULT37							21
#define SPLL_MULT38							22
#define SPLL_MULT39							23
#define SPLL_MULT40							24
#define SPLL_MULT41							25
#define SPLL_MULT42							26
#define SPLL_MULT43							27
#define SPLL_MULT44							28
#define SPLL_MULT45							29
#define SPLL_MULT46							30
#define SPLL_MULT47							31

/* SCG_RCCR */
#define	SCS_OSC									1
#define	SCS_SIRC								2
#define	SCS_FIRC								3
#define	SCS_PLL									6

#define DIVCORE_1								0
#define DIVCORE_2								1
#define DIVCORE_3								2
#define DIVCORE_4								3
#define DIVCORE_5								4
#define DIVCORE_6								5
#define DIVCORE_7								6
#define DIVCORE_8								7
#define DIVCORE_9								8
#define DIVCORE_10							9
#define DIVCORE_11							10
#define DIVCORE_12							11
#define DIVCORE_13							12
#define DIVCORE_14							13
#define DIVCORE_15							14
#define DIVCORE_16							15

#define DIVBUS_1								0
#define DIVBUS_2								1
#define DIVBUS_3								2
#define DIVBUS_4								3
#define DIVBUS_5								4
#define DIVBUS_6								5
#define DIVBUS_7								6
#define DIVBUS_8								7
#define DIVBUS_9								8
#define DIVBUS_10								9
#define DIVBUS_11								10
#define DIVBUS_12								11
#define DIVBUS_13								12
#define DIVBUS_14								13
#define DIVBUS_15								14
#define DIVBUS_16								15

#define DIVSLOW_1								0
#define DIVSLOW_2								1
#define DIVSLOW_3								2
#define DIVSLOW_4								3
#define DIVSLOW_5								4
#define DIVSLOW_6								5
#define DIVSLOW_7								6
#define DIVSLOW_8								7
#define DIVSLOW_9								8





/*
 *	PCC Clock
 */

typedef struct
{
			uint32_t RESERVED[32];
	_vo uint32_t FTFC;						
	_vo uint32_t DMAMUX;					
			uint32_t RESERVED1[2];		
	_vo uint32_t FlexCAN0;				
	_vo uint32_t FlexCAN1;				
	_vo uint32_t FTM3;						
	_vo uint32_t PCC_ADC1;						
			uint32_t RESERVED2[3];
	_vo uint32_t FlexCAN2;
	_vo uint32_t LPSPI0;
	_vo uint32_t LPSPI1;
	_vo uint32_t LPSPI2;
			uint32_t RESERVED3[2];
	_vo uint32_t PDB1;
	_vo uint32_t CRC;
			uint32_t RESERVED4[3];
	_vo uint32_t PDB0;
	_vo uint32_t PCC_LPIT;
	_vo uint32_t FTM0;
	_vo uint32_t FTM1;
	_vo uint32_t FTM2;
	_vo uint32_t PCC_ADC0;
			uint32_t RESERVED5;
	_vo uint32_t RTC;
			uint32_t RESERVED6[2];
	_vo uint32_t LPTMR0;
			uint32_t RESERVED7[8];
	_vo uint32_t PCC_PORTA;
	_vo uint32_t PCC_PORTB;
	_vo uint32_t PCC_PORTC;
	_vo uint32_t PCC_PORTD;
	_vo uint32_t PCC_PORTE;
			uint32_t RESERVED8[6];
	_vo uint32_t SAI0;
	_vo uint32_t SAI1;
			uint32_t RESERVED9[4];
	_vo uint32_t FlexIO;
			uint32_t RESERVED10[6];
	_vo uint32_t EWM;
			uint32_t RESERVED11[4];
	_vo uint32_t LPI2C0;
	_vo uint32_t LPI2C1;
			uint32_t RESERVED12[2];
	_vo uint32_t LPUART0;
	_vo uint32_t LPUART1;
	_vo uint32_t LPUART2;
			uint32_t RESERVED13;
	_vo uint32_t FTM4;
	_vo uint32_t FTM5;
	_vo uint32_t FTM6;
	_vo uint32_t FTM7;
			uint32_t RESERVED14;
	_vo uint32_t CMP0;
			uint32_t RESERVED15;
	_vo uint32_t QSPI;
			uint32_t RESERVED16[2];
	_vo uint32_t ENET;
}PCC_RegDef_t;


#define PCC_BASE								0x40065000U
#define PCC											((PCC_RegDef_t*) PCC_BASE)
						

/*
* Enable Clock PORT
*/
#define PCC_PORTA_EN()					(PCC->PCC_PORTA |= (1U << 30))
#define PCC_PORTB_EN()					(PCC->PCC_PORTB |= (1U << 30))
#define PCC_PORTC_EN()					(PCC->PCC_PORTC |= (1U << 30))
#define PCC_PORTD_EN()					(PCC->PCC_PORTD |= (1U << 30))
#define PCC_PORTE_EN()					(PCC->PCC_PORTE |= (1U << 30))

/*
* Disable Clock PORT
*/
#define PCC_PORTA_DI()					(PCC->PCC_PORTA &= ~(1U << 30))
#define PCC_PORTB_DI()					(PCC->PCC_PORTB &= ~(1U << 30))
#define PCC_PORTC_DI()					(PCC->PCC_PORTC &= ~(1U << 30))
#define PCC_PORTD_DI()					(PCC->PCC_PORTD &= ~(1U << 30))
#define PCC_PORTE_DI()					(PCC->PCC_PORTE &= ~(1U << 30))

#define PORTA_BASEADDR					0x40049000U
#define PORTB_BASEADDR					0x4004A000U
#define PORTC_BASEADDR					0x4004B000U
#define PORTD_BASEADDR					0x4004C000U
#define PORTE_BASEADDR					0x4004D000U

typedef struct
{
	_vo uint32_t	PCR[32];
	_vo uint32_t	GPCLR;
	_vo uint32_t	GPCHR;
	_vo uint32_t	GICLR;
	_vo uint32_t	GICHR;
	_vo uint32_t	ISFR;
	_vo uint32_t	DFER;
	_vo uint32_t	DFCR;
	_vo uint32_t	DFWR;
}PORT_RegDef_t;

/*PORTx */
#define PORTA										((PORT_RegDef_t*) PORTA_BASEADDR)
#define PORTB										((PORT_RegDef_t*) PORTB_BASEADDR)
#define PORTC										((PORT_RegDef_t*) PORTC_BASEADDR)
#define PORTD										((PORT_RegDef_t*) PORTD_BASEADDR)
#define PORTE										((PORT_RegDef_t*) PORTE_BASEADDR)

/*
*PORT_PCRx
*/
#define PORT_PCR_PS							0U
#define PORT_PCR_PE							1U
#define PORT_PCR_PFE						4U
#define PORT_PCR_DSE						6U
#define PORT_PCR_MUX						8U
#define PORT_PCR_IRQ						16U
#define PORT_PCR_ISF						24U

#define GPIOA_BASE							0x400FF000U
#define GPIOB_BASE							0x400FF040U
#define GPIOC_BASE							0x400FF080U
#define GPIOD_BASE							0x400FF0C0U
#define GPIOE_BASE							0x400FF100U

typedef struct
{
	_vo uint32_t PDOR;						/*Port Data Output register */
	_vo uint32_t PSOR;						/*Port Set Output register */
	_vo uint32_t PCOR;						/*Port Clear Output register */
	_vo uint32_t PTOR;						/*Port Toggle Output Register */
	_vo uint32_t PDIR;						/*Port Data Input register */
	_vo uint32_t PDDR;						/*Port Data Direction Register */
	_vo uint32_t PIDR;						/*Port Input Disable Register */
}GPIO_RegDef_t;

#define GPIOA										((GPIO_RegDef_t*) GPIOA_BASE)
#define GPIOB										((GPIO_RegDef_t*) GPIOB_BASE)
#define GPIOC										((GPIO_RegDef_t*) GPIOC_BASE)
#define GPIOD										((GPIO_RegDef_t*) GPIOD_BASE)
#define GPIOE										((GPIO_RegDef_t*) GPIOE_BASE)


/*
 *	IRQ Number
 */
#define IRQ_PORTC								61
#define IRQ_PORTD								62


/*
*	SysTick
*/
typedef struct
{
	_vo uint32_t CSR;
	_vo uint32_t RVR;
	_vo uint32_t CVR;
	_vo uint32_t CALIB;
}SysTick_RegDef_t;

				
#define SYSTICK									((SysTick_RegDef_t*)0xE000E010U)
#define SYSTICK_TIMER_CLK				48000000U

#define SYSTICK_CSR_EN					0
#define SYSTICK_CSR_TICKINT			1
#define SYSTICK_CSR_CLKSOURCE		2

/*
 *	LPIT
 */
typedef struct
{
	_vo uint32_t VERID;
	_vo uint32_t PARAM;
	_vo uint32_t MCR;
	_vo uint32_t MSR;
	_vo uint32_t MIER;
	_vo uint32_t SETTEN;
	_vo uint32_t CLRTEN;
			uint32_t RESERVED1;
	_vo uint32_t TVAL0;
	_vo uint32_t CVAL0;
	_vo uint32_t TCTRL0;
			uint32_t RESERVED2;
	_vo uint32_t TVAL1;
	_vo uint32_t CVAL1;
	_vo uint32_t TCTRL1;
			uint32_t RESERVED3;
	_vo uint32_t TVAL2;
	_vo uint32_t CVAL2;
	_vo uint32_t TCTRL2;
			uint32_t RESERVED4;
	_vo uint32_t TVAL3;
	_vo uint32_t CVAL3;
	_vo uint32_t TCTRL3;	
}LPIT_RegDef_t;

#define LPIT										((LPIT_RegDef_t*)0x40037000)

/*	Module PCS */
#define PCC_LPIT_Clock1()				(PCC->PCC_LPIT |= (1U << 24))
#define PCC_LPIT_Clock2()				(PCC->PCC_LPIT |= (2U << 24))
#define PCC_LPIT_Clock3()				(PCC->PCC_LPIT |= (3U << 24))
#define PCC_LPIT_Clock4()				(PCC->PCC_LPIT |= (4U << 24))
#define PCC_LPIT_Clock5()				(PCC->PCC_LPIT |= (5U << 24))
#define PCC_LPIT_Clock6()				(PCC->PCC_LPIT |= (6U << 24))
#define PCC_LPIT_Clock7()				(PCC->PCC_LPIT |= (7U << 24))

/* Enable/Disable Clock LPIT */
#define PCC_LPIT_EN()						(PCC->PCC_LPIT |= (1U << 30))
#define PCC_LPIT_DI()						(PCC->PCC_LPIT &= ~(1U << 30))

/* Module Control Register */
#define LPIT_MCR								0
#define LPIT_SW_RST							1
#define LPIT_DOZE_EN						2
#define LPIT_DBG_EN							3

/* Timer control Register*/
#define TCR_EN									0
#define TCR_CHAIN								1
#define TCR_MODE								2
#define TCR_TOST								16
#define TCR_TOSI								17
#define TCR_TROT								18
#define TCR_TRG_SRC							23
#define TRG_SEL									24

/* IRQ_Interrupt of LPIT */
#define LPIT_IT0								48
#define LPIT_IT1								49
#define LPIT_IT2								50
#define LPIT_IT3								51


/*
 *	ADC
 */
typedef struct
{
	_vo uint32_t SC1[16];
	_vo uint32_t CFG1;
	_vo uint32_t CFG2;
	_vo uint32_t R[16];
	_vo uint32_t CV[2];
	_vo uint32_t SC2;
	_vo uint32_t SC3;
	_vo uint32_t BASE_OFS;
	_vo uint32_t OFS;
	_vo uint32_t USR_OFS;
	_vo uint32_t XOFS;
	_vo uint32_t YOFS;
	_vo uint32_t G;
	_vo uint32_t UG;
	_vo uint32_t CLPS;
	_vo uint32_t CLP3;
	_vo uint32_t CLP2;
	_vo uint32_t CLP1;
	_vo uint32_t CLP0;
	_vo uint32_t CLPX;
	_vo uint32_t CLP9;
	_vo uint32_t CLPS_OFS;
	_vo uint32_t CLP3_OFS;
	_vo uint32_t CLP2_OFS;
	_vo uint32_t CLP1_OFS;
	_vo uint32_t CLP0_OFS;
	_vo uint32_t CLPX_OFS;
	_vo uint32_t CLP9_OFS;
			uint32_t RESERVED[7];
	_vo uint32_t aSC1[16];
	_vo uint32_t SC1_c[16];
	_vo uint32_t aR[16];
	_vo uint32_t R_c[16];
	
}ADC_RegDef_t;

#define ADC0_BASEADDR						0x4003B000U
#define ADC1_BASEADDR						0x40027000U
#define ADC0										((ADC_RegDef_t*) ADC0_BASEADDR)
#define ADC1										((ADC_RegDef_t*) ADC1_BASEADDR)

/*
* Enable Clock ADC
*/
#define PCC_ADC0_EN()						(PCC->PCC_ADC0 |= (1U << 30))
#define PCC_ADC1_EN()						(PCC->PCC_ADC1 |= (1U << 30))

/*
*	Disable Clock ADC
*/
#define PCC_ADC0_DI()						(PCC->PCC_ADC0 &= ~(1U << 30))
#define PCC_ADC1_DI()						(PCC->PCC_ADC1 &= ~(1U << 30))

/*	Module PCS */
#define PCC_ADC0_SOSCCLK()			(PCC->PCC_ADC0 |= (1U << 24))
#define PCC_ADC0_SIRCCLK()			(PCC->PCC_ADC0 |= (2U << 24))
#define PCC_ADC0_FIRCCLK()			(PCC->PCC_ADC0 |= (3U << 24))
#define PCC_ADC0_SPLLCLK()			(PCC->PCC_ADC0 |= (6U << 24))


typedef struct {
  _vo uint32_t CS; 							/**< Watchdog Control and Status Register, offset: 0x0 */
  _vo uint32_t CNT;							/**< Watchdog Counter Register, offset: 0x4 */
  _vo uint32_t TOVAL;						/**< Watchdog Timeout Value Register, offset: 0x8 */
  _vo uint32_t WIN;							/**< Watchdog Window Register, offset: 0xC */
} WDOG_RegDef_t;

#define WDOG_BASE								(0x40052000u)
#define WDOG										((WDOG_RegDef_t *)WDOG_BASE)


#include "gpio.h"
#include "LPIT.h"
#include "ClockADC.h"

#endif
