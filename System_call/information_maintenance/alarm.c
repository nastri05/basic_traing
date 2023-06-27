#include <stdio.h>
#include <unistd.h>
#include <signal.h>
// 1 s function sigalrm_handler callded

void sigalrm_handler(int);

int  main(void)
{
    signal(SIGALRM, sigalrm_handler);   
    alarm(1);                         
    while (1);  
}

void sigalrm_handler(int sig)
{
    printf("Alarm clock \n");
	exit(0);
}