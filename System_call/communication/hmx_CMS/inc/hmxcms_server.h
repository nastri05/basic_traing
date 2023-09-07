#ifndef __HMXCMS_SERVER_H
#define __HMXCMS_SERVER_H
#include <mqueue.h>
#include "hmxcms_linkedlist.h"

mqd_t cms_server_init();
void cms_server_start();
void cms_server_stop();

int create_handling_pthread(cms_msg_t *msg);

#endif