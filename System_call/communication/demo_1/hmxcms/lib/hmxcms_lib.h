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
#define CMS_UNSUBCRIBE_SUCCESS    "CMS_UNSUBCRIBE_SUCCESS"  
#define CMS_UNSUBCRIBE_FAIL       "CMS_UNSUBCRIBE_FAIL"  
#define CMS_CONFIG_SUCCESS        "CMS_CONFIG_SUCCESS"
#define CMS_CONFIG_FAIL           "CMS_CONFIG_FAIL"
#define RESPONE_MESSAGE           "RESPONE MESSAGE"

#define CMS_CTRL_HELP             "--help"
#define CMS_CTRL_SET_LOG          "--set-log-level"
#define CMS_CTRL_SET_QUEUE_ATTR   "--set-queue-attr"

#define MAX_NAME_LENGTH         64
#define MAX_LINE_LENGTH         100

#define CMS_SUCCESS              0
#define CMS_FAIL                 1
#define CMS_ERROR               -1


// #define HMX_CLIENT_1        "/HMX_CLIENT_1"
// #define HMX_CLIENT_2        "/HMX_CLIENT_2"
// #define HMX_CLIENT_3        "/HMX_CLIENT_3"
// #define HMX_CLIENT_4        "/HMX_CLIENT_4"
// #define HMX_CLIENT_5        "/HMX_CLIENT_5"

enum CLIENTS_NAME{
    HMX_CLIENT_1 = 1,
    HMX_CLIENT_2,
    HMX_CLIENT_3,
    HMX_CLIENT_4,
    HMX_CLIENT_5
};

enum TAG_MESSAGE {
    CMS_SUBCRIBE_MESSAGE = 0,
    CMS_UNSUBCRIBE_MESSAGE,
    CMS_REQUEST_SEND_MESSAGE,
    CMS_REQUEST_SEND_TO_MESSAGE,
    CMS_RESPONSE_MESSAGE,
    CMS_CONFIG_SERVER
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

/**
* \brief cms client infor
* \param mq_mq message queue of client
* \param client_name client name of client
*/
typedef struct 
{
    mqd_t my_mq;
    char client_name[MAX_NAME_LENGTH];
}cms_client_infor;

/**
 * \brief Initialize cms client 
 * \param max_message max message in queue message, Normally max_message will be less than or equal 10
 * \param client_name name of client
 * \return result of client init CMS_SUCCES/CMS_ERROR/ CMS_FAIL
*/
int cms_client_init(enum CLIENTS_NAME client_name);

/**
 * \brief Close cms client
 * \return void
*/
void cms_client_close();

/**
 * \brief Send cms message 
 * \param topic destination is group of clients interested in the topic 
 * \param data data of message
 * \return CMS_FAIL/CMS_SUCCESS/CMS_ERROR
*/
int cms_client_send(char * topic, char * data);

/**
 * \brief Receive cms message
 * \param destination_client destination of client
 * \param data cms received message
 * \return CMS_ERROR/CMS_SUCCESS/CMS_FAIL
*/
int cms_client_send_to(enum CLIENTS_NAME destination_client_name, char* data);

/**
 * \brief Receive cms message to server
 * \param message cms received message
 * \return length_message/CMS_ERROR
*/
int cms_client_receive(cms_msg_t* message);


/**
 * \brief subcribe one topic of server
 * \param topic destination of group
 * \return CMS_SUCCESS/CMS_ERROR/CMS_ERROR
*/
int cms_subcribe_topic(char * topic);

/**
 * \brief unsubcribe one topic of server
 * \param topic destination of group
 * \return CMS_SUCCESS/CMS_ERROR/CMS_ERROR
*/
int cms_unsubcribe_topic(char * topic);
#endif

