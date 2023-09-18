#ifndef HMXCMS_MONITOR_H
#define HMXCMS_MONITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 64
#define MAX_DATA_LENGTH 64

#define CMS_SUCCESS     0
#define CMS_ERROR       -1

#define DEBUG_LIST
#ifdef DEBUG_LIST
#define STR(x)   #x
#define LOG(x) printf("[LIST] [%s]         %s = %s\n",__FUNCTION__,STR(x),x)
#define LOG_INT(x) printf("[LIST] [%s]         %s = %d\n",__FUNCTION__,STR(x),x)
#define LOG_STATE(x) printf("[LIST] [%s]   %s",__FUNCTION__, x)
#else
#define LOG(x)
#define LOG_INT(x)
#define LOG_STATE(x)
#endif

typedef struct cms_payload_t{
    char source[MAX_NAME_LENGTH];
    char topic[MAX_NAME_LENGTH];
    char data[MAX_DATA_LENGTH];
    char client_name[MAX_NAME_LENGTH];
    int send_count;
} cms_payload_t;

typedef struct cms_monitor_info_t{
    cms_payload_t *payload;
    struct cms_monitor_info_t *next;
} cms_monitor_info_t;




/**
* \brief Create payload_t with its data
* \param source is name of send client
* \param topic is topic of message
* \param data is data in the message that send client send to receive client
* \return payload_t
*/
cms_payload_t* create_payload(char* source, char* topic, char* data, char * client_name);
/**
* \brief Add client message to list when send to client is error to be monitored
* \param head_monitor_list is head of linked list that store all data
* \param payload is datagram send to client that send failed
* \return CMS_SUCCESS or CMS_ERROR
*/
int add_to_monitor(cms_monitor_info_t **head_monitor_list, cms_payload_t* payload);
/**
* \brief Delete client message from error list
* \param head_monitor_list is head of linked list that store all data
* \param destination is client that restore from error state to normal state will not be monitored
* \return CMS_SUCCESS or CMS_ERROR
*/
int delete_from_monitor(cms_monitor_info_t **head_monitor_list, char* client_name);
/**
* \brief Payload of a client store in monitor list
* \param head_monitor_list is head of linked list that store all data
* \param client_name is name of client that we want to get payload
* \return payload if client name existed in monitor list, else NULL
*/
cms_payload_t* get_payload_monitor_list(cms_monitor_info_t *head_monitor_list, char *client_name);
/**
* \brief Free monitor list that store send failed message
* \param head_monitor_list is head of linked list store send failed message.
*/
void free_monitor_list(cms_monitor_info_t **head_monitor_list);

void print_monitor_list(cms_monitor_info_t *head_monitor_list);


#endif