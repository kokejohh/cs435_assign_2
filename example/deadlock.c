#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t v1, v2;

void *t(void *arg)
{
	long int id = (long)arg;
	if (id == 1)
	{
		pthread_mutex_lock(&v1);
		sleep(2);
		pthread_mutex_lock(&v2);
		printf("enter critical section of t(1)\n");
		pthread_mutex_unlock(&v2);
		pthread_
	}
}
