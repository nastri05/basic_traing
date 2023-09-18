#ifndef __HMXCMS_SERVER_H
#define __HMXCMS_SERVER_H
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include "hmxcms_linkedlist.h"
#include "libhmxcms.h"
#include "hmxcms_monitor.h"


#define DEBUG_SERVER
#ifdef DEBUG_SERVER
#define STR(x)   #x
#define LOG_SERVER(x) printf("[SERVER] [%s]         %s = %s\n",__FUNCTION__,STR(x),x)
#define LOG_SERVER_INT(x) printf("[SERVER] [%s]         %s = %d\n",__FUNCTION__,STR(x),x)
#define LOG_SERVER_STATE(x) printf("[SERVER] [%s]   %s",__FUNCTION__, x)
#else
#define LOG_SERVER(x)
#define LOG_SERVER_INT(x)
#define LOG_SERVER_STATE(x)
#endif


/**
 * \brief Handle signal SIGINT
 * \return nothing
 */
void cms_sig_handler();

/**
 * \brief Function used to send from server to client
 * \param msg message to handle
 * \param mqdes message queue descriptor
 * \return CMS_SUCCESS/CMS_ERROR
 */
int cms_server_send(cms_msg_t *msg, mqd_t mqdes, char *client_name);

/**
 * \brief Handle full queue
 * \param msg message to handle
 * \param mqdes message queue descriptor
 * \return CMS_SUCCESS/CMS_ERROR
 */
int cms_full_queue_handler(cms_msg_t *msg, mqd_t mqdes, char *client_name);

/**
 * \brief Initialize communication management server
 * \return server message queue descriptor if successful
 */
mqd_t cms_server_init();

/**
 * \brief Start communication management server
 * \return nothing
 */
void cms_server_start();

/**
 * \brief Stop communication management server
 * \return nothing
 */
void cms_server_stop();

#endif