#include"stdio.h"
//#include"conio.h"
int main()
{
  char *inpstr = '\0' ,*destchar = '\0';
  int size = 0, cnt = 0, length = 0;
  char remchar[12];
  printf ("\nEnter the string size u need:\n");
  scanf ("%d",&size);

  if ( inpstr == '\0' )
  {
    inpstr = (char*)malloc(sizeof(char*)*size);
    destchar = (char*)malloc(sizeof(char*)*size);
    printf ("\nMem.is allocate");
  }
  else
  {
   printf("\nfree called\n");
   free (inpstr);
   free (destchar);
   inpstr = destchar = '\0';
   exit (0);
  }
  
  printf("\nEnter the string now press space key to remove the character from above string:");
  scanf("%s ",inpstr );

  printf("\nstring is entered");

  printf("\n*****Enter the character to remove");
  
  scanf ("%s",remchar );
  
  printf ("\nThe character is:[%s]",remchar );
  
  length = strlen ( inpstr );

  for ( cnt = 0; cnt < length ; cnt++ )
  {
    if ( *inpstr == remchar )
    {
       *destchar = 'e';
        printf ("\nassign char is:[%c] [%c]",remchar , *destchar );
        destchar++;
       /* no operation */
    }
    else
    {
     *destchar = *inpstr;
     printf ("\nCpy char is:[%c]",*destchar);    
     destchar++;
    }
    inpstr++;
  }
}
