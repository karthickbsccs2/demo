#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
 
void allocate2D(int** array, int nrows, int ncols)
{
 int i;
 /*  allocate array of pointers  */
 printf( "Allocating memory for array of pointers\n" );
 array = ( int** )malloc( nrows*sizeof( int* ) );
 
 /*  allocate each row  */
 for(i = 0; i < nrows; i++)
 {
  printf( "Allocating memory for each row %d\n",i );
  array[i] = ( int* )malloc( ncols*sizeof( int ) );
 }
}
 
void deallocate2D(int** array, int nrows)
{ 
 /*  deallocate each row  */
 int i;
 for(i = 0; i < nrows; i++)
 {
  printf( "deallocate each row %d\n",i );
  free(array[i]);
 }
 
 /*  deallocate array of pointers  */
 printf( "deallocating array of pointers\n" );
 free(array);
}
 
int main()
{
  int **arr;
  
  allocate2D (arr, 2, 2);
  deallocate2D (arr, 2);
  
  getch( );
  return 0;
}
