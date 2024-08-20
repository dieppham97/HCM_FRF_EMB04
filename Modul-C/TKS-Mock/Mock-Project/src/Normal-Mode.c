#include "Normal-Mode.h"


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


/**************************************************
*	FUNCTION TO CHECK IN BY ID
*	Argument: List and ID to check 
*	Return:	
**************************************************/
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
					break;
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
					break;
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
