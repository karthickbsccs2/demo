#include <stdio.h>
#include <conio.h>
#include <string.h>

int main ( )
{
	char string1[400]  = { 0 },
		 string2[400]  = { 0 };
	int  retvalue;
	 
	printf ( "Enter a string : " );
	scanf ( "%s", string1 );

	printf ( "\nEnter a string to compare : " );
	fflush ( stdin );
	scanf ( "%s", string2 );
	
	retvalue = MyStringCompare ( string2, string1 );
	
	if ( 0 != retvalue )
	{
		printf ( "\nBoth strings are different" );
	}
	else
	{
		printf ( "\nBoth strings are same" );
	}
	
	getch( );
	return 0;
}

int MyStringCompare ( char* destination, char* string )
{	
	int i = 0;
	
	if ( ( string[ i ] != '\0' ) && \
	     ( destination[ i ] != '\0' ) && \
		 ( string[ i ] == destination[ i ] ) )
	{				
		MyStringCompare ( ++destination, ++string );
		
		return 0;
	}
	else
	{
		return ( string[ i ] - destination[ i ] );
	}
}
