#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/wait.h>  /* Needed for the wait function */
#include <unistd.h>    /* needed for the fork function */
#include <string.h>    /* needed for the strcat function */
#define SHMSIZE 27
int main() {
	int shmid;
	char *shm;
	/* Variable s removed, it wasn't used */
      /* Removed first call to wait as it held up parent process */
    shmid = shmget(2010, SHMSIZE, 0666 | IPC_CREAT);
    shm = (char*)shmat(shmid, 0, 0);
      //wait(NULL);
    printf ("Parent reads <%s>\n",shm) ;
	  //detach from 
    shmdt(shm);
	  // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);
	return 0;
}