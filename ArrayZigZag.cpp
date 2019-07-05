#include "stdio.h"
#include "conio.h"
#include  "stdlib.h"

typedef int UINT32;
typedef signed char BOOLEAN;
typedef unsigned char UINT8;
typedef void VOID;

#define ROW    6
#define COLUMN 6

UINT32 main ( )
{
 UINT32 aui32Array [ ROW ] [ COLUMN ] = { 1, 2, 3, 4,5,6,
                                          7, 8, 9, 10,11,12,
                                          11,12,13,14,15,16,                                          
                                          17,18,19,20,21,22,
                                          23,24,25,26,27,28,
                                          29,30,31,32,33,34 };
                                          
 UINT32 tra,row,col,ui32Dummy,ui32RowLimit,ui32ColumnLimit,ui32Cnt=1;
 #if 0
 row = ROW-1;
 col = 0;
 tra = 1;
 ui32RowLimit =ROW -1; 
 ui32ColumnLimit = 0;
               printf ("****%d****ROW %d COL %d\n",aui32Array[ row ] [ col ],row,col);                         
 col++;
  while ( !((row==0)&(col==COLUMN-1)))
  {
        //printf("\n Start tra %d row %d col %d\n",tra,row,col);        
        if ( tra%2 == 0 )
        {   
        
        //     printf("\n 1st tra %d row %d col %d ui32RowLimit %d\n",tra,row,col,ui32RowLimit);                                    

             for (;row<=ui32RowLimit;row++,col++)
             {
               printf ("****%d****ROW %d COL %d\n",aui32Array[ row ] [ col ],row,col);                         
               
               }
      //         printf("\nRow %d Col %d\n",row,col);

               if (col==COLUMN)
               {
                 col--;
                 row = row - 2;
                 ui32ColumnLimit=ui32ColumnLimit+ui32Cnt;
                 if (ui32Cnt==1)
                 ui32Cnt++;
                 }
                else if (row==ROW)
                 row--;
                else
                {
                }

        }
        else
        {
    //         printf("\n 2nd tra %d row %d col %d  ui32ColumnLimit %d\n",tra,row,col,ui32ColumnLimit);                    
             for (; col>=(ui32ColumnLimit);row--,col--)
             {
               printf ("****%d****ROW %d COL %d\n",aui32Array[ row ] [ col ],row,col);                         
      
               }
                 //  printf("\nRow %d Col %d\n",row,col);

               if (row<0)
               {
                 ++row;
                 col = col +2;
                 ui32RowLimit=ui32RowLimit-ui32Cnt;
                 if (ui32Cnt==1)
                 ui32Cnt++;
                 }
                 else if (col<0)
                 ++col;
                 else
                 {
                 }

                 
        }
        tra++;
  }
   printf ("****%d****ROW %d COL %d\n",aui32Array[ row ] [ col ],row,col);  
#else
 row = 0;
 col = 0;
 
 tra = 1;
 ui32RowLimit =1; 
 ui32ColumnLimit = 1;
 printf ("****%d****ROW %d COL %d\n",aui32Array[ row ] [ col ],row,col);                         
 row++;
 
  while ( !((row==ROW-1)&(col==COLUMN-1)))
  {
       printf("\n Start tra %d row %d col %d\n",tra,row,col);        
        if ( tra%2 == 0 )
        {
             printf("\n 1st tra %d row %d col %d ui32RowLimit %d\n",tra,row,col,ui32RowLimit);                                    

             for (;row<=ui32RowLimit;row++,col--)
             {
               printf ("****%d****ROW %d COL %d\n",aui32Array[ row ] [ col ],row,col);                         
             }
               printf("\nRow %d Col %d\n",row,col);

               if (row==ROW)
               {
                 col+=2;
                 --row;
                 ui32ColumnLimit = COLUMN-1;
                 
                 }
                else if (col<0)
                {
                 col++;
                 ui32ColumnLimit=ui32ColumnLimit+ui32Cnt;
                 if (ui32Cnt==1)
                 ui32Cnt++;
                 }
                else
                {
                }

        }
        else
        {
             printf("\n 2nd tra %d row %d col %d  ui32ColumnLimit %d\n",tra,row,col,ui32ColumnLimit);                    
             for (; col<=(ui32ColumnLimit);row--,col++)
             {
               printf ("****%d****ROW %d COL %d\n",aui32Array[ row ] [ col ],row,col);                         
             }
                if (col==COLUMN)
                 {
                      row+=2;
                      --col;
                      ui32RowLimit = ROW-1;
                 }
                 else if (row<0)
                {
                 ++row;
                 ui32RowLimit=ui32RowLimit+ui32Cnt;                
                 if (ui32Cnt==1)
                 ui32Cnt++;
                 }
                 else
                 {
                 }

                 
        }
        tra++;
  }
   printf ("****%d****ROw %d COL %d\n",aui32Array[ row ] [ col ],row,col);                         
                          
#endif

printf("\nFinished\n");
scanf("%d",&tra);
}

