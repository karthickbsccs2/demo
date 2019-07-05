#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>
int g_var; /* Global variablr*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem1;
void * display(void * msg)
{
	int l_var = 0,i=0;
	char * message = (char *)msg;
	for(i=0;i<10;i++)
	{
		l_var++;
		sem_wait(&sem1);
		//pthread_mutex_lock(&mutex);
		g_var++;
		printf("%s, global variable : %d, local variable : %d i=%d \n",message,g_var,l_var,i);
		//pthread_mutex_unlock(&mutex);
		sem_post(&sem1);
	}
	pthread_exit(NULL);
}

void * display(void * msg)
{
	int l_var = 0,i=0;
	char * message = (char *)msg;
	for(i=0;i<10;i++)
	{
		l_var++;
		sem_wait(&sem1);
		//pthread_mutex_lock(&mutex);
		g_var++;
		printf("%s, global variable : %d, local variable : %d i=%d \n",message,g_var,l_var,i);
		//pthread_mutex_unlock(&mutex);
		sem_post(&sem1);
	}
	pthread_exit(NULL);
}
int main()
{
	pthread_t thread1, thread2;
	int ret1, ret2;
	char *msg1 = "Thread 1 executed";
	char *msg2 = "Thread 2 executed";
    
    sem_init(&sem1,0,1);
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
