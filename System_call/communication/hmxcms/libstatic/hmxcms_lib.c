#include "hmxcms_lib.h"
#include <string.h>
#include<unistd.h>

int cms_send(mqd_t mqdes, int tag, char *name_client, char *mq_name, int type, char *topic, char *data){
    cms_msg_t msg ;
    // = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    create_msg(&msg, tag, name_client, mq_name, type, topic, data);

    if(mq_send(mqdes, (char *) &msg, sizeof(cms_msg_t), 0)==-1) {
        printf(" mq send error\n");
        return CMS_ERROR;
    }
    return CMS_SUCCESS;
}

// mqd_t mqserver;
mqd_t cms_client_init(struct mq_attr* mq_attr,char *name_client, char *mq_name, int type, char *topic)
{
    mqd_t mqdes, mqserver;
    mq_unlink(name_client);
    cms_msg_t * response = (cms_msg_t * ) malloc(sizeof(cms_msg_t));
    int size_client_name = strlen(name_client);
    printf("Flag is mq_attr  : %ld \n",mq_attr->mq_flags);
    printf("mq_flags is mq_attr  : %ld \n",mq_attr->mq_flags);
    printf("mq_maxmsg is mq_attr  : %ld \n",mq_attr->mq_maxmsg);
    printf("mq_msgsize is mq_attr  : %ld \n",mq_attr->mq_msgsize);
    if(mq_attr==NULL){
        set_mq_attr(mq_attr, FLAG, MAX_MSG, MSG_SIZE);
        printf("Flag is mq_attr  : %ld \n",mq_attr->mq_flags);
        printf("mq_flags is mq_attr  : %ld \n",mq_attr->mq_flags);
        printf("mq_maxmsg is mq_attr  : %ld \n",mq_attr->mq_maxmsg);
        printf("mq_msgsize is mq_attr  : %ld \n",mq_attr->mq_msgsize);
    }
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(cms_msg_t);
    attr.mq_curmsgs = 0;
    mqdes = mq_open(name_client, O_RDONLY | O_CREAT | O_NONBLOCK  , 0666, &attr);
    if(mqdes == -1){
        printf("Open receive queue error!\n");
        return mqdes;
    }
    mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == -1){
        printf("Can not open send queue. Open failed or server not existed\n");
        return mqserver;
    }
    printf("number of mq_servat %d",mqserver);
    int result = cms_send(mqserver, CONFIG_MESSAGE, name_client, mq_name, type, topic, ADD_CLIENT);
    printf("result = %d\n",result);
    mq_close(mqserver);
    // Listen on client message queue
    int read =-1;
    do{
    sleep(1);
    read = cms_receive(mqdes, response);
    printf("return read %d\n",read);
    
    if(read > 0){
        printf("gia tri la %s\n",response->payload.client_name);
        
    }
    }while(read <0);
    free(response);
    free(mq_attr);
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
    // if(cms_msg!=NULL){
    //     cms_msg = NULL;
    // }
    //printf("123\n");
    int ret = mq_receive(mqdes, (char *) cms_msg, sizeof(cms_msg_t), NULL);
    
    return (ret == -1) ? CMS_ERROR : ret;
};

int set_mq_attr(struct mq_attr *attr, long flag, long maxmsg, long msgsize){
    attr = (struct mq_attr*)malloc(sizeof(struct mq_attr));
    attr->mq_flags = flag;
    attr->mq_maxmsg = maxmsg;
    attr->mq_msgsize = msgsize;
    attr->mq_curmsgs = 0;
    return CMS_SUCCESS;
}

int create_msg(cms_msg_t* msg, int tag, char* name_client, char* mq_name, int type, char* topic, char* data){
    msg->tag = tag;
    msg->length = PAYLOAD_LENGTH;
    cms_payload_t payload;
    strcpy(msg->payload.client_name, name_client);
    strcpy(msg->payload.mq_name, mq_name);
    msg->payload.type = type;
    strcpy(msg->payload.topic, topic);
    strcpy(msg->payload.data, data);
    return CMS_SUCCESS;
}

