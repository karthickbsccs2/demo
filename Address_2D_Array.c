#include <stdio.h>
#include <conio.h>

void myfunc ( char **name, int size );

int main(void)
{
  char *name;

  myfunc ( &name, 14 );
   
  printf("[%s]", name );
  
  getch ( );
  return 0;
}

void myfunc ( char **name, int size )
{
  *name = (char *) malloc ( size * sizeof( char *) );
  
  strcpy( *name, "DAVID RAJESH" );
}
