#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Let us create a global variable to change it in threads
int g = 0;

// The function to be executed by all threads
void *myThreadFun(void *vargp)
{
	// Store the value argument passed to this thread
	int *myid = (int *)vargp;

	// Let us create a static variable to observe its changes
	static int s = 0;

	// Change static and global variables
	++s; ++g;

	// Print the argument, static and global variables
	printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g);
	//while(1){}
}

int main()
{
	int i;
	pthread_t tid;

	// Let us create three threads
	//pthread_exit(NULL);
	for (i = 0; i < 3; i++)
		pthread_create(&tid, NULL, myThreadFun, (void *)&tid);
		//pthread_join(tid, NULL);
	pthread_exit(NULL);
	printf("The End \n"); // not printf because call pthread_exit() after this
	return 0;
}
