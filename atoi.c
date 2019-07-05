/* To convert string into number */

#include <stdio.h>
#include <conio.h>

int main ()
{
	char	string[40] = { 0 };
	int		number = 0;

	printf ( "Enter the string, to convert into a integer : \n" );
	scanf ( "%s", string );
	
	number = myatoi ( string );
	
	printf ( "\nResult is : [%d]", --number );
	
	getch ();
}

int myatoi( char *string )
{
    static int i;
    
    printf ( "\n static i[%d]", i );

    if ( *string != '\0' )
    {		
		i = (i << 3) + (i << 1) + (*string - '0');
		
		printf ( "\n (*string)[%d], (*string - '0')[%d], ['0'][%d]",(*string), (*string - '0'), '0' );
		
        myatoi ( ++string );        
    }
    return( i );
}

