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
#include <string.h>

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

// Define Client Queue Name
#define RECEIVE_QUEUE_NAME      "/mqa"
#define RECEIVE2_QUEUE_NAME     "/mqb"

// Define Server Queue
#define SERVER_QUEUE_NAME       "/HmxCmsServer"
#define FLAG                    0
#define MSG_SIZE                sizeof(cms_msg_t)
#define MAX_MSG                 10


// Define response data
#define CMS_RECEIVE_SUCCESS       "CMS_RECEIVE_SUCCESS"
#define CMS_RECEIVE_FAIL          "CMS_RECEIVE_FAIL"
#define CMS_SUBCRIBE_SUCCESS      "CMS_SUBCRIBE_SUCCESS"
#define CMS_SUBCRIBE_FAIL         "CMS_SUBCRIBE_FAIL"
#define CMS_UNSUBCRIBE_SUCCESS    "CMS_SUBCRIBE_SUCCESS"  
#define CMS_UNSUBCRIBE_FAIL       "CMS_UNSUBCRIBE_FAIL"  



#define MAX_NAME_LENGTH          64
#define MAX_LINE_LENGTH          100

#define CMS_SUCCESS              0
#define CMS_FAIL                 1
#define CMS_ERROR               -1


enum TAG_MESSAGE {
    CMS_SUBCRIBE_MESSAGE = 0,
    CMS_UNSUBCRIBE_MESSAGE = 1,
    CMS_REQUEST_SEND_MESSAGE = 2,
    CMS_REQUEST_SEND_TO_MESSAGE = 3,
    CMS_RESPONSE_MESSAGE = 4
};


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

typedef struct cms_client_infor
{
    mqd_t my_mq;
    char client_name[MAX_NAME_LENGTH];
} cms_client_infor;

/**
 * \brief Initialize cms client 
 * \param mq_attr attributes of message queue
 * \param client_name name of client
 * \return cms_client_infor* client message queue descriptor if success
*/
cms_client_infor *cms_client_init(struct mq_attr* mq_attr, char* client_name);

/**
 * \brief Close cms client
 * \param mqdes client message queue descriptor
 * \param client_name name of client
 * \return void
*/
void cms_client_close(const cms_client_infor * my_client);

/**
 * \brief Send cms message 
 * \param source identify of client
 * \param topic destination is group of clients interested in the topic 
 * \param data data of message
 * \return CMS_FAIL/CMS_SUCCESS/CMS_ERROR
*/
int cms_client_send(const cms_client_infor * my_client, char * topic, char * data);

/**
 * \brief Receive cms message
 * \param source identify of client
 * \param destination destination group/client
 * \param data cms received message
 * \return CMS_ERROR/CMS_SUCCESS/CMS_FAIL
*/
int cms_client_send_to(const cms_client_infor * my_client, char * destination, char* data);

/**
 * \brief Receive cms message to server
 * \param mqdes client message queue descriptor
 * \param message cms received message
 * \return length_message/CMS_ERROR
*/
int cms_client_receive(const cms_client_infor * my_client, cms_msg_t* message);

/**
 * \brief Set message queue attributes
 * \param flag flag
 * \param maxmsg maximum message in queue
 * \param msgsize maximum size of message
 * \return struct mq_attr *
*/
struct mq_attr * cms_create_attr(long flag, long maxmsg, long msgsize);

/**
 * \brief Create cms message
 * \param tag tag of message
 * \param source name of client
 * \param topic name of group destination
 * \param data data of message
 * \return struct cms_msg_t 
*/
cms_msg_t cms_create_message(int tag,const char* source,char* topic, char* data);

/**
 * \brief subcribe one topic of server
 * \param source identify of client
 * \param topic destination of group
 * \return CMS_SUCCESS/CMS_ERROR/CMS_ERROR
*/
int cms_subcribe_topic(const cms_client_infor * my_client, char * topic);

/**
 * \brief unsubcribe one topic of server
 * \param source identify of client
 * \param topic destination of group
 * \return CMS_SUCCESS/CMS_ERROR/CMS_ERROR
*/
int cms_unsubcribe_topic(const cms_client_infor * my_client, char * topic);
#endif

