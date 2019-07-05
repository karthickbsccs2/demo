#include"stdio.h"
//int myconcatfun ( char *, char *);
int mycpyfun ( char * , char * );
int main()
{
  char src[] = "justin";
  char des[] = "is a good boy";
  
  char *srcstr, *deststr;

  srcstr = src;
  deststr = des;

//  myconcatfun ( srcstr, deststr );
  mycpyfun ( srcstr , deststr );

  printf("\nthe string is [%s]\n",src);
}

int myconcatfun ( char *srcstr , char *deststr )
{
  int cnt;

  while ( *srcstr != '\0' )
  { 
    printf ("\nlenth is [%d]",cnt);
    cnt++;
    srcstr++;
  }
  printf ("\nlenght of the string is [%d]\n",cnt );

 // srcstr++;

 while ( *deststr != '\0' )
 {
   *srcstr = *deststr;
   printf ( "\ncpy str is [%c]\n",*srcstr );
   deststr++;
   srcstr++;
 }   
  
}

int mycpyfun ( char *srcstr ,char  *deststr )
{
  while ( *deststr )
  {
    *srcstr = *deststr;
    srcstr++;
    deststr++;
  }
  *srcstr = '\0';
}
