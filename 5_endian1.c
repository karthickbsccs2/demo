/* Program to check the endianness of a machine using unions */

#include <stdio.h>

union number
{
	int i;
	char first;
};

int main()
{
	union number n;
	n.i = 1;

	if(n.first == 1)
	{
		printf("little endian\n");
		printf("value of the first byte  : %d\n",n.first);
	}
	else
	{
		printf("Big endian\n");

		printf("value of the first byte  : %d\n",n.first);
	}
}
