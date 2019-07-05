#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

typedef int INT32;
typedef unsigned int UINT32;
typedef char CHAR;
typedef char * PCHAR;
typedef void VOID;

typedef struct node
{
        INT32 i32value;
        struct node *pstNext;
}List;

VOID AddNode (List **Head)
{
     List *temp=NULL;
     temp=(List *)malloc(sizeof(List));
     if (temp == NULL )
     {
              printf("\nReverseList.C %d> Memory allocation failed\n",__LINE__);
              return ;
     }
      printf("\nReverseList.C %d> Enter the value\n",__LINE__);
      scanf("%d",&temp->i32value);
      temp->pstNext=NULL;
      if (*Head== NULL)              
      {
         printf("\nReverseList.C %d> Head node added\n",__LINE__);
         *Head = temp;
      }
      else
      {
          List *temp1=*Head;
		  while (temp1->pstNext!=NULL)
		  temp1=temp1->pstNext;
          
          temp1->pstNext=temp;          
      }
      printf("\nReverseList.C %d> Node created succssfully\n",__LINE__);      
 }
 VOID Display (List **Head)
 {
      if (*Head== NULL)              
      {
         printf("\nReverseList.C %d> DBASE is empty\n",__LINE__);
      }
      else
      {
          List *temp1=*Head;
          while (temp1!=NULL)
		  {
          printf("\nReverseList.C %d> Value %d\n",__LINE__,temp1->i32value);
		  temp1=temp1->pstNext;
		  }
          

      }
      
  }
  VOID ReverseNode (List **Head)
  {
      if (*Head== NULL)              
      {
         printf("\nReverseList.C %d> DBASE is empty\n",__LINE__);
      }
      else
      {
          List *p,*q,*r;
          p=q=r=NULL;
          p=*Head;
          q=p->pstNext;
          p->pstNext=NULL;
          while (q!=NULL)
          {
                printf("\nReverseList.C %d> Value %u/%u/%u\n",__LINE__,p,q,r);                
                r=q->pstNext;
                q->pstNext=p;
                p=q;
                q=r;
          }
          *Head=p;
      }
        
  }
List *RecursiveReverse(List *Current,List *Head)
{
     List *ret;
     printf("\nReverseList.C %d> Value %u/%u\n",__LINE__,Current,Head);                
     if (Current == NULL )
     {
                 getch();
         return NULL;
     }
     ret = NULL;
     if ( Current->pstNext!=NULL)
     {
          ret = RecursiveReverse (Current->pstNext,Current);
     printf("\nReverseList.C %d> Value %u/%u\n",__LINE__,Current,Head);                          
      }
      else
      {
          ret = Current;
               printf("\nReverseList.C %d> Value %u/%u\n",__LINE__,Current,Head);                
      }
      Current->pstNext=Head;
           printf("\nReverseList.C %d> Value %u/%u\n",__LINE__,Current,Head);                
                            getch();
      return ret;
 }
List *AddRecursive (List *Number1,List *Number2, List *Number3)
{
     int Add,Carriage;
     
     if ( ( Number1->pstNext == NULL ) ||  (Number2->pstNext== NULL))
     {
          Add = Number1->i32Value + Number2-> i32Value;
          
          Carriage = Add%10;
          Add=Add/10;
          InsertNode (Number3,Add);
          return Number3;
     }
     
 }
INT32 main ()
{
      List *Number1=NULL,*Number2=NULL,*Number3=NULL;
      List *head=NULL;
      
      AddNode(&head);
      AddNode(&head);
      AddNode(&head);
      AddNode(&head);
      
      Display (&head);
      ReverseNode (&head);
      Display (&head);
      
      head= RecursiveReverse(head,NULL);
      Display (&head);
      /* Addition of 2 numbers*/      
      
      AddNode(&Number1);
      AddNode(&Number1);
      AddNode(&Number1);


      Display (&Number1);
      AddNode(&Number2);
      AddNode(&Number2);
      AddNode(&Number2);
      Display (&Number2);
      
      AddRecursive (Number1,Number2,Number3);
      Display (&Number3);      
      
      getch();
      return 0;
}
