#include <stdio.h>
#include <conio.h>

int fun ( int x, char *p[] );

int main(void)
{
  char *p[10],
       n = 0,
       i;

  do
  {
    p[n] = ( char *) malloc ( 12 * sizeof( char ) );
    scanf ("%s", p[n]);
  }while(strcmp(p[n++], "END"));
  
  printf ( "\n%d", n);
  
  fun ( --n, p );
  
  for ( i=0; i < n ; i++ )
  {
   printf ( "\n%s", p[i]);
  }

  getch ( );
  return 0;
}

int fun ( int x, char *p[] )
{
  int item,
      k;
  char *temp;

  for ( item=0; item < x; item++ )
  for ( k=item+1; k < x; k++ )
  {
    if ( ( strcmp ( p[item], p[k] ) ) > 0 )
    {
      temp = p[item];
      p[item] = p[k];
      p[k] = temp;
    }
  }  
}
