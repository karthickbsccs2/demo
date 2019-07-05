#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#define MAX_COUNT 10

pthread_t thread1,thread2;
key_t key;
int msgid;

struct message
{
	long type;
	int num;
}sent_msg, recv_msg;

void * thread1_func()
{
	int i;
	for(i=1; i<=MAX_COUNT; i++)
	{
		sent_msg.type = 1;
		sent_msg.num = i;
		msgsnd(msgid, &sent_msg, sizeof(struct message),0);
		printf("Thread1 sent : %d\n",sent_msg.num);
		usleep(1000);
	}
	pthread_exit(0);
}

void* thread2_func()
{
	int i;
	for(i=1; i<=MAX_COUNT; i++)
	{
		msgrcv(msgid, &recv_msg, sizeof(struct message), 0, 0);
		printf("Thread2 recieved : %d\n",recv_msg.num);
	//	sleep(1000);
	}
	pthread_exit(0);
}

int main()
{	
	key = ftok("sem.c",65);
	msgid = msgget(key, IPC_CREAT|0666);
	
	if(pthread_create(&thread1, NULL, &thread1_func, NULL))
	{
		printf("Thread 1 cannot be created\n");
		exit(1);
	}
	if(pthread_create(&thread2, NULL, &thread2_func, NULL))
	{
		printf("Thread 2 cannot be created\n");
		exit(1);
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}
