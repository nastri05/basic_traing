#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("PID of main_program.c = %d\n", getpid());
    char *args[] = {"Hello", "C", "Programming", NULL};
    execv("./sub_program", args);
    printf("Back to example.c");
    return 0;
}
