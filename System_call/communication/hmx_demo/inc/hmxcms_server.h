#ifndef __HMXCMS_SERVER_H
#define __HMXCMS_SERVER_H
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include "hmxcms_linkedlist.h"
#include "hmxcms_lib.h"

#define CHANGE_CLIENT "change"

mqd_t cms_server_init();
void cms_server_start();
void cms_server_stop();

int create_handling_pthread(cms_msg_t *msg);

#endif