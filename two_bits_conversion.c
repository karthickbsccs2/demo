
#include <stdio.h>
#include <conio.h>

#define BIT_SET( X, Y, Z )    ( ( X & ( ( ( 1 << Z ) - 1 ) << Y ) ) >> Y )

int main(void)
{
  unsigned int n,
               bit_value,
               no_of_bits,
               res;
   
  printf ( "\nEnter a number : " );
  scanf ( "%x", &n );

  fflush(stdin);

  printf ( "\nEnter a nth bit to check : " );
  scanf ( "%x", &bit_value );
  
  printf ( "\nEnter a number of bits to check : " );
  scanf ( "%x", &no_of_bits );
  
  res = BIT_SET( n, bit_value, no_of_bits );
  
  printf ( "\nRes[%x]", res );
    
  getch ( );
  return 0;
}
