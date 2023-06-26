#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
void forkexample()
{
    // child process because return value zero
    if (fork() == 0)
    {
        printf("Hello from Child!\n");
	return;
    }
  
    // parent process because return value non-zero.
    else
    {	
		exit(0);
        printf("Hello from Parent!\n");
	return;
    }
}
int main()
{
    forkexample();
    return 0;
}
