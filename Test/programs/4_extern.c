/* Program to understand the usage of extern storage class.
It should be compiled as follows
	gcc var_a.c 4_extern.c

*/

#include <stdio.h>

int main()
{
	extern int a;
	printf("value of the extern variable : %d\n",a);
}
