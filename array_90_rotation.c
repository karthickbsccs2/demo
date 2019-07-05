#include <stdio.h>
#include <conio.h>

int main(void)
{
  int num[3][3],
      res[3][3],
      i,
      j,
      row,
      col,
      size = 0;
  
  size = 0;
  row = 3;
  col = 3;
  
  printf ( "Enter the value for 3x3 matrix\n" );
  for ( i = 0; i < row; i++ )
  {
   for ( j = 0; j < col; j++ )
   {
    scanf ( "\n%d", &num[i][j] );
   }
  }

  printf ( "\n" );

  for ( i = 0; i < row; i++ )
  {
   for ( j = 0; j < col; j++ )
   {
    printf ( "[%d]\t", num[i][j] );
   }
   printf ( "\n" );
  }
  
  for ( i = 0; i < row; i++ )
  {
   size = col - 1;
   for ( j = 0; j < col; j++ )
   {
    res[i][j] = num[size--][i];
   }
  }
  
  printf ( "\n" );
  
  for ( i = 0; i < row; i++ )
  {
   for ( j = 0; j < col; j++ )
   {
    printf ( "[%d]\t", res[i][j] );
   }
   printf ( "\n" );
  }

  getch ( );
  return 0;
}
