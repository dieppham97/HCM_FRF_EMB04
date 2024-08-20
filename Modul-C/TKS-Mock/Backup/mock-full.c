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


/************************************************************
*	FUNCTION TO CREATE LINKED LIST
*	Argument: 	pHead and pTail
*	Return:		
*************************************************************/
void createListEmploy (EMPLOYEE_LIST *employeeList)
{
	employeeList->pHead = NULL;
	employeeList->pTail = NULL;
} 


/************************************************************
*	FUNCTION TO CHECK WHETHER IT IS A NUMBER
*	Argument: 	Stdin
*	Return:		0: not numbers
				1: numbers.
*************************************************************/
uint32_t checkVaidNumber(uint8_t *sChoice) 
{
   for (uint32_t i = 0; i < strlen(sChoice); i++) {
        if (!isdigit(sChoice[i])) {
            return 0;
        }
    }
    return 1;
}


/************************************************************
*	FUNCTION TO CREATE NEW NODE TIME WORK
*	Argument: 	Date, Check in, Check Out
*	Return: 	Node of time work
*************************************************************/
TIMEWORK* addTimeWork(time_t date, time_t checkIn, time_t checkOut)
{
	TIMEWORK* newTimeWork = (TIMEWORK*)malloc(sizeof(TIMEWORK));
	
    newTimeWork->date = date;
    newTimeWork->checkIn = checkIn;
    newTimeWork->checkOut = checkOut;
    newTimeWork->next = NULL;
    return newTimeWork;
}


/************************************************************
*	FUNCTION TO CREATE NEW NODE EMPLOYEE
*	Argument: 	ID, Name, Basic salary
*	Return: 	Node new employee
*************************************************************/
EMPLOYEE* addNewNodeEmploy (uint32_t ID, uint8_t *sName, uint32_t basicSalary)
{
	
	EMPLOYEE* newEmployee = (EMPLOYEE*)malloc(sizeof(EMPLOYEE));
	newEmployee->ID = ID;
	strcpy(newEmployee->sName, sName);
	newEmployee->basicSalary = basicSalary;
	newEmployee->timeWork = NULL;
	newEmployee->next = NULL;
	return newEmployee;
}


/************************************************************
*	FUNCTION TO ADD NEW NODE IN HEAD OF LINKED LIST
*	Argument: 	List and new node employee
*	Return: 	
*************************************************************/
void addNodeHead (EMPLOYEE_LIST *list, EMPLOYEE *newNodeEmployee)
{
	if (list->pHead==NULL) {
        list->pHead = newNodeEmployee;
        list->pTail = newNodeEmployee;
    }else
    {
    	newNodeEmployee->next = list->pHead;
    	list->pHead = newNodeEmployee;
	}
}


/************************************************************
*	FUNCTION TO ADD NEW NODE IN TAIL OF LINKED LIST
*	Argument: 	List and new node employee
*	Return: 	
*************************************************************/
void addNodeTail (EMPLOYEE_LIST *list, EMPLOYEE *newNodeEmployee)
{
	if(list->pHead == NULL)
	{
		list->pHead = newNodeEmployee;
		list->pTail = newNodeEmployee;
	}else
	{
		list->pTail->next = newNodeEmployee;
		list->pTail = newNodeEmployee;
	}
}


/************************************************************
*	FUNCTION TO SAVE LINKED LIST TO FILE TXT
*	Argument: 	List
*	Return: 	
*************************************************************/
void saveEmloyeeToFile (EMPLOYEE_LIST *list)
{
	FILE* fEmployee = fopen("Employee.txt", "w");
	
	for (EMPLOYEE *i = list->pHead; i != NULL; i = i->next)
	{
		fprintf(fEmployee,"ID: %05d __ Name: %s__ Salary/H: %d\n", i->ID, i->sName, i->basicSalary);
		for (TIMEWORK *j = i->timeWork; j != NULL; j = j->next)
		{
			uint8_t dateStr[20], checkInStr[20], checkOutStr[20];
			strftime(dateStr, sizeof(dateStr),"%d-%m-%Y", localtime(&j->date));
			if (j->checkIn != 0) 
			{
                strftime(checkInStr, sizeof(checkInStr),"%H:%M", localtime(&j->checkIn));
            }else 
			{
                strcpy(checkInStr, "N/A");
            }
			if (j->checkOut != 0) 
			{
                strftime(checkOutStr, sizeof(checkOutStr), "%H:%M", localtime(&j->checkOut));
            }else 
			{
                strcpy(checkOutStr, "N/A");
            }
            
			fprintf(fEmployee,"\tDate: %s In: %s Out: %s", dateStr, checkInStr, checkOutStr);
			fprintf(fEmployee,"\n");
		}
	}
	fclose(fEmployee);
}


/************************************************************
*	FUNCTION TO PASES THE DATE TO STRING (DD:MM:YY)
*	Argument: 	String and struct tm (in time.h)
*	Return: 	Type time_t (epoch)
*************************************************************/
time_t parseDate (uint8_t* dateStr, struct tm *tm)
{
    if (sscanf(dateStr, "%d-%d-%d", &tm->tm_mday, &tm->tm_mon, &tm->tm_year) != 3) {
        fprintf(stderr, "Invalid date format: %s\n", dateStr);
        return (time_t)-1;
    }
    tm->tm_year -= 1900;  // Year (1900 + year)
    tm->tm_mon--;         // Month (from 0 - 11)
    //tm->tm_isdst = -1;    //Daylight Saving Time
    return mktime(tm);
}


/************************************************************
*	FUNCTION TO PASES THE TIME TO STRING (HH:MM)
*	Argument: 	String and struct tm (in time.h)
*	Return: 	Type time_t (epoch)
*************************************************************/
time_t parseTime (uint8_t* timeStr, struct tm *tm)
{
    if (strcmp(timeStr, "N/A") == 0) {
        return 0;
    }

    if (sscanf(timeStr, "%d:%d", &tm->tm_hour, &tm->tm_min) != 2) {
        fprintf(stderr, "Invalid time format: %s\n", timeStr);
        return (time_t)-1;
    }
    
    //tm->tm_isdst = -1;    //Daylight Saving Time
    return mktime(tm);
}


/************************************************************
*	FUNCTION TO LOAD INFORMATION FROM FILE AND CREATE LIST
*					WITH THAT INFORMATION
*	Argument: 	List (NULL)
*	Return: 	New list with all informatio from file
*************************************************************/
EMPLOYEE_LIST* loadEmployeeFromFile (EMPLOYEE_LIST *list)
{
	FILE *fEmployee = fopen("Employee.txt", "r");
	
	if(fEmployee == NULL)
	{
		printf("Don't found file!");
		return NULL;
	}

	createListEmploy(list);
    
    uint8_t line[256];
    while (fgets(line, sizeof(line), fEmployee) != NULL)
    {
    	// Process information of each employee
    	if (strstr(line, "ID:") != NULL && strstr(line, "Name:") != NULL && strstr(line, "Salary/H:") != NULL)
    	{
    		uint32_t ID, basicSalary;
    		uint8_t name[24];
    		
    		sscanf(line, "ID: %05d __ Name: %[^_]__ Salary/H: %d", &ID, name, &basicSalary);
    	
    		EMPLOYEE* newEmployee = addNewNodeEmploy(ID, name, basicSalary);
            addNodeTail(list, newEmployee);
            continue;
		}
		
		// Process time 
		else if(strstr(line,"Date:") != NULL && strstr(line,"In:") != NULL && strstr(line, "Out:") != NULL)
		{	
			TIMEWORK *pTailTime = list->pTail->timeWork;
			
            uint8_t dateStr[20], checkInStr[20], checkOutStr[20];
            time_t date, checkIn, checkOut;
                
            sscanf(line, "\t Date: %s In: %s Out: %s", dateStr, checkInStr, checkOutStr);
            
            struct tm tm;
            memset(&tm, 0, sizeof(struct tm));
			date = parseDate(dateStr, &tm);
            checkIn = parseTime(checkInStr, &tm);
            checkOut = parseTime(checkOutStr, &tm);
			
			TIMEWORK *newTimeWork = addTimeWork(date, checkIn, checkOut);
			
			if(list->pTail->timeWork == NULL)
			{
				list->pTail->timeWork = newTimeWork;
			}else
			{
				for (TIMEWORK *j = pTailTime; j != NULL; j = j->next)
				{
					if (j->next == NULL)
					{
						j->next = newTimeWork;
						j = newTimeWork;
					}
				}
			}
        }
    }
    
    fclose(fEmployee);
    return list;
}


/************************************************************
*	FUNCTION TO DELETE LINKED LIST AFTER USED
*	Argument: 	List
*	Return: 	
*************************************************************/
void freeMemory(EMPLOYEE_LIST *list) 
{
    for (EMPLOYEE *i = list->pHead; i != NULL;) {
        EMPLOYEE *nextEmployee = i->next;  

        for (TIMEWORK *j = i->timeWork; j != NULL;) {
            TIMEWORK *nextTimework = j->next;  
            free(j);  
            j = nextTimework;  
        }

        free(i);  
        i = nextEmployee;  
    }

    list->pHead = NULL;  
}


/******************************************************************
*	FUNCTION PASSWORD ENCRYPTION
*	Argument:	Password 
*	Return:		Result passwork already encryp
*******************************************************************/
uint8_t* encrypPassword(uint8_t *password)
{
    uint16_t len = strlen(password);
    uint8_t *result = (uint8_t *)malloc((len + 1) * sizeof(uint8_t));
    if (result == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    // Reverse characters and add 24 to each
    for (uint32_t i = 0; i < len; ++i) {
        result[i] = password[len - 1 - i] + 24;
    }
    result[len] = 0;

    return result;
}


/******************************************************************
*	FUNCTION CHECK SPACE IN STRING
*	Argument:	String 
*	Return:		1: yes
*				0: no
*******************************************************************/
uint16_t hasWhitespace(uint8_t *str) 
{
    while (*str) {
        if (*str == ' ') {
            return 1; // Tìm thấy khoảng trắng
        }
        str++;
    }
    return 0; // Không tìm thấy khoảng trắng
}


/************************************************************
*	FUNCTION TO CHANGE PASSWORD OF ADMIN
*	Argument: 
*	Return:		
*************************************************************/
void changePass()
{
	uint8_t newPass[129];
	
	do{
		printf("Input new password (No space):");
		fgets(newPass, sizeof(newPass), stdin);
		newPass[strcspn(newPass,"\n")] = '\0';
		
		if(hasWhitespace(newPass))
		{
			printf("Password have space. Incorrect format.\n Please re-input!\n");
		}else
		{
			break;
		}
	}while (1);
	
	strcpy(newPass, encrypPassword(newPass));
	
	FILE* fAdmin = fopen("Admin.txt", "r");
	FILE *temp_file = fopen("temp_file.txt", "w");
	
    if (temp_file == NULL) {
        perror("Error opening temporary file");
        fclose(fAdmin);
        return;
    }
	
	uint8_t line[129];
	if (fgets(line, sizeof(line), fAdmin) == NULL) {
        // If the file is empty, just write the new line and exit
        fputs(newPass, temp_file);
        fclose(fAdmin);
        fclose(temp_file);
        remove("temp_file.txt");
        return;
    }
    
	fputs(newPass, temp_file);
    fputs("\n", temp_file);
    
    // Copy remaining lines to the temporary file
    while (fgets(line, sizeof(line), fAdmin)) {
        fputs(line, temp_file);
    }
    
    fclose(fAdmin);
    fclose(temp_file);
    remove("Admin.txt");
	rename("temp_file.txt", "Admin.txt");

    printf("Password already change successfull.\n");

}


/******************************************************************
*	FUNCTION TO ADD AND SAVE NEW EMPLOYEE  IN FILE OF ADMIN
*	Argument:	Struct employee 
*	Return:		0: out function
*				1: success
******************************************************************/
uint8_t addNewEmployInFileAdmin (EMPLOYEE *employ)
{
	do{
		printf("Input name of new employee:");

		scanf("%[^\n]", employ->sName);
		getchar();

		if (strlen(employ->sName) > MAX_NAME) 
		{
            printf("Name too long! Please try again.\n");
            printf("(Press 0 to exit)\n");
        }
		if (strcmp(employ->sName, "0") == 0)
		{
			return 0;
		}
	}while (strlen(employ->sName) > MAX_NAME);
	
	printf("Input basic salary of new employee:");
	scanf("%d", &employ->basicSalary);
	 
	FILE *fAdmin = fopen("Admin.txt", "a+");
	uint8_t line[256];
	uint32_t lastID = 0;
	
	rewind(fAdmin);
	fgets(line, sizeof(line), fAdmin);
	
	while (fgets(line, sizeof(line), fAdmin) != NULL) {
        if (sscanf(line, "ID: %05d ", &lastID) == 1) {
            // ID successfully read, continue with the next line
            continue;
        } else {
            break;
        }
    }
    uint32_t newID = lastID + 1;
    employ->ID = newID;
    
	// Move the cursor to the end of the file to write new data
	fseek(fAdmin, 0, SEEK_END);
    // Record employee information at the end of the file
    fprintf(fAdmin, "ID: %05d __ Name: %s __ Salary/H: %d\n", employ->ID, employ->sName, employ->basicSalary);

    fclose(fAdmin);
    return 1;
}


/******************************************************************
*	FUNCTION TO VIEW INFORMATION OF ALL EMPLOYEES
*				(ID, NAME, BASIC SALARY)
*	Argument: List and specified number of working hours per month 
*	Return:	
*******************************************************************/
void printAdmin ()
{
	FILE *fAdmin = fopen("Admin.txt", "r");
	char line[256];
    if (fgets(line, sizeof(line), fAdmin) == NULL) {
        perror("Error reading file");
        fclose(fAdmin);
        return;
    }

    // Scan and print out from the second line onwards
    while (fgets(line, sizeof(line), fAdmin) != NULL) {
        printf("%s", line);
    }

    fclose(fAdmin);
}


/******************************************************************
*	FUNCTION TO VIEW WORKING TIME OF A SPECIFICED EMPLOYEE
*	Argument: List and ID of employee 
*	Return:	
*******************************************************************/
uint8_t findID (EMPLOYEE_LIST *list, uint32_t inID)
{
	for (EMPLOYEE *i = list->pHead; i != NULL; i = i->next)
	{
		if(i->ID == inID)
		{
			printf("ID: %05d, Name: %s", i->ID, i->sName);
			if (i->timeWork == NULL) 
			{
                printf("\t empty\n");
            }else
            {		
				for(TIMEWORK *j = i->timeWork; j != NULL; j = j->next)
				{
				uint8_t dateStr[20], checkInStr[20], checkOutStr[20];
				strftime(dateStr, sizeof(dateStr),"%d-%m-%Y", localtime(&j->date));
				if (j->checkIn != 0) 
				{
                	strftime(checkInStr, sizeof(checkInStr),"%H:%M", localtime(&j->checkIn));
            	}else 
				{
                	strcpy(checkInStr, "N/A");
            	}
				if (j->checkOut != 0) 
				{
                	strftime(checkOutStr, sizeof(checkOutStr), "%H:%M", localtime(&j->checkOut));
            	}else 
				{
                	strcpy(checkOutStr, "N/A");
            	}
			
				printf("\n\t Date: %s, Check in: %s, Check out: %s",dateStr, checkInStr, checkOutStr);
				}	
			}
			return 1;
		}
	}
	return 0;
}


/******************************************************************
*	FUNCTION TO CHECK VALID OF DATA
*	Argument: 	String from sdtin
*	Return:		0 OR 1
*******************************************************************/
uint8_t checkValidDate (uint8_t *date)
{
	if (strlen(date) != 5)
	{
		return 0;
	}
	if(!isdigit(date[0]) || !isdigit(date[1]) || date[2] != '/' || !isdigit(date[3]) || !isdigit(date[4]))
	{
		return 0;
	}
	uint32_t day = atoi(date);
    uint32_t month = atoi(date + 3);
    if (day < 1 || day > 31 || month < 1 || month > 12) 
    {
    	return 0;
	}
    return 1;
}


/******************************************************************
*	FUNCTION TO INPUT STRING AND CHECK VALID
*	Argument: 	String from sdtin
*	Return:		0 OR 1
*******************************************************************/
uint8_t checkStartEndDate (uint8_t *inStart, uint8_t *inEnd)
{
	do
	{
		printf("Input start date (DD/MM): ");
		scanf("%s",inStart);
		getchar();
		printf("Input end date (DD/MM): ");
		gets(inEnd);
		
		if(!checkValidDate(inStart))
		{
			printf("Start date wrong!. Please re-input!\n");
			printf("(Input 0 (both start and end) to back menu)\n");
		}else if(!checkValidDate(inEnd))
		{
			printf("End date wrong!. Please re-input!\n");
			printf("(Input 0 (both start and end) to back menu)\n");
		}else if(strcmp(inStart, "0") == 0 || strcmp(inEnd, "0") == 0)
		{
			return 0;
		}else
		{
			return 1;
		}
	}while (1);
}


/******************************************************************
*	FUNCTION TO CONVERT STRING TO TIME 
*	Argument:	String from sdtin
*	Return:		Value of time type
*******************************************************************/
time_t convertStartEndDate (uint8_t *inDate)
{
	time_t today = time(NULL);
	struct tm *date = localtime(&today);
	uint32_t day, month, year;
	sscanf(inDate,"%d/%d", &day, &month);
	date->tm_mday = day;
	date->tm_mon = month - 1;
	date->tm_hour = 0;
	date->tm_min = 0;
	date->tm_sec = 0;
	date->tm_isdst = 1;
	
	return mktime(date);
}


/******************************************************************
*	FUNCTION TO CALCULATE SALARY OF ALL EMPLOYEES
*	Argument: List and specified number of working hours per month, date
*	Return:	
*******************************************************************/
void calculateSalaries (EMPLOYEE_LIST* list, double requestedWorkingHours, time_t startDate, time_t endDate)
{
    struct tm requiredCheckIn = { 0 };
    struct tm requiredCheckOut = { 0 };
    requiredCheckIn.tm_hour = 8;
    requiredCheckOut.tm_hour = 17;
    time_t requiredCheckInTime = mktime(&requiredCheckIn);
    time_t requiredCheckOutTime = mktime(&requiredCheckOut);
    
    for (EMPLOYEE *i = list->pHead; i != NULL; i = i->next) 
	{
		printf("ID: %05d __ Name: %s\n", i->ID, i->sName);
        int32_t totalSalary = 0;
        
        
        for (TIMEWORK *j = i->timeWork; j != NULL; j = j->next) 
		{
            struct tm* dateTM = localtime(&j->date);
            uint8_t storeCheckin[6], storeCheckOut[6], storeDate[20];
            uint32_t totalPunishedMoney = 0;
            int32_t dailySalary = 0;
            uint16_t checkHalfDay = 0;
            
            strftime(storeCheckin, sizeof(storeCheckin), "%H:%M", localtime(&j->checkIn));
            uint32_t hourCheckIn = atoi(storeCheckin);
            uint32_t miCheckIn = atoi(storeCheckin + 3);

            
            strftime(storeCheckOut, sizeof(storeCheckOut),"%H:%M", localtime(&j->checkOut));
            uint32_t hourCheckOut = atoi(storeCheckOut);
            uint32_t miCheckOut = atoi(storeCheckOut + 3);

             if (j->date < startDate || j->date > endDate) 
            {
                continue;
            }
            
            strftime(storeDate, sizeof(storeDate),"%d-%m-%Y", localtime(&j->date));
            printf("\tDate: %s : ", storeDate);
            // Check if the day is Saturday or Sunday
            
            if (dateTM->tm_wday == 0 || dateTM->tm_wday == 6) 
			{
				printf("Weekend.\n");
                continue;
            }

            // Check if check-in and check-out are missing
            if (j->checkIn == 0) 
			{
				printf("Forgot check in.\n");
                continue;
            }
			if (j->checkOut == 0) 
			{
				printf("Forgot check out.\n");
                continue;
            }	

            
             // Calculate punished money
            if (hourCheckIn >= 9 || (hourCheckIn == 8 && miCheckIn > 30))
			{
				if (hourCheckIn >= 12) 
				{
					printf("half a day off in morning, ");
					checkHalfDay = 1;
                	//totalPunishedMoney += (i->basicSalary * 4);
            	}else
            	{
            		printf("staff is late, ");
                	totalPunishedMoney += 20000;
				}
			}
            if (hourCheckOut < 16 || (hourCheckOut == 16 && miCheckOut< 30))
			{
				if (hourCheckIn <= 13) 
				{
					printf("half a day off in afternoon, ");
					checkHalfDay = 1;
                	//totalPunishedMoney += (i->basicSalary * 4);
				}else
				{
					printf("staff leave early, ");
                	totalPunishedMoney += 20000;
				}
			}
			uint32_t checkInMinu = hourCheckIn * 60 + miCheckIn;
			uint32_t checkOutMinu = hourCheckOut * 60 + miCheckOut;
            uint32_t dailyWorkingHours = checkOutMinu - checkInMinu;

            // Handle lunch break time (12:00-13:00)
            uint64_t lunchBreakMinutes = 0;
            if (hourCheckIn < 12 && hourCheckOut > 13)
            {
            	lunchBreakMinutes = 60;
			} else if(hourCheckIn < 12 && hourCheckOut == 12)
			{
				lunchBreakMinutes = miCheckOut;
			} else if(hourCheckIn == 12 && hourCheckOut > 13)
			{
				lunchBreakMinutes = 60 - miCheckIn;
			}

            dailyWorkingHours -= lunchBreakMinutes;
			dailySalary = dailyWorkingHours * (i->basicSalary / 60);
 			
            if (dailyWorkingHours >= (8*60))
            {
            	dailySalary = 8 * i->basicSalary;
			}
            dailySalary -= totalPunishedMoney;
            if((checkHalfDay == 1 || totalPunishedMoney != 0) && dailyWorkingHours >= (8*60))
			{
				printf("\n\t**The employee has made up enough time.\n");
			}
            else if (totalPunishedMoney == 0 && checkHalfDay == 0)
            {
            	printf("Full day.\n");
			}
            totalSalary += dailySalary;
            printf("\n");
        }
        int32_t salaryMaximum = (requestedWorkingHours * i->basicSalary);
        // Calculate salary
        if (totalSalary > salaryMaximum) 
		{
            printf("\n\tSalary: %d VND\n\n", salaryMaximum);
			printf("***********************************************************************\n");
        }else if (totalSalary < 0)
        {
        	printf("\tSalary: 0 VND (Actually: %d VND)\n\n", totalSalary);
			printf("***********************************************************************\n");
		}else
		{
			printf("\n\tSalary: %d VND\n\n", totalSalary);
			printf("***********************************************************************\n");
		}
    }
}


/******************************************************************
*	FUNCTION TO CHECK IN BY ID
*	Argument: List and ID to check 
*	Return:	
******************************************************************/
void checkInByID (EMPLOYEE_LIST *list, uint32_t inID)
{
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    struct tm today;
    memcpy(&today, localTime, sizeof(struct tm));
    today.tm_hour = 0;
    today.tm_min = 0;
    today.tm_sec = 0;
    time_t todayTime = mktime(&today);
    TIMEWORK *newTimeWork = addTimeWork(todayTime, currentTime, 0);

    for (EMPLOYEE *i = list->pHead; i != NULL; i = i->next) {
        if (i->ID == inID) 
		{
            if (i->timeWork == NULL) 
			{
                i->timeWork = newTimeWork;
                printf("\nID: %05d has checked in successfull\n\n", i->ID);
            	return;
            } 
			else
			{
                for (TIMEWORK *j = i->timeWork; j != NULL; j = j->next)
            	{
            		struct tm *workDate = localtime(&j->date);
                	if ((today.tm_year == workDate->tm_year) && (today.tm_mon == workDate->tm_mon) && (today.tm_mday == workDate->tm_mday))
                	{
                		if(j->checkOut != 0)
                		{
                			printf("\nYou have checked out.\nPlease contact HR to modify check in.\n\n");
                    		return;
						}else if(j->checkIn != 0)
						{
							printf("\nID: %05d has already checked in.\n", i->ID);
							return;
						}
					}else
					{
						newTimeWork->next = i->timeWork;
                		i->timeWork = newTimeWork;
                		printf("\nID: %05d has checked in successfull\n\n", i->ID);
                		return;
					}
				}
            }
            return;
        }
    }
    free(newTimeWork);
    printf("Employee with ID: %05d not found!\n", inID);
}



/******************************************************************
*	FUNCTION TO CHECK OUT BY ID
*	Argument: List and ID to check
*	Return:	
******************************************************************/
void checkOutByID(EMPLOYEE_LIST *list, uint32_t inID)
{
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    struct tm today;
    memcpy(&today, localTime, sizeof(struct tm));
    today.tm_hour = 0;
    today.tm_min = 0;
    today.tm_sec = 0;
    time_t todayTime = mktime(&today);
    TIMEWORK *newTimeWork = addTimeWork(todayTime, 0, currentTime);

    for (EMPLOYEE *i = list->pHead; i != NULL; i = i->next) {
        if (i->ID == inID) 
		{
            if (i->timeWork == NULL) 
			{
                i->timeWork = newTimeWork;
                printf("\nID: %05d has checked out successfull\n\n", i->ID);
                printf("**You forgot to check in!\n");
            	return;
            }else
            {
            	for (TIMEWORK *j = i->timeWork; j != NULL; j = j->next)
            	{
            		struct tm *workDate = localtime(&j->date);
            		if ((today.tm_year == workDate->tm_year) && (today.tm_mon == workDate->tm_mon) && (today.tm_mday == workDate->tm_mday))
            		{
            			j->checkOut = currentTime;
                    	printf("ID: %05d has checked out successfully", i->ID);
                    	free(newTimeWork);
                    	return;
					}else
					{
						newTimeWork->next = i->timeWork;
                		i->timeWork = newTimeWork;
                		printf("\nID: %05d has checked out successfull\n", i->ID);
                    	printf("**You forgot to check in!\n");
                		return;
					}
				}
			}
			return;
        }
    }
    free(newTimeWork);
    printf("Employee with ID: %05d not found!\n", inID);
}


/******************************************************************
*	FUNCTION TO CHECK ID IN ADMIN FILE
*	Argument: 	ID
*	Return: 	result 0 or 1
******************************************************************/
uint8_t checkID(uint32_t inID)
{
	FILE *fAdmin = fopen("Admin.txt", "r");

	uint8_t line[256], sName[24];
	uint8_t choice;
	uint32_t ID;
	rewind(fAdmin);	
	fgets(line, sizeof(line), fAdmin);
	while (fgets(line, sizeof(line), fAdmin) != NULL)
	{
		sscanf(line, "ID: %05d __ Name: %[^_]", &ID, sName);
		if (inID == ID)
		{
			printf("\tID: %05d __ Name: %s\n", ID, sName);
			printf("\tIs this you?\n");
			
			printf("Yes / No (Y/N): ");
			do
			{
				scanf(" %c", &choice);
				getchar();
				switch (choice)
				{
					case 'y':
					case 'Y':
						fclose(fAdmin);
						return 1;
					case 'n':
					case 'N':
						fclose(fAdmin);
						return 0;
					default:
						printf("Wrong! Re-input!\n");
						break;
				}
			}while (1);
        }	
    }
    fclose(fAdmin);
    return 0;
}


/******************************************************************
*	FUNCTION ADMIN MODE
*	Argument:	
*	Return:
*******************************************************************/
void adminMode()
{
	uint32_t inID4;
	uint8_t sName[24], choice1;
	uint8_t checkValidAdChoice[20], checkValidAdChoice4[20];
	uint8_t pass[129], line[256];
	do {	
		loadEmployeeFromFile(&list);
		system("cls");
		printf("\n\n\n\t- Input '1' to change password.\n");
		printf("\t- Input '2' to add new employee.\n");
   		printf("\t- Input '3' to view information of all employee.\n");
    	printf("\t- Input '4' to View working time of a specified employee.\n");
    	printf("\t- Input '5' to calculate salary for all employees.\n");
    	printf("\t- Input '6' to back mode.\n");
    	printf("Your chosen:");
  	
    	fgets(checkValidAdChoice, sizeof(checkValidAdChoice), stdin);
		checkValidAdChoice[strcspn(checkValidAdChoice, "\n")] = 0; // Delete newline characters if any
        if(checkVaidNumber(checkValidAdChoice) == 1)
        {
        	choice1 = atoi(checkValidAdChoice);
		}else
		{
			choice1 = 0;
		}
		switch (choice1)
		{	
			case 1:
				{
					FILE *fAdmin = fopen("Admin.txt", "r");
					printf("Input old password:");
					fgets(pass, sizeof(pass), stdin);
					pass[strcspn(pass, "\n")] = '\0';
		
					strcpy(pass, encrypPassword(pass));
		
        			rewind(fAdmin);
        
        			fgets(line, sizeof(line), fAdmin);
        			line[strcspn(line, "\n")] = '\0';
        			fclose(fAdmin);
        			if(strcmp(pass, line) == 0)
					{
        				changePass();
        				return;
        			}else 
					{
            			printf("\nPassword is not correct!\n");
            			system("pause");
					}						
				break;
				}
			case 2:
				{
					EMPLOYEE employee;
					uint8_t check = addNewEmployInFileAdmin(&employee);
					if(check)
					{
						EMPLOYEE *newEmployee = addNewNodeEmploy(employee.ID, employee.sName, employee.basicSalary);
						addNodeTail(&list, newEmployee);
						printf("Staff has been added.\n");
						getchar();
						system("pause");
						break;
					}
					
					break;
				}
			
			case 3:
				printAdmin();
				printf("\n");
				system("pause");
				break;
			
			case 4:
				{
					printf("Input ID of employee: ");
					fgets(checkValidAdChoice4, sizeof(checkValidAdChoice4), stdin);
        			checkValidAdChoice4[strcspn(checkValidAdChoice4, "\n")] = 0; 
        			if(checkVaidNumber(checkValidAdChoice4) == 1)
        			{
        				inID4 = atoi(checkValidAdChoice4);
					}else
					{
						printf("Error.\n");
						system("pause");
						break;
					}
					uint8_t check;
					check = findID(&list, inID4);
					if(check == 1)
					{
						printf("\n");
						system("pause");
						break;
					}else
					{
						printf("ID not found!\n");
						system("pause");
						break;
					}
				}

			case 5:
				{
					time_t today,startDate, endDate;
					uint8_t inSart[10], inEnd[10], days, choice;
					uint8_t valid = checkStartEndDate(inSart, inEnd);
					if(valid == 1)
					{
						startDate = convertStartEndDate(inSart);
						endDate = convertStartEndDate(inEnd);
						days = (uint8_t)(difftime(endDate, startDate) / (60 *60 *24));
						if (days > 30)
						{
							printf("Number of payroll days is > 30\n");
							printf("Are you continuous?");
							printf("(Press y to calculate salary)\n");
							choice = getchar();
							if (choice == 'y' || choice =='Y')
							{
								calculateSalaries(&list, REQUEST_WORKING_HOURS_IN_MONTH, startDate, endDate);
							}else
							{
								break;
							}
						}else
						{
							calculateSalaries(&list, REQUEST_WORKING_HOURS_IN_MONTH, startDate, endDate);
						}
					}else
					{
						break;
					}
					getchar();
					system("pause");
					break;	
				}	
			case 6:
				break;			
			default:
				printf("Wrong! Please input again!\n");
				system("pause");
				break;
		}
		printf("\n");
		saveEmloyeeToFile(&list);
		freeMemory(&list);
    }while (choice1 != 6); 
}


/******************************************************************
*	FUNCTION TO CHECK FILE ADMIN
*	Argument:	
*	Return:
*******************************************************************/
void checkFileAdmin ()
{
	uint8_t newPass[128] = "123456789";
	uint8_t line[130];
	FILE *fAdmin = fopen("Admin.txt", "r");
	if (fAdmin == NULL)
	{
		printf("This is the first time it works. Please input default password!\n");
		fAdmin = fopen("Admin.txt", "w");
		strcpy(newPass, encrypPassword(newPass));
		fprintf(fAdmin,"%s", newPass);
		fclose(fAdmin);
	}else
	{
		if(fgets(line, sizeof(line),fAdmin) == 0)
		strcpy(newPass, encrypPassword(newPass));
		fprintf(fAdmin,"%s", newPass);
		fclose(fAdmin);
	}
}


/************************************************************
*	FUNCTION TO CHECK WHETHER THE EMPLOYEE FILE EXIST
*	Argument: 	
*	Return: 	
*************************************************************/
void checkFileEmployee ()
{
	FILE* fEmployee = fopen("Employee.txt", "r");
	if(fEmployee == NULL)
	{
		fEmployee = fopen("Employee.txt", "w");
		fclose(fEmployee);
	}
	fclose(fEmployee);
}


/******************************************************************
*	FUNCTION TO CHECK PASSWORD OF ADMIN MODE
*	Argument:	
*	Return:
*******************************************************************/
void checkPass()
{
	uint8_t pass[128];
	uint8_t line[50];
	uint8_t ch[3];
	checkFileAdmin();
	while (1)
	{
		FILE *fAdmin = fopen("Admin.txt", "r");
		printf("Input password:");
		fgets(pass, sizeof(pass), stdin);
		pass[strcspn(pass, "\n")] = '\0';
	
		strcpy(pass, encrypPassword(pass));
		rewind(fAdmin);
        fgets(line, sizeof(line), fAdmin);
		line[strcspn(line, "\n")] = '\0';
		fclose(fAdmin);
		if(strcmp(pass, line) == 0)
		{
			adminMode();
			break;
        }else 
		{
            printf("\nPassword is not correct!\n");
			while (1) 
			{
                printf("Do you want to re-input? (Y to re-input, N to go back): ");
                scanf("%[^\n]", ch);
                getchar();
                
                if (strcmp(ch,"n") == 0 || strcmp(ch,"N") == 0) 
				{
                    printf("Going back...\n");
					break;
                } else if (strcmp(ch, "y") == 0 || strcmp(ch,"Y") == 0) 
				{
					break;
                }else if(strlen(ch) > 1)
				{
                	printf("Invalid input! Please enter Y or N.\n");
				}
				else {
					printf("Invalid input! Please enter Y or N.\n");
                   
                }
            }
            if (strcmp(ch,"n") == 0 || strcmp(ch,"N") == 0) 
			{
				break;
            }
        }
    }
}


/******************************************************************
*	FUNCTION NORMAL MODE  
*	Argument:	
*	Return:	
******************************************************************/
void normalMode()
{
	uint8_t checkValid1[20], checkValidID[20], choice1;
	uint32_t inID, outID, check;
	do 
	{
		loadEmployeeFromFile(&list);
    	system("cls");		
		printf("\n\n\n\t- Input '1' Check In.\n");
    	printf("\t- Input '2' to Check Out.\n");
    	printf("\t- Input '3' to back menu.\n");
    	printf("Your chosen:");
    	fgets(checkValid1, sizeof(checkValid1), stdin);
        checkValid1[strcspn(checkValid1, "\n")] = 0;
        if(checkVaidNumber(checkValid1) == 1)
        {
        	choice1 = atoi(checkValid1);
		}else
		{
			choice1 = 0;
		}
		switch (choice1)
		{
			case 1:
				
				printf("Input your ID: ");
				scanf("%s", checkValidID);
				getchar();

        		if(checkVaidNumber(checkValidID) == 1)
        		{
        			inID = atoi(checkValidID);
				}else
				{	
					printf("Invalid!\n");
					system("pause");
					while (getchar() != '\n');
					break;
				}
				check = checkID(inID);
				if(check == 1)
				{
					checkInByID(&list, inID);
					saveEmloyeeToFile(&list);
				}else
				{
					printf("ID not found!\n");
				}
				system("pause");
				break;
		
			case 2:
				printf("Input your ID: ");
				scanf("%s", checkValidID);
				getchar();

        		if(checkVaidNumber(checkValidID) == 1)
        		{
        			outID = atoi(checkValidID);
				}else
				{
					printf("Invalid!\n");
					system("pause");
					while (getchar() != '\n');
					break;
				}
				
				check = checkID(outID);
				
				if(check == 1)
				{
					checkOutByID(&list, outID);
					saveEmloyeeToFile(&list);
				}else
				{
					printf("ID not found!\n");
				}
				printf("\n");
				system("pause");
				break;
			
			case 3:
				break;
				
			default:
				printf("Wrong! Please input again!\n");
				system("pause");
				break;
		}
		
		freeMemory(&list);
    } while (choice1 != 3); 
}


int main(int argc, char *argv[]) {
	
	uint8_t checkValid[20], choice;
	checkFileEmployee();

	do{
	    printf("--------------------------------------------------------------------------\n");
        printf("|                               INTRUCTIONS                              |\n");
        printf("--------------------------------------------------------------------------\n\n");
        printf("\t- Input '1': Admin Mode.\n");
        printf("\t- Input '2': Employee Mode.\n");
        printf("\t- Input '3' to quit program.\n");
        printf("Your choice: ");
		
		fgets(checkValid, sizeof(checkValid), stdin);
        checkValid[strcspn(checkValid, "\n")] = 0;
        if(checkVaidNumber(checkValid) == 1)
        {
        	choice = atoi(checkValid);
		}else
		{
			choice = 0;
		}
        switch (choice) {
            case 1:
                checkPass();
                system("pause");
                break;
                
            case 2:
				normalMode();
                break;
                
            case 3:
				printf("Good bye!\n");
                break;
            
            default:
                printf("Wrong! Please input again!\n");
                system("pause");
                break;
        }
        system("cls");
    } while (choice != 3);
	return 0;
}