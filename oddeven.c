#include"stdio.h"

/*#define CHK_ADD_OR_EVEN
#define   CHK_NO_OF_BIT_SET*/

#define POW_2(__X__)               (1<<__X__)
#define ODD_EVEN(__X__)            (__X__ & 1)
#define TOGGLE_BIT(__X__, __Y__)   (__X__ ^(1<<(__Y__))) 
#define MAX(__X__,__Y__,__Z__)  
#define SET_BIT(__X__,__Y__)       (__X__ |(1<<(__Y__)))
#define CLEAR_BIT(__X__,__Y__)     (__X__ ^(1<<(__Y__)))
#define MUL_4(__X__)               (__X__+3)&(~3)
#define MUL_2(__X__)               (__X__+1)&(~1)
#define DIV_4(__X__)		   (__X__&3)
#define DIV_8(__X__)               (__X__&7)
#define MUL_8(__X__)               (__X__+7)&(~7)

int main ()
{
   int inpnum,tmp,position,cnt=0,loop;
   char str [] = "httpi:/justin/in";
   char str2[] = "hi this is justin";
 
   //str2 = strstr(str,"in");

   printf ("strcpy is:[%s]\n",strncpy(str2,str,6 ));

/*   printf("%d\n",strlen("test"));*/

  printf ("\nPls enter the no. & position to reset the particular bit:\n");
  scanf ("%d %d",&inpnum , &position );

  printf ("\nMultiple of 8 is[%d]\n",MUL_4(inpnum)); 
#ifdef CHK_NO_OF_BIT_SET
/*   
   while ( inpnum )
   {
     cnt++;
     inpnum = inpnum & ( inpnum -1 );
   }
*/

   for ( loop = 0; loop < 32; loop++ )
   {
     if ( inpnum & (1<<loop) )
     {
       cnt++;
       printf("\nLoop cnt is:[%d]",cnt );
     }
   }

#endif /* */  

  printf ("\nTota; bit set is:[%d]",cnt );

  printf ("\nsizeof int is [%d]\n",sizeof(int));

  printf("\nPower of 2 is:[%d]", POW_2(inpnum));

#ifdef CHK_ADD_OR_EVEN
  
  tmp = ODD_EVEN(inpnum);
  if (tmp == 0 )
  {
    printf ("\ngiven number is even[%d]\n",tmp);
  }
  else
  {
   printf ("\ngiven number is odd[%d]\n",tmp);
  }
#else
  tmp = DIV_4(inpnum);
  
  if ( tmp == 0 )
  {
    printf ("\n[%d] is divide by 4\n",inpnum);
  }
  else
  {
   printf("\n[%d] is not divided by 4\n",inpnum);
  }
#endif /* */
  
  printf ("\nAfter reset the position:[%d]\n",TOGGLE_BIT(inpnum,position));

  printf("\nMultiple of 4 is:[%d]\n",((inpnum+1)&~1));

  printf("\nMultiple of 8 is[%d]\n",MUL_8(inpnum));      
}
