/* Program to demonstrate memset function to initialise an integer array with 0 */

#include <stdio.h>
#include <string.h>

#define MAX 10

int main()
{
	int array[MAX];
	int i;
	memset(array,0,MAX*sizeof(int));
	for(i=0; i<MAX; i++)
		printf("%d\n",array[i]);
}

