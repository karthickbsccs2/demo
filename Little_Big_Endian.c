#include <stdio.h>
#include <conio.h>

int main ( )
{
	unsigned int a = 0xAABBCCDD;
	char *p;

	p = ( char * )&a;

	if ( ( unsigned char ) *p == ( unsigned char ) a )
	{
		printf ( "Little Endian\n" );
	}
	else
	{
		printf ( "Big Endian\n" );
	}
 	printf ( "Result : [%x]\n", a );	
	a = ( ( ( a & 0x0000FFFF ) << 16 ) | \
	      ( ( a & 0xFFFF0000 ) >> 16 ) );
 
 	printf ( "Result : [%x]\n", a );
 	
	getch ( );	
	return 0;
}
