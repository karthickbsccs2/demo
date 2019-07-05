#include"stdio.h"

typedef struct tagcarrierstruct 
{

  int serviceno;
  char *servicename
}CARRIERSTRUCT;

CARRIERSTRUCT gstcarrierstruct ;

char ptr[15];

int main()
{
 CARRIERSTRUCT stcarrier;
 
 char *srcstr = "justin";
 char *deststr = "raj", *cpystr;
 char *inpstr = "abcdefgh";
 char *mvptr, *moptr;
 
 const int a = 10;
 int *p;
 
  
 int array [5] = { 1,2,3,4,5};
 
 static int starray [2] = { 0 };
 
 //mvptr = moptr = inpstr;

/* memcpy ( moptr+2 ,moptr , 6 );

 printf ("\nMem.Cpy op is:[%s]",moptr );  */

 //memcpy ( inpstr +2,inpstr , 6 );

 printf ("\nMem.Mve ptr is [%s]",inpstr );

 p = &a;

 *p = 15;

 strcpy (ptr , "hello");

 printf ("\ncpy str is[%s]",ptr );

 starray [ 0 ] = array [1];

 printf ("\nThe values are [%d %d %d]",array[1], starray[0],a);
 
 //cpystr = strtok ( srcstr , 'i' );
 
 //printf ("Cpy string is:%s \n",cpystr );

 printf ("%d %d", stcarrier.serviceno, gstcarrierstruct.serviceno ); 
 
 if ( (memcmp ( &gstcarrierstruct , &gstcarrierstruct , sizeof ( CARRIERSTRUCT )== 0) ))
 {
   printf ("\nboth are equal");
 }
 else
 {
  printf ("\nnot equal");
 }
}
