#include<stdio.h>
#include<string.h>
#define revserestring
int  myrevfun(char *,char *);
int  reverseachword( char * );

int main()
{
 char mystring [] = "I am a justin";
 char *revstring,*cpystring;
 char *tmpstring;

  for (revstring = mystring; *revstring;revstring++ );
  myrevfun(mystring,revstring-1);

  cpystring  = mystring-1;
  tmpstring  = mystring;
 
   while ( *cpystring )
   { 
        printf ("\n****again reverse called****\n");
        cpystring++;  
 	if ( *cpystring == '\0' || *cpystring == ' ' )
 	{
          myrevfun(tmpstring,cpystring-1 );
   	  tmpstring = cpystring+1;
	}
       	
  }

  printf ("\nOnce again modified string is %s\n",mystring);
 return 0;
}

int myrevfun ( char *mystring, char *revstring )
{
 char tmpstring;

 while (mystring < revstring)  
 {
   tmpstring = *mystring;
   *mystring = *revstring;
   *revstring = tmpstring;
   *mystring++;
   *revstring--; 
 }
} 
