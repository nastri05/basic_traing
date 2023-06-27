#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("We are in sub_program.c\n");
    printf("PID of sub_program.c = %d\n", getpid());
    return 0;
}




