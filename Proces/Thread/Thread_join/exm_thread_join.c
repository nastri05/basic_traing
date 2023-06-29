#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *myThreadFun(void *vargp)
{
	sleep(1);
	printf("Printing GeeksQuiz from Thread %s \n" ,(char*)vargp);
	return NULL;
}

int main()
{
	pthread_t thread_id_1,thread_id_2;
	printf("Before Thread\n");
	pthread_create(&thread_id_1, NULL, myThreadFun, (char*)"hello_1");
	pthread_create(&thread_id_2, NULL, myThreadFun, (char*)"hello_2");
	//sleep(1);
	pthread_join(thread_id_1, NULL);
	pthread_join(thread_id_2, NULL);
	printf("After Thread\n");
	exit(0);
}
