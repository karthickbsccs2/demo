/* program to unset a particular bit in a character.
It should be compiled as follows
	gcc 7_unset.c bits.c
*/

#include<stdio.h>
#include "bits.h"


int main()
{
	char c, p;
	int pos;
	printf("Enter a character : ");
	scanf("%c",&c);
	printf("Enter the position of the bit to be unset : ");
	scanf("%d",&pos);

	printf("Before unsetting\n");
	bit_representation(c);

	p = bit_unset(c,pos);

	printf("\nAfter unsetting the bit at position %d\n",pos);
	bit_representation(p);
	printf("\n");

}
