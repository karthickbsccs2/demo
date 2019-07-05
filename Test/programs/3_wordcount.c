/* program to count the number of words in file.
This program takes 'test.txt' as the input file
*/
#include<stdio.h>

int main()
{
	FILE *fp;
	char c;
	int wc = 0;
	fp = fopen("test.txt","r"); //Open test.txt file in the read mode.
	while((c = fgetc(fp)) != EOF) //Extract each character of the file till it reached End Of File
	{
		//Increment the word count when a space, tab space or a new line is encountered.
		if(c == ' ' || c == '\t' || c == '\n')
			wc++;
	}

	printf("Word count : %d\n",wc);
	fclose(fp); //File close
}
