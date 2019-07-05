#include <stdio.h>
#include <conio.h>
#include <string.h>

int main ( )
{
	char string[40]  = { 0 };
	int  i = 0;
	 
	printf ( "Enter a string, to remove the vowels : \n" );
	gets ( string );
	
	while ( i < strlen( string ) )
	{
		if ( ( string[ i ] == 'a' ) || ( string[ i ] == 'e' ) || ( string[ i ] == 'i' ) || \
		      ( string[ i ] == 'o' ) || ( string[ i ] == 'u' ) || ( string[ i ] == ' ' ) )
		{
			memcpy ( ( string + i ),
					   ( string + i + 1 ),
					   ( strlen( string ) - i ) );					   
		}
		else
		{
			++i;		
		}
	}
	printf ( "Result is [%s] Length[%d]", string, strlen ( string ) );
	getch( );
	return 0;
}
