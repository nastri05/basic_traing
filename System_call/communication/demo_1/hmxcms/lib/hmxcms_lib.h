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

#define DEBUG_LIB

#ifdef DEBUG_LIB
#define STR(x)   #x
#define LOG_CLIENT(x) printf("[HMX_CMS_LIB] [%s]         %s = %s\n",__FUNCTION__,STR(x),x)
#define LOG_CLIENT_INT(x) printf("[HMX_CMS_LIB] [%s]         %s = %d\n",__FUNCTION__,STR(x),x)
#define LOG_CLIENT_STATE(x) printf("[HMX_CMS_LIB] [%s]   %s",__FUNCTION__, x)
#else
#define LOG_CLIENT(x)
#define LOG_CLIENT_INT(x)
#define LOG_CLIENT_STATE(x)
#endif

// Define Server Queue
#define SERVER_QUEUE_NAME       "/HmxCmsServer"
#define FLAG                    0
#define MSG_SIZE                sizeof(cms_msg_t)
#define MAX_MSG                 10


// Define response data
#define CMS_CONFIG_SUCCESS       "config_success"
#define CMS_CONFIG_FAIL          "config_fail"
#define CMS_SEND_SUCCESS         "send_success"
#define CMS_SEND_FAIL            "send_fail"
// Define config data
#define ADD_CLIENT               "add"
#define REMOVE_CLIENT            "remove"
#define CHANGE_CLIENT            "change"



#define CMS_SUCCESS              0
#define CMS_FAIL                 1
#define CMS_ERROR               -1

enum TAG_MESSAGE {
    SUBCRIBE_MESSAGE = 0,
    UNSUBCRIBE_MESSAGE,
    SEND,
    SEND_TO,
    RESPONSE_MESSAGE
}


/**
 * \brief Cms message format
 * \param tag tag of message - classify messages
 * \param source source of message - identify owner of message
 * \param topic destination of message
 * \param data  describe in detail the content
 */
typedef struct cms_msg_t{
    int tag;
    char source[MAX_NAME_LENGTH];
    char topic[MAX_NAME_LENGTH];
    char data[MAX_NAME_LENGTH];
} cms_msg_t;

/**
 * \brief Initialize cms client
 * \param mq_attr attributes of message queue
 * \param name_client name of client
 * \return client message queue descriptor if success
*/
mqd_t init_client(struct mq_attr* mq_attr, char* name_client);

/**
 * \brief Close cms client
 * \param mqdes client message queue descriptor
 * \param name_client name of client
 * \return CMS_SUCCESS/CMS_ERROR
*/
void close_client(mqd_t mqdes, char* name_client);

/**
 * \brief Send cms message 
 * \param source identify of client
 * \param topic destination of group 
 * \param data data of message
 * \return CMS_SUCCESS/CMS_ERROR
*/
int send(char* source , char * topic, char * data);

/**
 * \brief Receive cms message
 * \param source identify of client
 * \param topic destination group/client
 * \param data cms received message
 * \return CMS_SUCCESS/CMS_ERROR
*/
int send_to(char * source, char * destination, char* data);

/**
 * \brief Receive cms message to server
 * \param mqdes client message queue descriptor
 * \param message cms received message
 * \return CMS_SUCCESS/CMS_ERROR
*/
int receive(mqd_t mqdes, cms_msg_t* message);

/**
 * \brief Set message queue attributes
 * \param flag flag
 * \param maxmsg maximum message in queue
 * \param msgsize maximum size of message
 * \return CMS_SUCCESS/CMS_ERROR
*/
struct mq_attr create_attr(long flag, long maxmsg, long msgsize);

/**
 * \brief Create cms message
 * \param tag tag of message
 * \param name_client name of client
 * \param data data of message
 * \return CMS_SUCCESS/CMS_ERROR
*/
cms_msg_t *create_msg(int tag, char* name_client, char* data);

/**
 * \brief subcribe one topic of server
 * \param source identify of client
 * \param topic destination of group
 * \return CMS_SUCCESS/CMS_ERROR
*/
int subcribe_topic(char * source, char * topic);

/**
 * \brief unsubcribe one topic of server
 * \param source identify of client
 * \param topic destination of group
 * \return CMS_SUCCESS/CMS_ERROR
*/
int unsubcribe_topic(char * source, char * topic);
#endif

