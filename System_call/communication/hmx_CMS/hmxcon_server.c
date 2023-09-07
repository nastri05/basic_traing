#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <pthread.h>

#include "inc/hmxcon_server.h"

void *handle_add_client(void *data){
    arg *args = (arg*)data;
    data_t * data = create_data(args->mesg->payload.name,args->mesg->payload.)
    add_client(args->list,args->mesg->payload.mqID,args->mesg->payload.name);
    pthread_exit(NULL);
    free(args);
}

int main()
{
    mqd_t mq;
    struct mq_attr attr;
    mesg_buffer * buffer;
    List_Client * List_Client = NULL;
    arg *args =(arg*) malloc(sizeof(arg));

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(mesg_buffer);
    attr.mq_curmsgs = 0;

    /* create the message queue */
 	  // mq_unlink(API_SERVER_SEND_MESSAGE);
    mq = mq_open(API_SERVER_SEND_MESSAGE, O_CREAT | O_RDWR , 0666, &attr);
    buffer = (mesg_buffer*) malloc(sizeof(mesg_buffer));
    if(mq < 0){
		  printf(" open fail \n");
		  return 0;
	    }
	  printf(" size of mesg_buffer = %d \n",mq);
    do {
		  sleep(1);
      ssize_t bytes_read;
      
      bytes_read = mq_receive(mq, (char*)buffer,sizeof(mesg_buffer), NULL); // null is priority
		  
      if(bytes_read >0 ){
			  printf("bytes_read number     = %d \n",bytes_read);
        printf("value of payload 1  = %d \n",buffer->tag);
        printf("value of payload 2 = %s \n",buffer->payload.value);
        printf("value of payload 3 = %s \n",buffer->payload.name);
        printf("value of length = %d \n",buffer->length);

        if(strncmp(buffer->payload.name, "exit",4*sizeof(char))==0){
          printf("Exit process server \n");
          break;
          
        }
        if(buffer->tag == 1){
          printf("Tag = 1 \n");
          pthread_t thread_id;
          args->mesg = buffer;
          args->list = &List_Client;
          int result = pthread_create(&thread_id, NULL, handle_add_client,(arg*)args);
          if(result != 0){
            printf("Creater thread fail \n");
            free(args);
            }
          pthread_detach(thread_id);
          }
          else{
          PrintList(List_Client);
        }
		  }
      else{
        printf("value of bytes_read = %d", bytes_read);
      }
    } while (1);

	  free(buffer);
    mq_close(mq);
    printf("Close program \n");
    mq_unlink(API_SERVER_SEND_MESSAGE);

    return 0;
}
