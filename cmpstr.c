#include"stdio.h"
#define enable_string_cmp
int mycmpstr(char * , char * );
int main()
{
  char first [] = "justinn i am";
  char second[] = "justinn i am";
  char *srcstr,*deststr;
  int  flagstat;

  srcstr = first;
  deststr = second;

  flagstat = mycmpstr (srcstr,deststr);
  if ( flagstat == 0 )
  {
    printf ("\nstring is equal[%d]\n",flagstat);
  }
  else
  {
   printf ("\nstring is not equal[%d]\n",flagstat );
  }
     
}

int mycmpstr ( char *first, char *second )
{
#ifdef enable_string_cmp
  while (*first == *second )
  {
     if ( *first == '\0'  )
     return 0;
     first++;
     second++;
  }
  
  return ( *first - *second );
#else
   while ( *first != '\0' && second != '\0' )
  {
      if ( *first == *second )
     {
       first++;
       second++;
     }
     else
    {
        printf ("\nnot equal\n");
        return  (*first - *second );
        
    }   

  } 
/*  if (*first == '\0' || *second == '\0' )
 {
   return 1;
 } */
#endif /* */
}
