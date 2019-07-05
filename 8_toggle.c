/*Program to toggle all the bits in a character expect the bit at a specified position 
It should be compiled as follows
gcc 8_toggle.c bits.c
*/

#include <stdio.h>
#include "bits.h"

int main()
{
	char c;
	int pos;
	printf("Entet a character : ");
	scanf("%c",&c);
	printf("Enter position :");
	scanf("%d",&pos);
	printf("Before toggling : ");
	bit_representation(c);
	c = bit_toggle(c,pos);
	printf("\nAfter toggling : ");
	bit_representation(c);
}
