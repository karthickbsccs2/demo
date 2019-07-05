#include"stdio.h"
int main()
{
  int a[ 12 ] = { 9,6,3,2,6,8,9,1,2,0,2,1};
  int b[12],c[12],t,i,j;
   
  for ( i = 0; i < 12; i++)
  {
     for ( j = i+1; j<12; j++)
     {
        if ( a[i] > a[j])
    	{
 	  t = a[i];
 	  a[i]= a[j];
          a[j] = t;
        }
    }
  }
  for ( i = 0; i<12;i++ )
  {
    printf (" %d ",a[i]);
  } 

}
