#include <stdio.h>
#include <conio.h>

typedef struct tagEmpDetails
{
	char* pcEmpFirstName;
	char* pcEmpLastName;
	int	EmpRollNo;
	int EmpId;	
}EMP_DETAILS_STRUCT;

typedef struct tagEmpList
{	
	EMP_DETAILS_STRUCT *pstEmpDetailsInfo;
	struct tagEmpList *pstNext;
}EMP_LIST_DETAILS_STRUCT;

static EMP_LIST_DETAILS_STRUCT *gspstEmpDetailsHead;

unsigned char AddEmployeeDetailInQueue ( EMP_DETAILS_STRUCT *pstEmpDetail );
unsigned char AddEmployeeDetailInQueueByRollNoSort ( EMP_DETAILS_STRUCT *pstEmpDetail );
unsigned int AddNewEmpDetails ( unsigned char ui8NodeAddType );
unsigned char DisplyEmployeeDetails ( unsigned int UserOption, char* pcEmployeeFirstName, char* pcEmployeeLastName );
unsigned char SearchByEmployeeFirstName ( void );
unsigned char SearchByEmployeeLastName ( void );
void PrintEmployeeDetails ( EMP_DETAILS_STRUCT *pstEmpDetail );
void DeleteSeachNode ( EMP_LIST_DETAILS_STRUCT *pstDeleteNode );
unsigned char DeleteEmpDetailsbyFirstName ( void );
unsigned char DispalyEmployeeListDetailsByIndex ( void );

int main ( )
{
	unsigned char ui8Choice,
	  			  ui8ErrorCode;
	
	while ( 1 )
	{
		printf ( "LINKED_LIST.C %d> Enter the any one of the option\n1.Add\n2.Sorting Add\n3.Delete\n4.Display\n5.Search by First Name\n6.Search by Last Name\n7.Display by Start Index\n\n8.Quit\n",__LINE__);
		fflush ( stdin );
		scanf ( "%d", &ui8Choice );

		switch ( ui8Choice )
		{
			case	1:
			{			
				ui8ErrorCode = AddNewEmpDetails ( 0 );
				
				if ( 0 != ui8ErrorCode )
				{
					printf ( "LINKED_LIST.C %d> Unable to add the current Emp details\n",__LINE__);	
				}
			}
				break;
				
			case	2:
			{
				ui8ErrorCode = AddNewEmpDetails ( 1 );

				if ( 0 != ui8ErrorCode )
				{
					printf ( "LINKED_LIST.C %d> Unable to add the current Emp details\n",__LINE__);	
				}
			}
				break;
				
			case	3:
			{
				ui8ErrorCode = DeleteEmpDetailsbyFirstName ( );
				
				if ( 0 != ui8ErrorCode )
				{
					printf ( "LINKED_LIST.C %d> Unable to delete the req Emp details\n",__LINE__);	
				}
				
			}
				break;
				
			case	4:
			{
				DisplyEmployeeDetails ( 0, NULL, NULL );
			}
				break;
				
			case	5:
			{
				ui8ErrorCode = SearchByEmployeeFirstName ( );
				
				if ( 0 != ui8ErrorCode )
				{
					printf ( "LINKED_LIST.C %d> Unable to search the Emp details\n",__LINE__);	
				}
			}
				break;
				
			case	6:
			{
				ui8ErrorCode = SearchByEmployeeLastName ( );
				
				if ( 0 != ui8ErrorCode )
				{
					printf ( "LINKED_LIST.C %d> Unable to search the Emp details\n",__LINE__);	
				}
			}
				break;

			case	7:
			{
				ui8ErrorCode = DispalyEmployeeListDetailsByIndex ( );				
			}
				break;
				
			case	8:
			{
				exit ( 0 );
			}
				break;
				
			default:
			{
				printf ( "LINKED_LIST.C %d> Enter the valid option like \n1.Add\n2.Sorting Add\n3.Delete\n4.Display\n5.Search by First Name\n6.Search by First Name\n7.Quit\n",__LINE__);
			}
				break;
		}
	}
	return 0;
}

unsigned int AddNewEmpDetails ( unsigned char ui8NodeAddType )
{
	char EmpFirstName [ 17 ] = { 0 },
		 EmpLastName [ 17 ] = { 0 };
		 
	unsigned int EmpRollNo = 0,
				 EmpId = 0;
				 
	EMP_DETAILS_STRUCT *pstEmpDetail = NULL;
	
	pstEmpDetail = ( EMP_DETAILS_STRUCT * ) malloc ( sizeof ( EMP_DETAILS_STRUCT ) );
	
	if ( NULL == pstEmpDetail )
	{
		printf ( "LINKED_LIST.C %d> Memory Allocation failed with size [ %0x%02X ]\n", 
				__LINE__,
				sizeof ( EMP_DETAILS_STRUCT ) );
		
		return 1;
	}
		 
	printf ( "LINKED_LIST.C %d> Enter the new Employee First name & Last name with max of 16 character\n",__LINE__ );
	scanf ( "%s%s", &EmpFirstName, &EmpLastName );
	
	printf ( "Employee First Name : %s\n", EmpFirstName );
	
	pstEmpDetail ->pcEmpFirstName = ( char* ) malloc ( 16 + 1 );
	
	if ( NULL == pstEmpDetail ->pcEmpFirstName )	
	{
		printf ( "LINKED_LIST.C %d> Memory Allocation failed with size [ %0x%02X ]\n", 
				__LINE__,
				16 + 1 );
		
		free ( pstEmpDetail );
		pstEmpDetail = NULL;
		
		return 1;	
	}
	
	strcpy ( pstEmpDetail ->pcEmpFirstName, EmpFirstName );
	printf ( "Employee First Name : %s\n", pstEmpDetail ->pcEmpFirstName );	
	
	pstEmpDetail ->pcEmpLastName = ( char* ) malloc ( 16 + 1 );
	
	if ( NULL == pstEmpDetail ->pcEmpLastName )	
	{
		printf ( "LINKED_LIST.C %d> Memory Allocation failed with size [ 0x%02X ]\n", 
				__LINE__,
				16 + 1 );

		free ( pstEmpDetail );
		pstEmpDetail = NULL;
						
		free ( pstEmpDetail ->pcEmpFirstName );
		pstEmpDetail ->pcEmpFirstName = NULL;
		
		return 1;
	}
	
	strcpy ( pstEmpDetail ->pcEmpLastName, EmpLastName );
	printf ( "Employee Last Name : %s\n", pstEmpDetail ->pcEmpLastName );
	
	printf ( "LINKED_LIST.C %d> Enter the Employee Roll No\n", __LINE__ );
	scanf ( "%d", &EmpRollNo );
	
	printf ( "LINKED_LIST.C %d> Enter the Employee ID\n", __LINE__ );
	scanf ( "%d", &EmpId );

	pstEmpDetail ->EmpRollNo = EmpRollNo;
	pstEmpDetail ->EmpId = EmpId;
	
	printf ( "Employee Roll No : %d\n", pstEmpDetail ->EmpRollNo );
	printf ( "Employee ID : %d\n", pstEmpDetail ->EmpId );
	
	if ( !ui8NodeAddType )
	{
		return ( AddEmployeeDetailInQueue ( pstEmpDetail ) );		
	}
	else
	{
		return ( AddEmployeeDetailInQueueByRollNoSort ( pstEmpDetail ) );	
	}

}

unsigned char AddEmployeeDetailInQueue ( EMP_DETAILS_STRUCT *pstEmpDetail )
{
	EMP_LIST_DETAILS_STRUCT *pstEmpDetailsInfo,
 							 *pstTemp;
	
	if ( NULL == pstEmpDetail )
	{
		printf ( "LINKED_LIST.C %d> Invalid Employee details received to add in database\n", 
				__LINE__,
				sizeof ( EMP_LIST_DETAILS_STRUCT ) );
				
		return 1;	
	}
	
	pstEmpDetailsInfo = ( EMP_LIST_DETAILS_STRUCT *)malloc ( sizeof ( EMP_LIST_DETAILS_STRUCT ) );
	
	if ( NULL == pstEmpDetailsInfo )
	{
		printf ( "LINKED_LIST.C %d> Memory Allocation failed with size [ %0x%02X ]\n", 
				__LINE__,
				sizeof ( EMP_LIST_DETAILS_STRUCT ) );
		
		return 1;
	}
	
	pstEmpDetailsInfo ->pstEmpDetailsInfo = pstEmpDetail;
	pstEmpDetailsInfo ->pstNext = NULL;
	
	if ( NULL == gspstEmpDetailsHead )
	{
		gspstEmpDetailsHead = pstEmpDetailsInfo;
	}
	else
	{
		pstTemp = gspstEmpDetailsHead;
		
		while ( NULL != pstTemp ->pstNext )
		{
			pstTemp = pstTemp ->pstNext;
		}
		
		pstTemp ->pstNext = pstEmpDetailsInfo;
	}
		return 0;
}

unsigned char AddEmployeeDetailInQueueByRollNoSort ( EMP_DETAILS_STRUCT *pstEmpDetail )
{
	EMP_LIST_DETAILS_STRUCT *pstEmpDetailsInfo,
 							 *pstTemp,
							  *pstPrev;
	
	if ( NULL == pstEmpDetail )
	{
		printf ( "LINKED_LIST.C %d> Invalid Employee details received to add in database\n", 
				__LINE__,
				sizeof ( EMP_LIST_DETAILS_STRUCT ) );
				
		return 1;	
	}
	
	pstEmpDetailsInfo = ( EMP_LIST_DETAILS_STRUCT *)malloc ( sizeof ( EMP_LIST_DETAILS_STRUCT ) );
	
	if ( NULL == pstEmpDetailsInfo )
	{
		printf ( "LINKED_LIST.C %d> Memory Allocation failed with size [ %0x%02X ]\n", 
				__LINE__,
				sizeof ( EMP_LIST_DETAILS_STRUCT ) );
		
		return 1;
	}
	
	pstEmpDetailsInfo ->pstEmpDetailsInfo = pstEmpDetail;
	pstEmpDetailsInfo ->pstNext = NULL;
	
	if ( NULL == gspstEmpDetailsHead )
	{
		gspstEmpDetailsHead = pstEmpDetailsInfo;
	}
	else
	{
		pstTemp = gspstEmpDetailsHead;
		pstPrev = pstTemp;
		
		while ( NULL != pstTemp )
		{
			if ( pstTemp ->pstEmpDetailsInfo->EmpRollNo > pstEmpDetailsInfo ->pstEmpDetailsInfo ->EmpRollNo )
			{
				break;
			}
			
			pstPrev = pstTemp;
			pstTemp = pstTemp ->pstNext;
		}
		
		pstEmpDetailsInfo ->pstNext = pstTemp;
		
		if ( pstTemp == gspstEmpDetailsHead )
		{
			gspstEmpDetailsHead = pstEmpDetailsInfo;
		}
		else
		{
			pstPrev ->pstNext = pstEmpDetailsInfo;
		}
	}
	
	return 0;
}

void DeleteSeachNode ( EMP_LIST_DETAILS_STRUCT *pstDeleteNode )
{
	EMP_LIST_DETAILS_STRUCT  *pstTemp = gspstEmpDetailsHead,
 							 *pstPrev;
	
	free( pstDeleteNode ->pstEmpDetailsInfo ->pcEmpFirstName );
	pstDeleteNode ->pstEmpDetailsInfo ->pcEmpFirstName = NULL; 
	
	printf ( "First name freed\n");
	
	free( pstDeleteNode ->pstEmpDetailsInfo ->pcEmpLastName );
	pstDeleteNode ->pstEmpDetailsInfo ->pcEmpLastName = NULL; 
	
	printf ( "list name freed\n");
	
	free ( pstDeleteNode ->pstEmpDetailsInfo );
	pstDeleteNode ->pstEmpDetailsInfo = NULL;
	
	printf ( "structure freed\n");
	
	pstPrev = pstTemp;
	
	while ( NULL != pstTemp )
	{
		if ( gspstEmpDetailsHead == pstDeleteNode )
		{
			printf ( "Head node deleted\n");
			gspstEmpDetailsHead = gspstEmpDetailsHead ->pstNext;
			
			break;
		}
		else
		{	
			if ( pstDeleteNode == pstTemp )
		 	{
		 		printf ( "Sub node deleted\n");
				pstPrev ->pstNext = pstTemp ->pstNext;
				
				break;
			}
			
			pstPrev = pstTemp;
			pstTemp = pstTemp ->pstNext;
		}
	}
	
	//free ( pstDeleteNode );
	//pstDeleteNode = NULL;
	
}

unsigned char DisplyEmployeeDetails ( unsigned int UserOption, char* pcEmployeeFirstName, char* pcEmployeeLastName )
{
	static unsigned int SearchNo = 1;
	EMP_LIST_DETAILS_STRUCT  *pstTemp = gspstEmpDetailsHead;
	
	if ( 0 == UserOption )
	{
		while ( NULL != pstTemp )
		{
			printf ( "Employee%d Details\n********************************************",SearchNo );
			PrintEmployeeDetails ( pstTemp ->pstEmpDetailsInfo );
			printf ( "*********************************************************************\n\n");
			
			pstTemp = pstTemp ->pstNext;
			++SearchNo;
		}
	}
	else if ( 1 == UserOption )
	{
		if ( NULL == pcEmployeeFirstName )
		{
			printf ( "NULL Employee received\n" );
			return 1;
		}
		
		while ( NULL != pstTemp )
		{
			if ( !strncmp ( pcEmployeeFirstName, pstTemp ->pstEmpDetailsInfo ->pcEmpFirstName, strlen ( pcEmployeeFirstName ) ) )
			{
				printf ( "Employee%d Details\n********************************************",SearchNo );
				PrintEmployeeDetails ( pstTemp ->pstEmpDetailsInfo );
				printf ( "*********************************************************************\n\n");
				++SearchNo;
				break;
			}
			pstTemp = pstTemp ->pstNext;
		}
	}
	else if ( 2 == UserOption )
	{
		if ( NULL == pcEmployeeLastName )
		{
			printf ( "NULL Employee received\n" );
			return 1;
		}
		
		while ( NULL != pstTemp )
		{
			if ( !strncmp ( pcEmployeeLastName, pstTemp ->pstEmpDetailsInfo ->pcEmpLastName, strlen ( pcEmployeeLastName ) ) )
			{
				printf ( "Employee%d Details\n********************************************",SearchNo );
				PrintEmployeeDetails ( pstTemp ->pstEmpDetailsInfo );
				printf ( "*********************************************************************\n\n");
				++SearchNo;
				break;
			}
			pstTemp = pstTemp ->pstNext;
		}
	}
	
	SearchNo = 1;
	
	return 0;
}

unsigned char DispalyEmployeeListDetailsByIndex ( void )
{
	unsigned int SearchNo   = 1,
                 StartIndex = 0,
                 TotalIndex = 0;
	EMP_LIST_DETAILS_STRUCT  *pstTemp = gspstEmpDetailsHead;
	
	printf ( "Enter the start index : \n" );
	scanf ( "%d", &StartIndex );
	
	printf ( "Enter the number of nodes : \n" );
	scanf ( "%d", &TotalIndex );

    while ( SearchNo < StartIndex )
	{
		if ( NULL != pstTemp )
		{
           printf ( "INSIDE WHILE number of nodes : [%d]\n", SearchNo );
           pstTemp = pstTemp ->pstNext;
		   ++SearchNo;
        }
    }
    
    printf ( "number of nodes : [%d]\n", SearchNo );
    
    SearchNo = 1;
    
	while ( SearchNo <= TotalIndex )
	{
        if ( NULL != pstTemp )
        {
           PrintEmployeeDetails ( pstTemp ->pstEmpDetailsInfo );
           pstTemp = pstTemp ->pstNext;
		   ++SearchNo;  
        }   
    }
}

void PrintEmployeeDetails ( EMP_DETAILS_STRUCT *pstEmpDetail )
{
	printf ( "Employee First Name : %s\n", pstEmpDetail ->pcEmpFirstName );
	printf ( "Employee Last Name : %s\n", pstEmpDetail ->pcEmpLastName );
	printf ( "Employee Roll No : %d\n", pstEmpDetail ->EmpRollNo );
	printf ( "Employee ID : %d\n", pstEmpDetail ->EmpId );
}

unsigned char SearchByEmployeeFirstName ( void )
{
	char cSeachName [ 17 ] = { 0 };
	
	printf ( "LINKED_LIST.C %d> Enter the Employee First name to search\n", __LINE__ );
	scanf ( "%s", &cSeachName );
	
	DisplyEmployeeDetails ( 1, cSeachName, NULL );
}

unsigned char SearchByEmployeeLastName ( void )
{
	char cSeachName [ 17 ] = { 0 };
	
	printf ( "LINKED_LIST.C %d> Enter the Employee Last name to search\n", __LINE__ );
	scanf ( "%s", &cSeachName );
	
	DisplyEmployeeDetails ( 2, NULL, cSeachName );
}

unsigned char DeleteEmpDetailsbyFirstName ( void )
{
	char cSeachName [ 17 ] = { 0 };
	EMP_LIST_DETAILS_STRUCT  *pstTemp = gspstEmpDetailsHead,
							 *pstDelete;
	static unsigned int SearchNo = 1;
	
	printf ( "LINKED_LIST.C %d> Enter the Employee first name to delete\n", __LINE__ );
	scanf ( "%s", &cSeachName );
	
	while ( NULL != pstTemp )
	{
		if ( !strncmp ( cSeachName, pstTemp ->pstEmpDetailsInfo ->pcEmpFirstName, strlen ( cSeachName ) ) )
		{
			printf ( "Deleted Employee%d Details\n********************************************",SearchNo );
			printf ( "Employee First Name : %s\n", pstTemp ->pstEmpDetailsInfo ->pcEmpFirstName );
			printf ( "*********************************************************************\n\n");
			DeleteSeachNode ( pstTemp );
			pstDelete = pstTemp;
			
			pstTemp = pstTemp ->pstNext;
			
			free ( pstDelete );
			pstDelete = NULL;
			
			++SearchNo;
		}
		else
		{
			pstTemp = pstTemp ->pstNext;
		}
	}
	SearchNo = 1;
	
	return 0;
}
