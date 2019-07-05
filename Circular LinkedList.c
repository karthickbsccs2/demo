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
	
	AddEmployeeDetailInQueue ( pstEmpDetail );
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

		pstTemp ->pstNext ->pstNext = gspstEmpDetailsHead;
	}
		return 0;
}

unsigned char DisplyEmployeeDetails ( void )
{
	EMP_LIST_DETAILS_STRUCT  *pstTemp;

	if ( gspstEmpDetailsHead != NULL )
	{
		pstTemp = gspstEmpDetailsHead;
		do
		{
			printf ( "*********************************************************************\n\n");
			PrintEmployeeDetails ( pstTemp ->pstEmpDetailsInfo );
			printf ( "*********************************************************************\n\n");
			
			pstTemp = pstTemp ->pstNext;

		}while ( ( pstTemp != gspstEmpDetailsHead ) && \
			 ( NULL != pstTemp ) );
	}
	return 0;
}

/* Display last node of linked list */
unsigned char DisplayLastNodeOfEmployeeDetails ( void )
{
	EMP_LIST_DETAILS_STRUCT  *pstTemp,
				 *pstPrev;

	if ( gspstEmpDetailsHead != NULL )
	{
		pstTemp = gspstEmpDetailsHead;
		
		do
		{
			pstPrev = pstTemp;
			pstTemp = pstTemp ->pstNext;

		}while ( ( pstTemp != gspstEmpDetailsHead ) && \
			 ( NULL != pstTemp ) );

		printf ( "*********************************************************************\n\n");
		PrintEmployeeDetails ( pstPrev ->pstEmpDetailsInfo );
		printf ( "*********************************************************************\n\n");
	}
	return 0;
}

void PrintEmployeeDetails ( EMP_DETAILS_STRUCT *pstEmpDetail )
{
	printf ( "Employee First Name : %s\n", pstEmpDetail ->pcEmpFirstName );
	printf ( "Employee Last Name : %s\n", pstEmpDetail ->pcEmpLastName );
	printf ( "Employee Roll No : %d\n", pstEmpDetail ->EmpRollNo );
	printf ( "Employee ID : %d\n", pstEmpDetail ->EmpId );
}

/* Delete a node based on First Name */
unsigned char DeleteEmployeeDetailsByName ( void )
{
	EMP_LIST_DETAILS_STRUCT  *pstTemp;

	printf ( "LINKED_LIST.C %d> Enter the Employee first name to delete\n", __LINE__ );
	scanf ( "%s", &cSeachName );

	if ( gspstEmpDetailsHead != NULL )
	{
		pstTemp = gspstEmpDetailsHead;
		do
		{
			if ( !strncmp ( cSeachName, pstTemp ->pstEmpDetailsInfo ->pcEmpFirstName, strlen ( cSeachName ) ) )
			{
				printf ( "*********************************************************************\n\n");
				PrintEmployeeDetails ( pstTemp ->pstEmpDetailsInfo );
				printf ( "*********************************************************************\n\n");
			}
			else
			{
				pstTemp = pstTemp ->pstNext;
			}

		}while ( ( pstTemp != gspstEmpDetailsHead ) && \
			 ( NULL != pstTemp ) );
	}
	return 0;
}

/* Linear Search */
unsigned char SearchEmployeeDetailsByNameUsingLinearSearch ( void )
{
	EMP_LIST_DETAILS_STRUCT  *pstTemp;

	printf ( "LINKED_LIST.C %d> Enter the Employee first name to search\n", __LINE__ );
	scanf ( "%s", &cSeachName );

	if ( gspstEmpDetailsHead != NULL )
	{
		pstTemp = gspstEmpDetailsHead;
		do
		{
			if ( !strncmp ( cSeachName, pstTemp ->pstEmpDetailsInfo ->pcEmpFirstName, strlen ( cSeachName ) ) )
			{
				printf ( "*********************************************************************\n\n");
				PrintEmployeeDetails ( pstTemp ->pstEmpDetailsInfo );
				printf ( "*********************************************************************\n\n");
			}
			else
			{
				pstTemp = pstTemp ->pstNext;
			}

		}while ( ( pstTemp != gspstEmpDetailsHead ) && \
			 ( NULL != pstTemp ) );
	}
	return 0;
}

/* Binary Search */
unsigned char SearchEmployeeDetailsByNameUsingBinarySearch ( void )
{
	EMP_LIST_DETAILS_STRUCT  *pstTemp;
	UINT16			 ui16TotalNode = 1,
				 ui16Low,
				 ui16High;
		

	printf ( "LINKED_LIST.C %d> Enter the Employee first name to search\n", __LINE__ );
	scanf ( "%s", &cSeachName );

	/* To  calculate total node */
	if ( gspstEmpDetailsHead != NULL )
	{
		pstTemp = gspstEmpDetailsHead;
		do
		{
			printf ( "*********************************************************************\n\n");
			PrintEmployeeDetails ( pstTemp ->pstEmpDetailsInfo );
			printf ( "*********************************************************************\n\n");
			
			pstTemp = pstTemp ->pstNext;
			
			++ui16TotalNode;
		}while ( ( pstTemp != gspstEmpDetailsHead ) && \
			 ( NULL != pstTemp ) );
	}


	ui16Low = 1;
	ui16High = ui16TotalNode;


	if ( gspstEmpDetailsHead != NULL )
	{
		pstTemp = gspstEmpDetailsHead;
		do
		{
			if ( ui16Low <= ui16High )
			{
				ui16Mid = ( ( ui16Low + ui16High ) / 2 );
				
				for ( i = 0; i < ui16Mid; i++ )
				{
					pstTemp = pstTemp ->pstNext;
				}

				if ( strcmp ( cSeachName, pstTemp ->pstEmpDetailsInfo ->pcEmpFirstName ) > 0 )
				{
					ui16High = ui16Mid - 1;
				}
				else
				{
					ui16Low = ui16Mid + 1;
				}
		}while ( ( pstTemp != gspstEmpDetailsHead ) && \
			 ( NULL != pstTemp ) );
	}
	return 0;
}
