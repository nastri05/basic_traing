#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>

#include "common.h"
#include <time.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    mqd_t mq;
    char buffer[MAX_SIZE];

    /* open the mail queue */
    mq = mq_open(QUEUE_NAME, O_WRONLY);
    CHECK((mqd_t)-1 != mq);

	srand(time(NULL));   
    printf("Send to server (enter \"exit\" to stop it):\n");

    do {
		int r = rand()%10;    
		printf(" priority : %d   ",r);
        printf("> ");
        fflush(stdout);
		
        memset(buffer, 0, MAX_SIZE);
        fgets(buffer, MAX_SIZE, stdin);
		
        /* send the message */
        CHECK(0 <= mq_send(mq, buffer, MAX_SIZE, r));

    } while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));

    /* cleanup */
    CHECK((mqd_t)-1 != mq_close(mq));

    return 0;
}