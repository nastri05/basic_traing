#ifndef HMX_CMS_LIB_H
#define HMX_CMS_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

// Define Server Queue
#define SERVER_QUEUE_NAME "/HmxCmsServer"
#define FLAG 0
#define MSG_SIZE sizeof(cms_msg_t)
#define MAX_MSG 10
#define PAYLOAD_LENGTH sizeof(cms_payload_t)

// Define type of message
#define REQUEST_MESSAGE_ALL      0
#define REQUEST_MESSAGE_CLIENT   1
#define REQUEST_MESSAGE_GROUP    2
#define RESPONSE_MESSAGE         3
#define CONFIG_MESSAGE           4
#define SEND_REQUEST             5
#define RECEIVE_REQUEST          6
// Define response data
#define CMS_CONFIG_SUCCESS "config_success"
#define CMS_CONFIG_FAIL "config_fail"
#define CMS_SEND_SUCCESS "send_success"
#define CMS_SEND_FAIL "send_fail"
// Define config data
#define ADD_CLIENT "add"
#define REMOVE_CLIENT "remove"


#define CMS_SUCCESS 0
#define CMS_FAIL 1
#define CMS_ERROR -1


typedef struct {
    char client_name[64];
    char mq_name[64];
    int type;
    char topic[64];
    char data[64];
} cms_payload_t;

typedef struct {
    int tag;
    int length;
    cms_payload_t payload;
} cms_msg_t;

mqd_t cms_client_init(struct mq_attr* mq_attr,char* name_client, char* mq_name, int type, char *topic);
int cms_client_close(mqd_t mqdes,char* name_client, char* mq_name);
int cms_send(mqd_t mqdes, int tag, char* name_client, char* mq_name, int type, char* topic, char* data);
int cms_receive(mqd_t mqdes, cms_msg_t* cms_msg);
int set_mq_attr(struct mq_attr* attr, long flag, long maxmsg, long msgsize);
int create_msg(cms_msg_t* msg, int tag, char* name_client, char* mq_name, int type, char* topic, char* data);

#endif

