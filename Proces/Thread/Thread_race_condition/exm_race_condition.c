#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
int i = 0;
void *myThreadFun(void *arg){
	for(i=0;i<100;i++){
		printf("value of i  = %d in thread %d \n",i,(int*)arg);
	}
}
int main()
{
	pthread_t thread_id_1, thread_id_2;
	printf("Before Thread\n");
    pthread_create(&thread_id_1, NULL, myThreadFun, (int*)1);
	pthread_create(&thread_id_2, NULL, myThreadFun, (int*)2);
    pthread_join(thread_id_1, NULL);
	pthread_join(thread_id_2, NULL);
    printf("After Thread\n");
    exit(0);
	return 0;
}