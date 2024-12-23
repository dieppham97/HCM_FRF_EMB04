/*
 * stm32f407xx_rcc_driver.c
 *
 *  Created on: May 3, 2024
 *      Author: diepp
 */


#include <stm32f407xx_rcc_driver.h>


uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint16_t APB1_PreScaler[4] = {2,4,8,16};

uint32_t RCC_GetPLLOutputClock()
{
	return 0;
}



uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1, SystemClk;

	uint8_t clksrc, temp,ahbp, apb1;

	clksrc = (RCC->CFGR >> 2) & 0x3;				//bring 2 bits to the LSB position and mask

	if(clksrc == 0)
	{
		SystemClk = 16000000;	//HSI=16MHz

	}else if(clksrc == 1)
	{
		SystemClk = 8000000;	//HSE=8MHz
	}else if(clksrc == 2)
	{
		SystemClk = RCC_GetPLLOutputClock();		//don't use this case in course
	}

	//for ahb
	temp = (RCC->CFGR >> 4) & 0xF;				//take the value of 4 bit first

	if(temp < 8)
	{
		ahbp = 1;
	}else
	{
		ahbp = AHB_PreScaler[temp-8];
	}

	//for abp1
	temp = (RCC->CFGR >> 10) & 0x7;				//take the value of 4 bit first

	if(temp < 4)
	{
		apb1 = 1;
	}else
	{
		apb1 = APB1_PreScaler[temp-4];
	}

	pclk1 = ((SystemClk / ahbp) / apb1) ;		//?

	return pclk1;
}


uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pclk2, SystemClk = 0;

	uint8_t clksrc, temp, ahbp, apb2;

	clksrc = (RCC->CFGR >> 2) & 0x3;
	if (clksrc == 0)
	{
		SystemClk = 16000000;
	}else if (clksrc == 1)
	{
		SystemClk = 8000000;
	}else if (clksrc == 2)
	{
		SystemClk = RCC_GetPLLOutputClock();
	}

	//for ahb
	temp = (RCC->CFGR >> 4) & 0xF;
	if (temp < 8)
	{
		ahbp = 1;
	}else
	{
		ahbp = AHB_PreScaler[temp - 8];
	}

	//for abp2
	temp = (RCC->CFGR >> 13) & 0x7;
	if(temp < 4)
	{
		apb2 = 1;
	}else
	{
		apb2 = APB1_PreScaler[temp - 4];
	}
	pclk2 = ((SystemClk / ahbp) / apb2);
	return pclk2;
}




