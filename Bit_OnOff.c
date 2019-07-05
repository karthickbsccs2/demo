#include <stdio.h>
#include <conio.h>

#define BIT_SET( X, Y )    ( X & ( 1 << Y ) )

int main(void)
{
  unsigned int n,
               num,
               res;
      
  printf ( "\nEnter a number : " );
  scanf ( "%x", &n );

  fflush(stdin);

  printf ( "\nEnter the nth bit, is set or not : " );
  scanf ( "%x", &num );
    
  if ( BIT_SET( n, num ) )
  {
   printf ( "\nBit is set" );
  }
  else
  {
   printf ( "\nBit is not set" );
  }

  getch ( );
  return 0;
}
