#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
        //A null terminated array of character
        //pointers
        //char *args[]={"ls-l",NULL};
        char* command = "ls";
		char* argument_list[] = {"ls", "-l", NULL};
		execvp(command,argument_list);
     
        /*All statements are ignored after execvp() call as this whole
        process(execDemo.c) is replaced by another process (EXEC.c)
        */
        printf("Ending-----");
     
    return 0;
}