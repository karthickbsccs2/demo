/* Program to check the use of conditional compilation statements */

#include <stdio.h>

int main()
{
	int a;

#ifdef PROJECT1
	a = 10;
	printf("In project 1\n");
	printf("Value of a = %d\n",a);
#else
	a = 20;
	printf("in project 2\n");
	printf("Value of a = %d\n",a);
#endif

}

