#include<stdio.h>
#include<conio.h>
#include<string.h>

int main()
{
 char s[20];
 int i,j,len,count;
 
 printf ("Enter name : ");
 scanf ("%s", s);
 
 len = strlen(s);
 count = 0;
 
 for ( i=0, j = len - 1; i < j; i++, j-- )
 {
  if ( s[i] != s[j] )
  {
   count++;
  }
 }
 
 if ( count )
 {
  printf ( "\n[%s] is Not Palindrome", s );
 }
 else
 {
  printf ( "\n[%s] is Palindrome", s );
 }

getch();
return 0;
}
