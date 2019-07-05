#include"stdio.h"
/*#define ENABLE_USING_ARRAY*/
int main()
{
#ifdef ENABLE_USING_ARRAY
  char a [] = "hi this is justin";
  char *deststr,*tmpbuf  ;
  deststr = tmpbuf = a;

  while ( *deststr )
  {
    if ( *deststr == 'a' || *deststr == 'e' || *deststr == 'i' || *deststr == 'o' || *deststr == 'u' )
    {
  //     *deststr++;
    //   *tmpbuf = *deststr;      
     //  printf("\nAssign char is[%c]\n",*tmpbuf);
     //  *tmpbuf++;
    }
    else
    {
      *tmpbuf++ = *deststr;   
      printf("\ntmpbuf[%c]\n",tmpbuf );
    }
    *deststr++;
  }
  *tmpbuf = '\0';

  printf ("\nAfter rem the vowel is:[%s]\n",a);
#else 
  char *inpstr = '\0',*opstr= '\0';
  int size = 0,lenght = 0,cnt = 0;

  printf ("\nPls enter the string size:");
  scanf ("%d",&size );

  if ( inpstr == '\0' && size != 0  )
  {
    inpstr = ( char * ) malloc ( sizeof(char *) * size );
    opstr  = (char * ) malloc ( sizeof ( char *) * size );
  }
  else
  {
    if ( inpstr != '\0' )
    {
      free ( inpstr );
      free (opstr );
      printf ("\nMem. is freed\n");
      opstr = inpstr = '\0';
   }
    exit ( 0 );
  }
  
  printf ("\nPls enter the string:\n");
  scanf ("%s",inpstr );
  
  lenght =  strlen ( inpstr );

  if ( lenght == 0 )
  {
    printf("\n not a valid string:\n");
    return 0;
  }
  
  printf ("\nEntered string is :[%s]",inpstr );
 
  for ( cnt = 0 ; cnt < lenght; cnt++ )
  {
    if ( *inpstr == 'a' || *inpstr == 'e' || *inpstr == 'i' || *inpstr == 'o' || *inpstr == 'u')
    {
     //  inpstr++;
    }
    else 
   {
     *opstr = *inpstr; 
     printf("\ncpy str is:[%c]",*opstr );
     opstr++;
   }
    inpstr++;
  }
   
  *opstr = '\0';

  for ( cnt = 0; cnt < 4; cnt++ )
  {
    opstr--; 
    printf ("\nthe cpy char is:[%c]",*opstr);
  }

  printf ("\nThe removed vowel is:[%s]\n",opstr );

  free ( inpstr );
  free ( opstr );
  
  inpstr = opstr = '\0'; 
#endif /* */
}
