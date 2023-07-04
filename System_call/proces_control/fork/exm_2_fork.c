#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int value;
void forkexample()
{
    // child process because return value zero
    if (fork() == 0)
    {	
        printf("Hello from Child!\n");
		printf("Gia tri child before la : %d\n",&value);
		value = 15;
		printf("Gia tri child after la : %d\n",&value);
	return;
    }
  
    // parent process because return value non-zero.
    else
    {
		value = 10;
		printf("Gia tri parent before la : %d\n",&value);
		//	int value =fork();
//		printf("Value of fork = %d \n",fork() );
        printf("Hello from Parent!\n");
		wait(NULL);
		printf("Gia tri parent after la : %d\n",value);
	return;
    }
}
int main()
{
    forkexample();
    return 0;
}
