/*
 * 017rtc_lcd.c
 *
 *  Created on: May 6, 2024
 *      Author: diepp
 */


#include <stdio.h>
#include "ds1307.h"

#define SYSTICK_TIM_CLK	16000000UL

void init_systick_timer(uint32_t tick_hz)
{
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;

    /* calculation of reload value */
    uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz)-1;	//if u need 1 interrupt every one second then the tick_hz = 1
    													//seem u need 10 interrupt every one second then the tick_hz = 10

    //Clear the value of SVR
    *pSRVR &= ~(0x00FFFFFFFF);

    //load the value in to SVR
    *pSRVR |= count_value;

    //do some settings
    *pSCSR |= ( 1 << 1); //Enables SysTick exception request:
    *pSCSR |= ( 1 << 2);  //Indicates the clock source, processor clock source

    //enable the systick
    *pSCSR |= ( 1 << 0); //enables the counter

}

char* get_day_of_week(uint8_t day)
{
	char* days[] = {"Sunday", "Monday","Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	return days[day-1];
}

void number_to_string(uint8_t num, char* buf)
{
	if (num < 10)
	{
		buf[0] = '0';
		buf[1] = num + 48;			//base on ascii
	}else
	{
		buf[0] = (num / 10) + 48;
		buf[1] = (num % 10) + 48;
	}
}



//hh:mm:ss
char* time_to_string(RTC_time_t *current_time)
{
	static char buf[9];

	buf[2] = ':';
	buf[5] = ':';

	number_to_string(current_time->hours, buf);
	number_to_string(current_time->minutes, &buf[3]);
	number_to_string(current_time->seconds, &buf[6]);

	buf[8] = '\0';

	return buf;
}
//dd/mm/yy
char* date_to_string(RTC_date_t *current_date)
{
	static char buf[9];

	buf[2] = '/';
	buf[5] = '/';

	number_to_string(current_date->day, buf);
	number_to_string(current_date->month, &buf[3]);
	number_to_string(current_date->year, &buf[6]);

	buf[8] = '\0';

	return buf;
}



int main(void)
{

	RTC_date_t current_date;
	RTC_time_t current_time;

	printf("RTC test\n");

	if(ds1307_init())
	{
		printf("RTC init has failed\n");
		while(1);
	}

	init_systick_timer(1);				//1 interrupt for every 1 second

	current_date.day 	= TUESDAY;
	current_date.date 	= 07;
	current_date.month 	= 05;
	current_date.year	= 24;

	current_time.hours 	 = 4;
	current_time.minutes = 25;
	current_time.seconds = 41;
	current_time.time_format = TIME_FORMAT_12HRS_PM;

	ds1307_set_current_date(&current_date);
	ds1307_set_current_time(&current_time);

	ds1307_get_current_date(&current_date);
	ds1307_get_current_time(&current_time);


	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS)
	{
		am_pm = (current_time.time_format) ? "PM" : "AM";
		printf("Current time = %s %s\n", time_to_string(&current_time), am_pm);
	}else
	{
		printf("Current time = %s\n", time_to_string(&current_time));
	}

	printf("Current date = %s <%s>\n", date_to_string(&current_date), get_day_of_week(current_date.day));

	while(1);
	return 0;
}






void SysTick_Handler(void)
{
	RTC_date_t current_date;
	RTC_time_t current_time;

	ds1307_get_current_time(&current_time);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS)
	{
		am_pm = (current_time.time_format) ? "PM" : "AM";
		printf("Current time = %s %s\n", time_to_string(&current_time), am_pm);
	}else
	{
		printf("Current time = %s\n", time_to_string(&current_time));
	}

	ds1307_get_current_date(&current_date);
	printf("Current date = %s <%s>\n", date_to_string(&current_date), get_day_of_week(current_date.day));
}
