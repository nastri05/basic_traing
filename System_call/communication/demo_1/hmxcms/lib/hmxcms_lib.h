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
#define SERVER_QUEUE_NAME       "/HmxCmsServer123"
#define FLAG                    0
#define MSG_SIZE                sizeof(cms_msg_t)
#define MAX_MSG                 10

// Define type of message
#define REQUEST_MESSAGE_ALL      0
#define REQUEST_MESSAGE_CLIENT   1
#define REQUEST_MESSAGE_GROUP    2
#define RESPONSE_MESSAGE         3
#define CONFIG_MESSAGE           4
#define SEND_REQUEST             5
#define RECEIVE_REQUEST          6
// Define response data
#define CMS_CONFIG_SUCCESS       "config_success"
#define CMS_CONFIG_FAIL          "config_fail"
#define CMS_SEND_SUCCESS         "send_success"
#define CMS_SEND_FAIL            "send_fail"
// Define config data
#define ADD_CLIENT               "add"
#define REMOVE_CLIENT            "remove"
#define CHANGE_CLIENT            "change"

#define LOST_WAN                 1
#define CHANGE_WAN               2

#define CMS_SUCCESS              0
#define CMS_FAIL                 1
#define CMS_ERROR               -1

/**
 * \brief Payload of cms message
 * \param client_name name of client
 * \param mq_name name of client message queue
 * \param type type of message
 * \param topic name of destination group/client 
 * \param data data of message
 */
typedef struct {
    char client_name[64];
    char mq_name[64];
    int type;
    char topic[64];
    char data[64];
} cms_payload_t;

/**
 * \brief Cms message format
 * \param tag tag of message - classify messages
 * \param length length of payload
 * \param cms_payload_t payload of message 
 */
typedef struct {
    int tag;
    int length;
    cms_payload_t payload;
} cms_msg_t;

/**
 * \brief Initialize cms client
 * \param mq_attr attributes of message queue
 * \param name_client name of client
 * \param mq_name name of client message queue
 * \param type communication type of client
 * \param topic destination group/client
 * \return client message queue descriptor if success
*/
mqd_t cms_client_init(struct mq_attr* mq_attr, char* name_client, char* mq_name, int type, char *topic);

/**
 * \brief Close cms client
 * \param mqdes client message queue descriptor
 * \param name_client name of client
 * \param mq_name name of client message queue
 * \return CMS_SUCCESS/CMS_ERROR
*/
int cms_client_close(mqd_t mqdes, char* name_client, char* mq_name);

/**
 * \brief Send cms message 
 * \param mqdes client message queue descriptor
 * \param tag tag of message
 * \param name_client name of client
 * \param mq_name name of client message queue
 * \param type communication type of client
 * \param topic destination group/client
 * \param data data of message
 * \return CMS_SUCCESS/CMS_ERROR
*/
int cms_send(mqd_t mqdes, int tag, char* name_client, char* mq_name, int type, char* topic, char* data);

/**
 * \brief Receive cms message
 * \param mqdes client message queue descriptor
 * \param cms_msg cms received message
 * \return CMS_SUCCESS/CMS_ERROR
*/
int cms_send_to_server(int tag, char *name_client, char *mq_name, int type, char *topic, char *data);

/**
 * \brief Receive cms message to server
 * \param cms_msg cms received message
 * \return CMS_SUCCESS/CMS_ERROR
*/
int cms_receive(mqd_t mqdes, cms_msg_t* cms_msg);

/**
 * \brief Set message queue attributes
 * \param attr message queue attributes struct
 * \param flag flag
 * \param maxmsg maximum message in queue
 * \param msgsize maximum size of message
 * \return CMS_SUCCESS/CMS_ERROR
*/
int set_mq_attr(struct mq_attr* attr, long flag, long maxmsg, long msgsize);

/**
 * \brief Create cms message
 * \param msg created cms message
 * \param tag tag of message
 * \param name_client name of client
 * \param mq_name name of client message queue
 * \param type communication type of client
 * \param topic destination group/client
 * \param data data of message
 * \return CMS_SUCCESS/CMS_ERROR
*/
int create_msg(cms_msg_t* msg, int tag, char* name_client, char* mq_name, int type, char* topic, char* data);

#endif

