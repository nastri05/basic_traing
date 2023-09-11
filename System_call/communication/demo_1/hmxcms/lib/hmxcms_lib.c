#include "hmxcms_lib.h"
#include <string.h>
#include<unistd.h>


// mqd_t mqserver;
mqd_t init_client(struct mq_attr* mq_attr,char *name_client)
{
    mqd_t mqdes;
    mq_unlink(name_client);
    mqdes = mq_open(name_client, O_RDONLY | O_CREAT | O_NONBLOCK , 0666, mq_attr);
    if(mqdes == -1){
        LOG_CLIENT_STATE("NOT INIT CLIENT\n");
    }
    return mqdes;
}

void close_client(mqd_t mqdes, char* name_client){
    mq_close(mqdes);
    mq_unlink(name_client);

}

int send_to(char * source, char * destination, char* data){
    cms_msg_t msg =create_msg(SEND_TO, source, destination, data);

    mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == CMS_ERROR){
        LOG_CLIENT_STATE("CAN NOT OPEN SERVER \n");
        return CMS_ERROR;
    }
    if(mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0)==-1) {
        LOG_CLIENT_STATE("MQ SEND ERROR\n");
        return CMS_ERROR;
    }
    LOG_CLIENT_STATE("SEND MESSAGE SUCCESS  \n");
    mq_close(mqserver);
    return CMS_SUCCESS;
}

int send(char * source, char * topic, char* data){
    cms_msg_t msg = create_msg(SEND, source, topic, data);
    mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == CMS_ERROR){
        LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
        return CMS_ERROR;
    }
    if(mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0)==-1) {
        LOG_CLIENT_STATE(" mq send error\n");
        return CMS_ERROR;
    }
    LOG_CLIENT_STATE("SEND MESSAGE TO SERVER SUCCESS ");
    mq_close(mqserver);
    return CMS_SUCCESS;
}

int subcribe_topic(char * source, char *topic){
    
    cms_msg_t msg = create_msg(SUBCRIBE_MESSAGE,source,topic,"SUBCRIBE TOPIC");
    mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == CMS_ERROR){
        LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
        return CMS_ERROR;
    }
    if(mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0) == -1) {
        LOG_CLIENT_STATE(" mq send error\n");
        return CMS_ERROR;
    }
    mq_close(mqserver);
    cms_msg_t respone;
    while(1){
    int result = receive(source,&respone);
    if(respone.tag == RESPONSE_MESSAGE){
        if(respone.data ==)
    }
    }
}

int receive(mqd_t mqdes, cms_msg_t* cms_msg){
    int ret;
    while(1){
    ret = mq_receive(mqdes, (char *) cms_msg, sizeof(cms_msg_t), NULL);
    if(ret > 0){
            LOG_CLIENT_STATE("---------------------------------\n");
            LOG_CLIENT_STATE("RECEIVE MESSAGE \n");
            LOG_CLIENT_STATE("---------------------------------\n");
            LOG_CLIENT_STATE("DATA MESSAGE : \n");
            LOG_CLIENT_INT(cms_msg->tag);
            LOG_CLIENT(cms_msg->source);
            LOG_CLIENT(cms_msg->topic);
            LOG_CLIENT(cms_msg->data);
        break;
    }
    }
    return ret;
};

struct mq_attr create_attr(long flag, long maxmsg, long msgsize){
    struct mq_attr attr;
    attr.mq_flags = flag;
    attr.mq_maxmsg = msgsize;
    attr.mq_msgsize = maxmsg;
    attr.mq_curmsgs = 0;
    return attr;
}

struct cms_msg_t create_msg(int tag, char* name_client,char* topic, char* data){
    struct cms_msg_t message;
    message.tag = tag;
    strcpy(message.source, name_client);
    strcpy(message.topic, topic);
    strcpy(message.data, data);
    return message;
}

