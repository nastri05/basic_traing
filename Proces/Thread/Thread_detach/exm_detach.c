#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *myThreadFun_2(void *vargp)
{
	printf("Starting concurrent thread.\n");
	sleep(2);
	printf("Exiting concurrent thread.\n");
	return NULL;
}
void *myThreadFun(void *vargp)
{
	pthread_t thread_id_2;
	printf("Starting thread caller.\n");
	pthread_create(&thread_id_2, NULL, myThreadFun_2, (char*)"hello_2");
	pthread_detach(thread_id_2);
	sleep(1);
	printf("Exiting thread caller.\n");
	return NULL;
}

int main()
{
	pthread_t thread_id_1;
	printf("Before Thread\n");
	pthread_create(&thread_id_1, NULL, myThreadFun, (char*)"hello_1");
	printf("After Thread\n");
	sleep(5);
	printf("After Thread again\n");
	//pthread_exit(NULL);
	exit(0);
}
