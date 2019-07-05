#include"stdio.h"

int myatoifun ( char * );
int mynumrevfun ( int );

int main()
{
  char a []="12345678";
  int tmp = 0, revnum=0 ,innum = 123456789;

  tmp = myatoifun ( &a );
  revnum = mynumrevfun ( innum );
  
  printf ("\nMy atoi value is:[%d Revno.is: %d]", tmp,revnum );
}

int mynumrevfun ( int inpnum )
{
  int tmp, result = 0;
  
  while ( inpnum != 0 )
  {
   tmp     = ( inpnum % 10 );
   inpnum  = ( inpnum / 10 );
   result  = (result*10) + (tmp  ); 
  }
   return result;
}

int myatoifun ( char  *inpstr )
{
  int tmp,resl = 0;

  while ( *inpstr != '\0' )
  {

    printf ("inp str is[%c]\n", *inpstr );

    tmp   = (   ( *inpstr - '0' ));
 
    printf ("inp is [%d]\n", tmp );

  //  resl  = tmp/10;
   
    resl = ( resl * 10 ) + tmp;

    inpstr++;  

  }

  return resl;
}


