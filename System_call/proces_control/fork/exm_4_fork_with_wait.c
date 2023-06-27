#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void forkexample()
{
    // child process because return value zero
    if (fork() == 0)
    {
        printf("Hello from Child!\n");
	sleep(3);
	printf("Child is Done\n");
	return;
    }
  
    // parent process because return value non-zero.
    else
    {
	printf("Hello from Parent. Waiting for Child!\n");
	wait(NULL); // wait wwhen sub-process exit() then continue
        printf("Parent is Done!\n");
	return;
    }
}
int main()
{
    forkexample();
    return 0;
}




