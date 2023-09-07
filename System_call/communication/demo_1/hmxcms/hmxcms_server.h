#ifndef __HMXCMS_SERVER_H
#define __HMXCMS_SERVER_H
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "lib/hmxcms_lib.h"
#include "lib/hmxcms_linkedlist.h"

#define MAX_THREADS 200
#define DEBUG_SERVER_LIST

#ifdef DEBUG_SERVER_LIST
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
 * \brief Initialize communication management server
 * \return server message queue descriptor if successful
 */
mqd_t cms_server_init();

/**
 * \brief Start communication management server
 * \return NULL
 */
void cms_server_start();

/**
 * \brief Stop communication management server
 * \return NULL
 */
void cms_server_stop();

/**
 * \brief Create thread to handle received message
 * \param msg received message from server queue
 * \return CMS_SUCCESS/CMS_ERROR
 */
int create_handling_pthread(cms_msg_t *msg);

#endif