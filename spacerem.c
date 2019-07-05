#include"stdio.h"
//char *remspace ( char * );
int main()
{
 char buf [] = "I  am a good";
 char *tmpbuf,*desbuf;

 desbuf=tmpbuf=buf;
 
 while ( *tmpbuf )
 {
 	if ( *tmpbuf != ' '  )
       {
	 *desbuf = *tmpbuf;
         desbuf++;
       }					    
	tmpbuf++;
 }
  *desbuf = '\0';
 //remspace ( buf );
 printf ("\n AFTER space removed %s\n", buf );

 }

/*
 char *remspace ( char *src )
 {
  char *tmp,*tmp2;
 //while (*src)
  for ( tmp = tmp2 = src; *src ; src++ )  
 {
    if ( !isspace( *src ) )
    {
       *tmp = *src;
      // src++;
       tmp++;
    }
  	
 }
  *tmp = '\0';

  return ( tmp );

}
*/

