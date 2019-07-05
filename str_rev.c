/* To reverse the  string */

#include <stdio.h>
#include <conio.h>

char * MyStringReverseUsingRecursive ( char *string );

int main ()
{
	char	string[40] = { 0 },
			*reverse;

	printf ( "Enter the string, to reverse : \n" );
	gets ( string );
	
	reverse = MyStringReverseUsingRecursive ( string );
	
	printf ( "Given string reversed into[%s] : \n", reverse );
	
	getch ( );
	return 0;
}

char * MyStringReverseUsingRecursive ( char *string )
{
	char* pcRev;
	static unsigned int ui16StrLen,
						ui16ParseCnt = 0;
	unsigned int ui16Temp;
						
	if ( '\0' != *string )
	{
		++ui16StrLen;

		pcRev = MyStringReverseUsingRecursive ( ++string );
	}
	else
	{	
		return ( string - ui16StrLen );
	}
	
	--string;
	
	if ( ui16ParseCnt < ui16StrLen / 2 )
	{
		ui16Temp = pcRev [ ui16ParseCnt ];
		
		pcRev [ ui16ParseCnt ] = *string;
		
		*string = ui16Temp;
		
		++ui16ParseCnt;
	}	
	return pcRev;
}
