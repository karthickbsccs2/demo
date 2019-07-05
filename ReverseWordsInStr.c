#include <stdio.h>
//#include <conio.h>
#include <stdlib.h>


typedef int INT32;
typedef unsigned int UINT32;
typedef void VOID;
typedef char* PCHAR;
typedef char CHAR;

VOID  MyStrnRev (PCHAR pSrc , PCHAR pDest , INT32 i32Size)
{
   CHAR ch;
   INT32 i32Count = 0;
   printf("\n String  %s/%s/%d\n",pSrc,pDest,i32Size );            
   
   while ( i32Count < i32Size/2 )
   {
         printf("\n *****String  %s/%s/%d\n",pSrc,pDest,i32Size );            
         ch = pSrc [ i32Size - (i32Count +1) ];
         pSrc [ i32Size - (i32Count+1) ] = pSrc [ i32Count ];
         pSrc [ i32Count ] = ch;
         
         i32Count++;

   }
}

INT32 main ()
{
      char   a[ 100 ]          = "I am atb home sweet";
      UINT32 ui8Count          = 0,
             ui8ReverseNeeded  = 0;
      char   *p;
      
      
      while ( ui8Count <= 14  )
      {
            printf("\n String  %s\n", &a[ui8Count] );            
            ui8ReverseNeeded++;
            
            if ( ui8ReverseNeeded == 1 )
            {
               p = &a[ui8Count];
               printf("\n base String  %s/%s\n", &a[ui8Count],p );            
            }
               
            if ( a [ ui8Count ] == ' ' || a [ ui8Count ] == '\0' )
            {
               printf("\n Word gap detected need to reverse ... ui8ReverseNeeded %d/ui8Count %d\n",ui8ReverseNeeded,ui8Count);

               if ( ui8ReverseNeeded > 2 )
               {
                    printf("\n Reverse process needed  %s %s\n",p, &a[ui8Count] );                    
                    
                    MyStrnRev (p,&a[ui8Count-1],(&a[ui8Count] - p) );
                    
                
                }
                ui8ReverseNeeded = 0;
             }
             
                ui8Count++;
      }
            printf("\n Aft String  %s\n", a );                  
	    
  //    getch ();
      return 0;
}

