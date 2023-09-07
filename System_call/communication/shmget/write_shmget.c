#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/wait.h>  /* Needed for the wait function */
#include <unistd.h>    /* needed for the fork function */
#include <string.h>    /* needed for the strcat function */
#define SHMSIZE 27
int main()
{
	int shmid;
	char *shm;
	shmid = shmget(2010, SHMSIZE,0666|IPC_CREAT);
	  if(shmid < 0){
		  printf(" Error \n");
		  return -1;
	  }
	  
	  //shmat to attach to shared memory
      shm = shmat(shmid, 0, 0);	  
      char *s = (char *) shm;
      *s = '\0';  /* Set first location to string terminator, for later append */
      /*
	  int i;
      for(i=0; i<5; i++) {
         int n;  
		// Variable to get the number into
         printf("Enter number<%i>: ", i);
         scanf("%d", &n);

         char number[20];
         sprintf(number, "%d", n); 
		 // Convert the number to string
         strcat(s, number);  
		 // Append the number to the string
      }
	  */
	  strcat(s,"who are you");
      //strcat(s, "\n");  /* Append newline */
      printf ("Child wrote < %s >\n",shm);
	  //detach from memory
      shmdt(shm);
	  return 0;
}
