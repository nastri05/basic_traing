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
#define MSG_SIZE 268
#define MAX_MSG 300
#define PAYLOAD_LENGTH 260

// Define type of message
#define REQUEST_MESSAGE_ALL 0
#define REQUEST_MESSAGE_CLIENT 1
#define REQUEST_MESSAGE_GROUP 2
#define RESPONSE_MESSAGE 3
#define CONFIG_MESSAGE 4
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

