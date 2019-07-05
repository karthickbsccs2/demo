#include <stdio.h>
#include <conio.h>

int main(void)
{
  unsigned int n;
      
  printf ( "\nEnter a number : " );
  scanf ( "%d", &n );
    
  if ( ( n & 1 ) || \
       ( n & ( 1 << 1 ) ) )
  {
   printf ( "\nNot Divisible by 4" );
  }
  else
  {
   printf ( "\nDivisible by 4" );
  }

  getch ( );
  return 0;
}
