/*
 * can.h
 *
 *  Created on: Dec 20, 2024
 *      Author: diepp
 */

#ifndef HEADER_CAN_H_
#define HEADER_CAN_H_

#define RCC_BASEADDR (0x40023800U)
#define TIME4_BASEADDR 0x40000800U
#define GPIOD_BASEADDR 0x40020C00U
#define GPIOA_BASEADDR 0x40020000U
#define CAN1_BASEADDR (0x40006400U)
#define NVIC_ISER0 ((_vo uint32_t *)0xE000E100)
#define _vo volatile

typedef struct
{
	_vo uint32_t MODER;		// GPIO port mode register						Address offset: 0x00
	_vo uint32_t OTYPER;	// GPIO port output type register 				Address offset: 0x04
	_vo uint32_t OSPEEDR; // GPIO port output speed register				Address offset: 0x08
	_vo uint32_t PUPDR;		// GPIO port pull-up/pull-down register 			Address offset: 0x0C
	_vo uint32_t IDR;			// GPIO port input data register					Address offset: 0x10
	_vo uint32_t ODR;			// GPIO port output data register 				Address offset: 0x14
	_vo uint32_t BSRR;		// GPIO port bit set/reset register				Address offset: 0x18
	_vo uint32_t LCKR;		// GPIO port configuration lock register			Address offset: 0x1C
	_vo uint32_t AFR[2];	// AFR[0]:GPIO alternate function low register	Address offset: 0x20
												// AFR[1]:GPIO alternate function high register	Address offset: 0x24
} GPIO_RegDef_t;

#define GPIOD ((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOA ((GPIO_RegDef_t *)GPIOA_BASEADDR)

typedef struct
{
	_vo uint32_t CR1;		// 0x00
	_vo uint32_t CR2;		// 0x04
	_vo uint32_t SMCR;	// 0x08
	_vo uint32_t DIER;	// 0x0C
	_vo uint32_t SR;		// 0x10
	_vo uint32_t EGR;		// 0x14
	_vo uint32_t CCMR1; // 0x18
	_vo uint32_t CCMR2; // 0x1C
	_vo uint32_t CCER;	// 0x20
	_vo uint32_t CNT;		// 0x24
	_vo uint32_t PSC;		// 0x28
	_vo uint32_t ARR;		// 0x2C
	_vo uint32_t Reserved;
	_vo uint32_t CCR1; // 0x34
	_vo uint32_t CCR2; // 0x38
	_vo uint32_t CCR3; // 0x3C
	_vo uint32_t CCR4; // 0x40
	_vo uint32_t Reserved1;
	_vo uint32_t DCR;	 // 0x48
	_vo uint32_t DMAR; // 0x4c
} TIM_RegDef_t;

#define TIM4 ((TIM_RegDef_t *)TIME4_BASEADDR)

typedef struct
{
	/* data */
	_vo uint32_t FR1;
	_vo uint32_t FR2;
} CAN_FilterBank;

typedef struct
{
	_vo uint32_t MCR;
	_vo uint32_t MSR;
	_vo uint32_t TSR;
	_vo uint32_t RF0R;
	_vo uint32_t RF1R;
	_vo uint32_t IER;
	_vo uint32_t ESR;
	_vo uint32_t BTR;
	_vo uint32_t Reserved[88];
	_vo uint32_t TI0R;
	_vo uint32_t TDT0R;
	_vo uint32_t TDL0R;
	_vo uint32_t TDH0R;
	_vo uint32_t TI1R;
	_vo uint32_t TDT1R;
	_vo uint32_t TDL1R;
	_vo uint32_t TDH1R;
	_vo uint32_t TI2R;
	_vo uint32_t TDT2R;
	_vo uint32_t TDL2R;
	_vo uint32_t TDH2R;
	_vo uint32_t RI0R;
	_vo uint32_t RDT0R;
	_vo uint32_t RDL0R;
	_vo uint32_t RDH0R;
	_vo uint32_t RI1R;
	_vo uint32_t RDT1R;
	_vo uint32_t RDL1R;
	_vo uint32_t RDH1R;
	_vo uint32_t Reserved1[12];
	_vo uint32_t FMR;
	_vo uint32_t FM1R;
	_vo uint32_t Reserved2;
	_vo uint32_t FS1R;
	_vo uint32_t Reserved3;
	_vo uint32_t FFA1R;
	_vo uint32_t Reserved4;
	_vo uint32_t FA1R;
	_vo uint32_t Reserved5[8];
	CAN_FilterBank FxRi[28];

} CAN_RegDef_t;
#define CAN1 ((CAN_RegDef_t *)CAN1_BASEADDR)

#endif /* HEADER_CAN_H_ */
