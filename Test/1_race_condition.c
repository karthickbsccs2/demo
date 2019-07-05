#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int g_var; /* Global variablr*/

void * display(void * msg)
{
	int l_var = 0;
	char * message = (char *)msg;
	l_var++;
	g_var++;
	printf("%s, global variable : %d, local variable : %d\n",message,g_var,l_var);
	pthread_exit(NULL);
}

int main()
{
	pthread_t thread1, thread2;
	int ret1, ret2;
	char *msg1 = "Thread 1 executed";
	char *msg2 = "Thread 2 executed";

	ret1 = pthread_create(&thread1, NULL, (void *)display, (void *)msg1);
	if(ret1 != 0)
	{
		printf("Error in creation of thread1\n");
		exit(1);
	}

	ret2 = pthread_create(&thread2, NULL, (void *)display, (void *)msg2);
	if(ret2 != 0)
	{
		printf("Error in creation of thread2\n");
		exit(1);
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	return 0;
}
