#include"stdio.h"
int main()
{
 int a = 1;

 if (*(char *)&a == 1  )
 {
   printf ("\nlittle\n");
 }
 else
 {
  printf ("\nbig\n");
 }




}
