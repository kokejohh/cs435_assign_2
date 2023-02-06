#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t ntid;

void printids(const char * s)
{
	pid_t pid = getpid();
	pthread_t tid = pthread_self();
	pid_t t;
	
	printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void *thr_fn(void *arg)
{
	printids("new thread: ");
	return ((void *)0);
}

int main()
{
	int err;

	err = pthread_create(&ntid, NULL, thr_fn, NULL);	
	if (err != 0) printf("can't create thread");
	printids("main thread: ");
	sleep(1);
	return (0);
}
