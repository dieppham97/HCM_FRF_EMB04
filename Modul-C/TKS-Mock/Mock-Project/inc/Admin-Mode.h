#include "main.h"



//FUNCTION PASSWORD ENCRYPTION
uint8_t* encrypPassword(const char *password);


//FUNCTION TO CHANGE PASSWORD OF ADMIN
void changePass();


//FUNCTION TO ADD AND SAVE NEW EMPLOYEE  IN FILE OF ADMIN
uint8_t addNewEmployInFileAdmin (EMPLOYEE *employ);


//FUNCTION TO VIEW INFORMATION OF ALL EMPLOYEES
void printAdmin ();


//FUNCTION TO VIEW WORKING TIME OF A SPECIFICED EMPLOYEE
uint8_t findID(EMPLOYEE_LIST *list, uint32_t inID);


//FUNCTION CHECK VALID OF DATA
uint8_t checkValidDate (uint8_t *date);


//FUNCTION INPUT STRING AND CHECK VALID
uint8_t checkStartEndDate (uint8_t *inStart, uint8_t *inEnd);


//FUNCTION CONVERT STRING TO TIME
time_t convertStartEndDate (uint8_t *inDate);


//FUNCTION TO CALCULATE SALARY OF ALL EMPLOYEES
void calculateSalaries(EMPLOYEE_LIST* list, uint32_t requestedWorkingHours, time_t startDate, time_t endDate);


//FUNCTION ADMIN MODE
void adminMode();
