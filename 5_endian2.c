/* Program to check the endianness of a machine using pointers */

#include<stdio.h>

int main()
{
	int num = 1;
	char *c = (char *)&num;

	if(*c == 1)
		printf("little endian\n");
	else
		printf("Big endian\n");
}
