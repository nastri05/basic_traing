#include "hmxcms_lib.h"
#include <string.h>
#include<unistd.h>

int cms_send(mqd_t mqdes, int tag, char *name_client, char *mq_name, int type, char *topic, char *data){
    cms_msg_t msg ;
    // = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    create_msg(&msg, tag, name_client, mq_name, type, topic, data);

    if(mq_send(mqdes, (char *) &msg, sizeof(cms_msg_t), 0)==-1) {
        LOG_CLIENT_STATE(" MQ SEND ERROR\n");
        return CMS_ERROR;
    }
    LOG_CLIENT_STATE("SEND MESSAGE SUCCESS  \n");
    LOG_CLIENT_INT(type);
    return CMS_SUCCESS;
}

int cms_send_to_server(int tag, char *name_client, char *mq_name, int type, char *topic, char *data){
    cms_msg_t msg ;
    // = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    create_msg(&msg, tag, name_client, mq_name, type, topic, data);
    int mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == -1){
        LOG_CLIENT_STATE("Can not open send queue. Open failed or server not existed\n");
        return CMS_ERROR;
    }
    if(mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0)==-1) {
        LOG_CLIENT_STATE(" mq send error\n");
        return CMS_ERROR;
    }
    LOG_CLIENT_STATE("SEND MESSAGE TO SERVER SUCCESS ");
    printf("%d \n",type);
    mq_close(mqserver);
    return CMS_SUCCESS;
}

// mqd_t mqserver;
mqd_t cms_client_init(struct mq_attr* mq_attr,char *name_client, char *mq_name, int type, char *topic)
{
    mqd_t mqdes, mqserver;
    //mq_unlink(mq_name);
    cms_msg_t * response = (cms_msg_t * ) malloc(sizeof(cms_msg_t));
    // if(mq_attr==NULL){
    //     set_mq_attr(mq_attr, FLAG, 10, MSG_SIZE);
    //     printf("Error\n");
    // }
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(cms_msg_t);
    attr.mq_curmsgs = 0;
    mqdes = mq_open(mq_name, O_RDONLY | O_CREAT , 0666, &attr);
    if(mqdes == -1){
        LOG_CLIENT_STATE("Open receive queue error!\n");
        return CMS_ERROR;
    }
    mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == -1){
        LOG_CLIENT_STATE("Can not open send queue. Open failed or server not existed\n");
        return CMS_ERROR;
    }
    //printf("number of mq_servat %d",mqserver);
    cms_send(mqserver, CONFIG_MESSAGE, name_client, mq_name, type, topic, ADD_CLIENT);
    //printf("result = %d\n",result);
    mq_close(mqserver);
    // Listen on client message queue
    int read =-1;
    while(1){
        //sleep(1);
        read = cms_receive(mqdes, response);
        if(strcmp(response->payload.data,CMS_CONFIG_SUCCESS)==0){
            LOG_CLIENT(response->payload.data);
            break;
        }
        //printf("return read %d\n",read);
        
    } 
    //mq_close(mqdes);
    free(response);
    return (strcmp(response->payload.data, CMS_CONFIG_FAIL) == 0) ? CMS_ERROR : mqdes;
}

int cms_client_close(mqd_t mqdes, char* name_client, char* mq_name){
    cms_msg_t response;
    mqd_t mqserver;
    mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    cms_send(mqserver, CONFIG_MESSAGE, name_client, mq_name, 0, NULL, REMOVE_CLIENT);
    // Listen on client message queue
    while(1){
        cms_receive(mqdes, &response);        
        if (response.tag == RESPONSE_MESSAGE) {
            if (strcmp(response.payload.data, CMS_CONFIG_SUCCESS) == 0) {
                mq_close(mqserver);
                mq_close(mqdes);
                mq_unlink(mq_name);
                return CMS_SUCCESS;
            } else if (strcmp(response.payload.data, CMS_CONFIG_FAIL) == 0) {
                mq_close(mqserver);
                mq_close(mqdes);
                return CMS_ERROR;
            }
        }
    }
}


int cms_receive(mqd_t mqdes, cms_msg_t* cms_msg){
    int ret;
    while(1){
    sleep(1);
    ret = mq_receive(mqdes, (char *) cms_msg, sizeof(cms_msg_t), NULL);
    if(ret > 0){
            LOG_CLIENT_STATE("---------------------------------\n");
            LOG_CLIENT_STATE("RECEIVE MESSAGE \n");
            LOG_CLIENT_STATE("---------------------------------\n");
            LOG_CLIENT_STATE("DATA MESSAGE : \n");
            LOG_CLIENT_INT(cms_msg->tag);
            LOG_CLIENT(cms_msg->payload.client_name);
            LOG_CLIENT(cms_msg->payload.mq_name);
            LOG_CLIENT(cms_msg->payload.topic);
            LOG_CLIENT(cms_msg->payload.data);
            LOG_CLIENT_INT(cms_msg->payload.type);
        break;
    }
    }
    return ret;
};

int set_mq_attr(struct mq_attr *attr, long flag, long maxmsg, long msgsize){
    attr->mq_flags = 0;
    attr->mq_maxmsg = 10;
    attr->mq_msgsize = sizeof(cms_msg_t);
    attr->mq_curmsgs = 0;
    return CMS_SUCCESS;
}

int create_msg(cms_msg_t* msg, int tag, char* name_client, char* mq_name, int type, char* topic, char* data){
    msg->tag = tag;
    strcpy(msg->payload.client_name, name_client);
    strcpy(msg->payload.mq_name, mq_name);
    msg->payload.type = type;
    strcpy(msg->payload.topic, topic);
    strcpy(msg->payload.data, data);
    msg->length = sizeof(msg->payload);
    return CMS_SUCCESS;
}

