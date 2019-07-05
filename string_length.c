#include <stdio.h>
#include <conio.h>
#include <string.h>

int main ( )
{
	char string[40]  = { 0 };
	int  length = 0;
	 
	printf ( "Enter a string, to find its length : \n" );
	scanf ( "%s", string );
	
	length = MyStringLength ( string );
	
	printf ( "Length of [%s] is %d\n", string, length );
	
	getch( );
	return 0;
}

int MyStringLength ( char* string )
{
	static int len;
	
	if ( *string == '\0' )
	{
		return 0;
	}
	else
	{	
		++len;
		
		MyStringLength ( ++string );
		
		return len;
	}
}
