#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

//check pid of process child and parrent
void forkexample()
{
    // child process because return value zero
    if (fork() == 0)
    {
        printf("Hello from Child!\n");
		printf(" Process ID : %d \n", getpid());
	return;
    }
  
    // parent process because return value non-zero.
    else
    {
		//	int value =fork();
        printf("Hello from Parent!\n");
		printf(" Process ID : %d \n", getpid());
	return;
    }
}
int main()
{
    forkexample();
    return 0;
}