/*Simple recursion program to display numbers from 1 to n given n as input */

#include<stdio.h>

void print(int );

int main()
{
	int n;
	printf("Enter n : ");
	scanf("%d",&n);
	print(n);
}

void print(int n)
{
	if(n > 1)
	    print(n-1);

	printf("%d\n",n);
}
