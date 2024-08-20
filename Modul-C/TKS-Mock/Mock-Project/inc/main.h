#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define REQUEST_WORKING_HOURS_IN_MONTH 	184
#define MAX_NAME						25

typedef struct timeWork
{
	time_t			date;
	time_t 			checkIn;
	time_t 			checkOut;
	struct timeWork	*next;
}TIMEWORK;

typedef struct Employee
{
	uint32_t	 	ID;
	uint8_t			sName[MAX_NAME];
	uint32_t		basicSalary;
	TIMEWORK		*timeWork;	
	struct Employee	*next;
}EMPLOYEE;

typedef struct
{
	EMPLOYEE *pHead;
	EMPLOYEE *pTail;
}EMPLOYEE_LIST;


// CREATE EMPLOYEE LIST MANAGED BY POINTER HEAD AND TAIL
EMPLOYEE_LIST list;


// FUNCTION TO CREATE LINKED LIST
void createListEmploy (EMPLOYEE_LIST *employeeList);


// FUNCTION TO CHECK WHETHER IT IS A NUMBER
uint32_t checkVaidNumber (uint8_t* str);


//FUNCTION CHECK SPACE IN STRING
uint16_t hasWhitespace (uint8_t *str);

// FUNCTION TO CREATE NEW NODE TIME WORK
TIMEWORK* addTimeWork (time_t date, time_t checkIn, time_t checkOut);


// FUNCTION TO CREATE NEW NODE TIME WORK
EMPLOYEE* addNewNodeEmploy (uint32_t ID, uint8_t *sName, uint32_t basicSalary);


// FUNCTION TO CREATE NEW NODE EMPLOYEE
void addNodeHead (EMPLOYEE_LIST *list, EMPLOYEE *n);

// FUNCTION TO ADD NEW NODE IN HEAD OF LINKED LIST
void addNodeTail (EMPLOYEE_LIST *list, EMPLOYEE *n);


// FUNCTION TO SAVE LINKED LIST TO FILE TXT
void saveEmloyeeToFile (EMPLOYEE_LIST *list);


// FUNCTION TO PASES THE DATE TO STRING (DD:MM:YY)
time_t parseDate (const char* dateStr,struct tm *tm);


// FUNCTION TO PASES THE TIME TO STRING (HH:MM)
time_t parseTime (const char* timeStr, struct tm *tm);


// FUNCTION TO LOAD INFORMATION FROM FILE AND CREATE LIST WITH THAT INFORMATION
EMPLOYEE_LIST* loadEmployeeFromFile (EMPLOYEE_LIST *list);


// FUNCTION TO DELETE LINKED LIST AFTER USED
void freeMemory (EMPLOYEE_LIST *list);


// FUNCTION TO CHECK FILE ADMIN
void checkFileEmployee ();


// FUNCTION TO CHECK WHETHER THE EMPLOYEE FILE EXIST
void checkFileAdmin ();


//FUNCTION TO CHECK PASSWORD OF ADMIN MODE
void checkPass ();



#include "Normal-Mode.h"
#include "Admin-Mode.h"