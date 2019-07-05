#include <stdio.h>
#include <limits.h>

int main()
{
	printf("Number of bits in a char : %d\n",CHAR_BIT);
	printf("Minimum value of a signed character : %d\n",SCHAR_MIN);
	printf("Maximum value of a signed character : %d\n",SCHAR_MAX);
	printf("Maximum value of an unsigned short int : %d\n",USHRT_MAX);

	printf("Minimum value of a long integer : %d\n",LONG_MIN);
	printf("Maximum value of a long integer : %d\n",LONG_MAX);
}
