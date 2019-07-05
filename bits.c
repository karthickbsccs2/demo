#include "bits.h"
#include<stdio.h>
#define CHAR_SIZE (sizeof(char)*8)

/* Function that prints the bit pattern of a character */
void bit_representation(char c)
{
	int i;
	int mask;

	/* Mask is applied to each character to know the bit value */
	for(i = CHAR_SIZE-1; i >=0 ; i--)
	{
		mask = 1 << i;
		if((c & mask) == mask)
			printf("1");
		else
			printf("0");
	}
	printf("\n");
}

/* Function to unset a bit in a particular position : if 1, set to 0 else no change*/
char bit_unset(char c, int pos)
{
	int mask;
	mask = 1<<pos;
	c = c & ~(mask);
	return c;
}

/*m Function to toggle the bits except for the bit position specified */
char bit_toggle(char c, int pos)
{
	int mask;
	mask = 1<<pos;
	c = c ^ ~(mask);
	return c;
}

/* Function to swap nibbles in an 8 bit character variable */
char nibble_swap(char c)
{
	char temp1,temp2;
	temp1 = c & 0x0F;
	temp2 = c & 0xF0;
	temp1 = temp1 << 4;
	temp2 = temp2 >> 4;
	return(temp1|temp2);	
}
#if 0
int main()
{
	char c = 'C';
	bit_representation(c);
}
#endif
