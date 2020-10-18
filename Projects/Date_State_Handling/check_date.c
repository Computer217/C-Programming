#include <stdio.h>
#include <ctype.h>

#define     MIN_YEAR    1600
#define     MAX_YEAR    2399
#define     YEAR_OFFSET 2000

#define     RV_NODATE   0
#define     RV_VALIDDATE 1
#define     RV_INVALIDDATE 2

#define 	STATE_S	3
#define		M_1	4
#define		M_2 5
#define		D_0 6
#define		D_1 7
#define		D_2 8
#define		Y_0 9
#define		Y_1 10
#define		Y_2 11
#define		Y_3 12
#define 	Y_4 13
#define		STATE_OK 14
#define 	ERROR 15


/***** DFA *****/

/* The function checks if a date is present at the beginning of a line,
 * and if the date is valid.
 *
 * Return values:
 *
 * 0:  Not date was found.
 * 1:  Date is valid.
 * 2:  Date was found, but not valid.
 */
int get_date() 
{
	//printf("Enter character: ");
    int date = getchar();
    int state = STATE_S;
    int mm = 0, dd = 0, yy = 0;
    
    while (date != '\n' &&  date != EOF){
    	switch(state) {
    		case STATE_S:
    			// possible inputs for initial state
    			if (isdigit(date))
    			{
    				mm = date - '0';//M_1
    				state = M_1;
    				break;
    			}
    			else if (isblank(date))
    			{
    				state = STATE_S;
    				break;
    			}
    			else
    			{
    				state = ERROR;
    			}
    		case M_1:
    			// possible inputs for M_1 state
    			if (isdigit(date))
    			{
    				mm = mm*10 + date - '0';//M_1
    				state = M_2;
    				break;
    			}
    			else if (date == 47) //sllash
    			{
    				state = D_0;
    				break;
    			}
    			else
    			{
    				state = ERROR;
    			}
    		case M_2:
    			// possible inputs for M_1 state
    			if (date == 47) //sllash
    			{
    				state = D_0;
    				break;
    			}
    			else
    			{
    				state = ERROR;
    				break;
    			}
    		case D_0: 
				if (isdigit(date)) 
				{
					dd = date - '0'; //d1
					state = D_1;
					break;
				}
				else 
				{
					state = ERROR;
					break;
				}
			case D_1: 
				if (isdigit(date)) 
				{
					dd = dd * 10 + date - '0'; //d2
					state = D_2;
					break;
				}
				else if (date == '/')
				{
					state = Y_0;
					break;
				}
				else 
				{
					state = ERROR;
					break;
				}
			case D_2: // need to fix
				if (date == '/') 
				{
					state = Y_0;
					break;
				}
				else 
				{
					state = ERROR;
					break;
				}
			case Y_0: 
				if (isdigit(date))
				{
					state = Y_1;
					yy = date - '0'; 
					break;
				}
				else
				{
					state = ERROR;
					break;
				}
			case Y_1: 
				if (isdigit(date))
				{
					state = Y_2;
					yy = yy * 10 + date - '0'; //Y2
					break;
				}
				else //might need elif
				{
					state = ERROR;
					break;
				}
			case Y_2: // 
				if (isdigit(date))
				{
					state = Y_3;
					yy = yy * 10 + date - '0'; //Y3
					break;
				}
				if (isblank(date) || date == '\n' || date == EOF)
					{
						state = STATE_OK;
						yy += 2000;
						break;
					}
				else
				{
					state = ERROR;
					break;
				}
			case Y_3: 
				if (isdigit(date))
				{
					state = Y_4;
					yy = yy * 10 + date - '0'; // Y4
					break;
				}
				else 
				{
					state = ERROR;
					break;
				}
			case Y_4: //
				if (isblank(date) || date == 10)
				{
					state = STATE_OK;
					break;
				}
				else 
				{
					state = ERROR;
					break;
				}
				
			case STATE_OK:
				break;
    	}
    	date = getchar();
    }
    
    //printf("excecuted");
    // NO DATE wrong format
    //invalid correct format but not possible 
    
    //printf("%d", state);
 
    
    
    
    
    
    
    if (state == ERROR || state == STATE_S)
    {
		return RV_NODATE;
    }
	if (state == Y_2)
	{
		yy += 2000;
	}
	if ((yy >= 1600) && (yy <= 2399))
	{
		if ((mm >= 1) && (mm <= 12))
		{
			if ((state == Y_2) || (state == Y_4) || (state == STATE_OK))
			{
				if (((dd >= 1) && (dd <= 30)) && ((mm == 4) || (mm == 6) || (mm == 9) || (mm == 11)))
				{
					return RV_VALIDDATE;
				}
				else if (((dd >= 1) && (dd <=31)) && ((mm == 1) || (mm == 3) || (mm == 5) || (mm == 7) || (mm == 8) || (mm == 10) || (mm == 12))) 
				{
					return RV_VALIDDATE;
				}
				else if(((dd >= 1) && (dd <=28)) && ((mm == 2))) 
				{
					return RV_VALIDDATE;
				}
				else if(((dd == 29) && (mm == 2)) && ((yy % 400 == 0) || ((yy % 4 == 0) && (yy % 100 != 0))))
				{
					return RV_VALIDDATE;
				}
				else
				{
				return RV_INVALIDDATE;
				}
			}
			else 
			{
				return RV_NODATE;
			}
		}
		else 
		{
			return RV_NODATE;
		}
	}
	else if (Y_1 == isblank(date)) 
	{
		return RV_NODATE;
	}
	else 
	{
		return RV_INVALIDDATE;
	}

}

    
 

/***** main *****/
/* Do not change the main function. */
int main(void) 
{
    // run the loop until hit end of file
    do  {
        int rv = get_date();

        if (!feof(stdin)) {
            switch (rv) {
                case RV_NODATE:
                    printf("No date found.\n");
                    break;
                case RV_VALIDDATE:
                    printf("Valid date.\n");
                    break;
                case RV_INVALIDDATE:
                    printf("Invalid date.\n");
                    break;
                default:
                    printf("Unknown return value %d.\n", rv); 
                    break;
            }
        }
    } while (! feof(stdin));

    return 0;
}
