#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>

#include "inc/hmxcms_lib.h"

#define MAX_SIZE 32


int main()
{
	cms_msg_t *message;
	mqd_t mq;
	struct mq_attr attr ;
	/* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(cms_msg_t);
    attr.mq_curmsgs = 0;

	char* name_client = (char*) malloc(32*sizeof(char));
	message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    /* create the message queue */
	//mq_unlink(API_SERVER_SEND_MESSAGE);
    mq = mq_open(SERVER_QUEUE_NAME, O_RDWR , 0444, &attr);
	printf(" size of mesg_buffer = %d \n",mq);
	if(mq < 0){
		  printf(" open fail \n");
		  return 0;
	    }
	do {
		
        printf("> ");
        fflush(stdout);
		memset(name_client, 0, MAX_SIZE);
        fgets(name_client, MAX_SIZE, stdin);
		create_msg(message,1,"hello",name_client,getpid(),"Wan","nothing");
      	printf("value of payload 1 = %s \n",message->payload.client_name);
		printf("value of payload 2 = %s \n",message->payload.topic);
		printf("value of payload 3 = %d \n",message->tag);
		printf("value of sizeof = %d \n\n",sizeof(mesg_buffer));
		printf("value of obj = %s\n\n",(char*)message);
        /* send the message */
        if(0 <= mq_send(mq, (char*)message, sizeof(mesg_buffer), 0)){
			printf("send complete \n");
		}
		if(strncmp(message->payload.name, "exit",4*sizeof(char))==0){
			printf("Exit process process \n");
          	break;
		}

    } while (1);
	free(message);
	mq_close(mq);
    //mq_unlink(API_SERVER_SEND_MESSAGE);
	return 0;
}