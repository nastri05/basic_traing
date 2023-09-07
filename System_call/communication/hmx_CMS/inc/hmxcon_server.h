#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<string.h>

#include"hmxcms_linkedlist.h"

#define API_SERVER_SEND_MESSAGE "/cms_server"

#define TRUE 1
#define FALSE 0


typedef struct {
    int tag;
    int length;
     struct payload
    {
        char name[32];
        char value[32];
        int mqID;
        int type;
        int flag;
    } payload;
} mesg_buffer;

typedef struct 
{
    /* data */
    mesg_buffer * mesg;
    List_Client ** list;
} arg;
