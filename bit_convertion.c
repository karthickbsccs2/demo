#include <stdio.h>
#include <conio.h>

void add ( void );

int main ()
{
	unsigned int a = 0xf7,
		b,
	 	pos = 5,  
		 d,
         e,
         f;

/* Method : 1 */
	b = ( ( a & ( ( 0xFFFFFFFF ) ^ ( 1 << pos ) ) ) | \
		( ( ~( ( a & ( 1 << pos ) ) >> pos ) & 1 ) << pos ) );

/* Method : 2 */			
	d = ( a ^ ( 1 << pos ) );
	
	e = ( a | ( 1 << pos ) );
	
	f = ( a & ~( 1 << pos ) );
	
	add ( );
	printf ( "\nRESET[%x], SET[%x] CLEAR[%x]", d, e, f );
	add ( );
	getch ( );
	return 0;
}

void add ( void )
{
     int d = 5;
     
     printf ( "\nd[%d]", d );
}
