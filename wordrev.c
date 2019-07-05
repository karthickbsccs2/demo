#include"stdio.h"
int rev ( char * , char * );
int main()
{
 char buf [] = "I am the good one";
 char *end,*x,*y;
 
 for ( end = buf; *end; end++  );
 rev(buf,end-1);
	printf ("\nstr is %s\n",buf);  
	 y = buf -1;
 	x = buf;

	 while ( x++ < end  )
 	{
	  if ( *x == '\0' || *x == ' ' )
          {
		rev ( y,x-1);
                y = x +1;
	  } 	
	}	
          
       	printf ("\nstr is %s\n",buf);  
}

int rev ( char *l , char *r )
{
  char t;
  while ( l < r )
  {
   t = *l;
   *l = *r;
   *r = t;
   l++;
   r--;
 }
}




