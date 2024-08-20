#include "main.h"



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
            return 1; 
        }
        str++;
    }
    return 0;
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
				else 
				{
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