/* To count number of bits are set */

#include <stdio.h>
#include <conio.h>

int main ()
{
	unsigned int num   = 0,
				 count = 0;
	
	printf ( "Enter the number, to count no. of bit is set : \n" );
	scanf ( "%d", &num );
	
	for ( ; 0 != num; num >>= 1 )
	{	
		if ( num & 1 )
		{	
			count++;
		}
	}
	printf ( "\nNo. of bit is : %d", count );
	getch ();
}
