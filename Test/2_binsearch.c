/* Program to perform binary search on an array of sorted elements*/

#include<stdio.h>
#include<stdlib.h>

#define FOUND 1
#define NOT_FOUND -1

//Function performing binary search operation with the array, low, high and the search element as arguments
int binary_search(int *a, int low, int high, int key)
{
	int mid;
	//iterate till low becomes greater than high
	while(low <= high)
	{
		mid = (low+high)/2; //Compute the value of mid
		if(a[mid] == key) //When the search element is found
			return FOUND;
		else if(key < a[mid]) //when the search element is less than the mid element, modify high value to mid-1
			high = mid - 1;
		else		     //When the search element is greater than the mid element, modify the low value to mid+1
			low = mid + 1;	
	}

	return NOT_FOUND; //When the element is not found
}

int main()
{
	int a[10], key, size;
	int i, result;
	printf("Enter the size of the array : ");
	scanf("%d",&size);
	printf("Enter %d elements in sorted order\n",size); //Accepting the array elements from the user
	for(i=0; i<size; i++)
		scanf("%d",&a[i]);
	
	printf("Enter the search element :");
	scanf("%d",&key);
	
	result = binary_search(a,0,size-1,key); //Calling binary search for key element
	if(result == FOUND)
		printf("FOUND\n\n");
	else
		printf("NOT FOUND\n\n");
}
