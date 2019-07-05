#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#define MAX_COUNT 10
#define TCOUNT 5

int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem1, sem2;

void *function1(void* tid)
{
	int i;
	int id = (int) tid;
	pthread_t wait_thread;
	printf("Function 1\n");
	for(i=1; i<=TCOUNT; i++)
	{
		/*if(count == MAX_COUNT)
		{
			pthread_exit(NULL);
		}*/
		printf("\nFunction 1 before sem1 wait\n");
		sem_wait(&sem1);
		pthread_mutex_lock(&mutex);
		count++;
		printf("Thread%d : %d\n",id,count);
		pthread_mutex_unlock(&mutex);
		sem_post(&sem2);
		printf("Function 1 after sem2 post\n\n");
		//if(count < MAX_COUNT)
		//pthread_mutex_unlock(&mutex);
	}
}

void *function2(void* tid)
{
	int i;
	int id = (int) tid;
	printf("Function 2\n");
	for(i=1; i<=TCOUNT; i++)
	{
		printf("\nFunction 2 before sem2 wait\n");
		sem_wait(&sem2);
		pthread_mutex_lock(&mutex);
		count++;
		printf("Thread%d : %d\n",id,count);
		pthread_mutex_unlock(&mutex);
		sem_post(&sem1);
		printf("Function 2 after sem1 post\n\n");
	}
}

int main()
{
	pthread_t thread1, thread2;
	int tid1 = 1, tid2 = 2;
	int i;
	if(sem_init(&sem1,0,1))
	{
		printf("Semaphore cannot be initialized\n");
		exit(1);
	}

	if(sem_init(&sem2,0,1))
	{
		printf("Semaphore2 cannot be created\n");
		exit(1);
	}

	if(pthread_create(&thread1, NULL, &function1,(void *)tid1))
	{
		printf("Error in creation of thread1\n");
		exit(1);
	}
	
	if(pthread_create(&thread2, NULL, &function2, (void *)tid2))
	{
		printf("Error in creation of thread2\n");
		exit(1);
	}

	//pthread_join(wait_thread, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	sem_destroy(&sem1);
	sem_destroy(&sem2);

	return 0;	
}

