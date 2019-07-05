/*Program to allocate a chunk of memory to a character pointer*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Function that allocates memory to a character pointer*/
void allocate(char **c, int size)
{
	*c = malloc(size*sizeof(char));
}

int main()
{
	char *c;
	int size;
	printf("Enter the size :");
	scanf("%d",&size);
	allocate(&c,size); //calling the allocate function with address of the character pointer and size.
	strcpy(c,"Hello");
	printf("%s\n",c);
}
