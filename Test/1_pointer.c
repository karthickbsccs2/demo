/* Program to undersatnd the concept of constant pointer and pointer to a constant*/
#include <stdio.h>

int main()
{
	int a = 10,b;
	int* const p = &a; //Constant pointer
	//p = &b; Gives an error when attempted to change the value of a constant pointer

	const int * q = &a;
	//*q = 20; Gives an error when attempted to modify the value of the constant to which the pointer is pointing to

	q = &b; //Valid

}
