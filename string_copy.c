#include <stdio.h>
#include <conio.h>
#include <string.h>

void MyStringCopy ( char* destination, char* string );

int main ( )
{
	char string[40]  = { 0 },
		 destination[40]  = { 0 };
	 
	printf ( "\nEnter a string, to copy : " );
	gets ( string );
	
	MyStringCopy ( destination, string );
	
	printf ( "\nResult is [%s]", destination );
	
	getch( );
	return 0;
}

void MyStringCopy ( char* destination, char* string )
{	
	int i = 0;
	
	if ( string[ i ] != '\0' )
	{	
		destination[ i ] = string[ i ];
		
		printf ( "\n Source[%c] Destination[%c]", string[ i ], destination[ i ] );
		
		MyStringCopy ( ++destination, ++string );
	}
}
