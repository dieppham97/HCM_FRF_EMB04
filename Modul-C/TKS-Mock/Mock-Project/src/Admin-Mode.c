#include "Admin-Mode.h"



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

