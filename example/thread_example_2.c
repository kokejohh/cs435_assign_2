#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_fn(void *arg)
{
	printf("thread 1 returning\n");
	return ((void *)1);
}

void *thr_fn2(void *arg)
{
	printf("thread 2 existing\n");
	pthread_exit((void *)2);
}

int main(void)
{
	int		err;
	pthread_t 	tid1, tid2;
	void		*tret;
	
	err = pthread_create(&tid1, NULL, thr_fn, NULL);
	if (err != 0) printf("can't create thread 1");
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0) printf("can't create thread 2");
	err = pthread_join(tid1, &tret);
	if (err != 0) printf("can't join with thread 1");
	printf("thread 1 exit code %ld\n", (long)tret);
	err = pthread_join(tid2, &tret);
	printf("thread 2 exit code %ld\n", (long)tret);
	exit(0);
}
