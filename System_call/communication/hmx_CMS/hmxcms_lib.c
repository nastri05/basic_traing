#include "hmxcms_lib.h"


int set_mq_attr(mq_attr* attr, long flag, long maxmsg, long msgsize){
    attr->flag = flag;
    attr->maxmsg = maxmsg;
    attr->msgsize = msgsize;
    return CMS_SUCCESS;
}


int cms_send(mqd_t mqdes, int tag, char* name_client, char* mq_name, int type, char* topic, char* data){
    cms_msg_t msg;
    create_msg(&msg, &name_client, &mq_name, 0, &topic, &data);
    
    if(mq_send(mqdes, &msg, MSG_SIZE, 0)==-1){
        printf(" mq send error\n");
        return CMS_ERROR;
    }
    return CMS_SUCCESS;
}

// mqd_t mqserver;
mqd_t cms_client_init(char* name_client, char* mq_name, mq_attr_t* attr, char* topic, mqd_t* mqserver)
{
    mqd_t mqdes;
    cms_msg_t response;
    int size_client_name = strlen(name_client);
    int size_mq_name = strlen(mq_name);
    char data[]="add";
    if(attr==NULL){
        set_mq_attr(&attr, FLAG, MAX_MSG, MSG_SIZE);
    }
    mqdes = mq_open(&name_client, O_RDONLY | O_CREAT , 0777, &attr);
    if(mq_des == -1){
        printf("Open receive queue error!");
        return mq_des;
    }
    mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY, 0777, &attr);
    if(mqserver == -1){
        printf("Can not open send queue. Open failed or server not existed");
        return mqserver;
    }
    //if error why not send again
    cms_send(mqserver, CONFIG_MESSAGE, &name_client, &mq_name, 0, &topic, &data)
    mq_close(mqserver);
    // Listen on client message queue
    // 
    cms_receive(mqdes, &response);

    if (strcmp(response.payload.data, CMS_CONFIG_FAIL))
        return CMS_ERROR;
    else     
        return mqdes;
}

int cms_client_close(char* name_client, char* mq_name, mqd_t mqdes){
    cms_msg_t response;
    mqd_t mqserver;
    mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY); 
    cms_send(mqserver, CONFIG_MESSAGE, &name_client, &mq_name, 0, &topic, REMOVE_CLIENT);
    // Listen on client message queue
    while(1){
        int cms_receive(mqdes, &response);        
        if (response.tag == RESPONSE_MESSAGE) {
            if (strcmp(response.payload.data, CMS_CONFIG_SUCCESS) == 0) {
                mq_unlink(&name_client);
                mq_close(mqserver);
                mq_close(mqdes);
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
    if(cms_msg!=NULL){
        cms_msg_t * tmp = cms_msg;
        cms_msg = NULL;
        free(tmp);   
    }
    int msg_size = sizeof(struct cms_msg_t);
    return mq_receive(mqdes, &cms_msg, msg_size, 0);
};


int create_msg(cms_msg_t* msg, int tag, char* name_client, char* mq_name, int type, char* topic, char* data){
    msg->tag = tag;
    msg->length = sizeof(struct payload);
    cms_payload_t payload;
    payload.client_name = name_client;
    payload.mq_name = mq_name;
    payload.type = type;
    payload.topic = topic;
    payload.data = data;
    msg->payload = payload;
    return CMS_SUCCESS;
}
