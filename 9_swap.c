/* Program to swap the nibbles in a character variable
It should be compiled as follows
	gcc 9_swap.c bits.c
*/

#include <stdio.h>
#include "bits.h"

int main()
{
	char c;
	printf("Enter a character : ");
	scanf("%c",&c);
	printf("Before swapping: ");
	bit_representation(c);
	c = nibble_swap(c);
	printf("\nAfter swapping : ");
	bit_representation(c);
}
