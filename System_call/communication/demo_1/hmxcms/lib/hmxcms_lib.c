#include "hmxcms_lib.h"

static cms_client_infor * global_client_infor = NULL;

static struct cms_msg_t cms_create_message(int tag,const char* source,char* topic, char* data){
    struct cms_msg_t message;
    message.tag = tag;
    strcpy(message.source, source);
    strcpy(message.topic, topic);
    strcpy(message.data, data);
    return message;
}
static struct mq_attr cms_create_attr(long flag, long maxmsg, long msgsize){
    struct mq_attr mq_attr;
    mq_attr.mq_flags = flag;
    mq_attr.mq_maxmsg = maxmsg;
    mq_attr.mq_msgsize = msgsize;
    mq_attr.mq_curmsgs = 0;
    return mq_attr;
}

int cms_client_init(enum CLIENTS_NAME client_name)
{
    if(global_client_infor!=NULL){
        free(global_client_infor);
        global_client_infor == NULL;
    }
    struct mq_attr  mq_attr = cms_create_attr(0,10,sizeof(struct cms_msg_t));
    mqd_t mqdes;
    char mqueue_client_name[64];
    memset(mqueue_client_name,'\0',sizeof(mqueue_client_name));
    sprintf(mqueue_client_name,"/HMX_CLIENT_%d",client_name);
    mq_unlink(mqueue_client_name);
    mqdes = mq_open(mqueue_client_name, O_RDONLY | O_CREAT | O_NONBLOCK , 0666, &mq_attr);
    LOG_CLIENT_INT(mqdes);
    if(mqdes == -1){
        LOG_CLIENT_STATE("CAN NOT INIT CLIENT\n");
        return CMS_ERROR;
    }
    LOG_CLIENT_STATE("INIT CLIENT\n");
    global_client_infor = (cms_client_infor*) malloc(sizeof(cms_client_infor));
    strcpy(global_client_infor->client_name,mqueue_client_name);
    global_client_infor->my_mq = mqdes;
    if(global_client_infor==NULL){
        return CMS_FAIL;
    }
    return CMS_SUCCESS;
}

void cms_client_close(){
    if(global_client_infor != NULL){
        mq_close(global_client_infor->my_mq);
        mq_unlink(global_client_infor->client_name);
        free(global_client_infor);
    }
}

int cms_client_send_to(enum CLIENTS_NAME destination_client_name, char* data){
    if(global_client_infor != NULL){
        char mqueue_client_name[64];
        memset(mqueue_client_name,'\0',sizeof(mqueue_client_name));
        sprintf(mqueue_client_name,"/HMX_CLIENT_%d",destination_client_name);
        struct cms_msg_t msg = cms_create_message(CMS_REQUEST_SEND_TO_MESSAGE,
                                                  global_client_infor->client_name,
                                                  mqueue_client_name,
                                                  data);

        mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
        if(mqserver == CMS_ERROR){
            LOG_CLIENT_STATE("CAN NOT OPEN SERVER \n");
            mq_close(mqserver);
            return CMS_ERROR;
        }
        int result = mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0);
        if(result==-1) {
            LOG_CLIENT_STATE("MQ SEND ERROR\n");
            mq_close(mqserver);
            return CMS_FAIL;
        }
        LOG_CLIENT_STATE("SEND MESSAGE SUCCESS  \n");
        mq_close(mqserver);
        return CMS_SUCCESS;
    }
    LOG_CLIENT_STATE("global client infor is NULL,  please check init client\n");
    return CMS_ERROR;
}

int cms_client_send(char * topic, char * data){
    if(global_client_infor != NULL){
        cms_msg_t msg = cms_create_message(CMS_REQUEST_SEND_MESSAGE,
                                           global_client_infor->client_name,
                                           topic,
                                           data);
        mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
        if(mqserver == CMS_ERROR){
            LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
            return CMS_ERROR;
        }
        int result =  mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0);
        
        if(result==CMS_ERROR) {
            LOG_CLIENT_STATE("CAN'T OPEN SERVER\n");
            mq_close(mqserver);
            return CMS_FAIL;
        }
        LOG_CLIENT_STATE("SEND MESSAGE TO SERVER SUCCESS \n");
        mq_close(mqserver);
        return CMS_SUCCESS;
    }
    LOG_CLIENT_STATE("global client infor is NULL,  please check init client\n");
    return CMS_ERROR;
}

int cms_subcribe_topic(char * topic){
    if(global_client_infor != NULL){
        cms_msg_t msg = cms_create_message(CMS_SUBCRIBE_MESSAGE,
                                           global_client_infor->client_name,
                                           topic,
                                           "SUBCRIBE TOPIC");
        mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
        if(mqserver == CMS_ERROR){
            LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
            return CMS_ERROR;
        }
        int result =  mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0);
        if(result == CMS_ERROR) {
            LOG_CLIENT_STATE(" mq send error\n");
            return CMS_ERROR;
        }
        mq_close(mqserver);
        cms_msg_t response;
        while(1){
        int result = cms_client_receive(&response);
        if(result == CMS_ERROR){
            printf("Lỗi\n");
            return CMS_ERROR;
        }
        if(response.tag == CMS_RESPONSE_MESSAGE){
            if(strcmp(response.data,CMS_SUBCRIBE_SUCCESS)==0){
                printf("SUBCRIBE SUCCESS\n");
                return CMS_SUCCESS;
            }else{
                printf("SUBCRIBE FAIL\n");
                return CMS_FAIL;
                }
            }
        }
    }
    LOG_CLIENT_STATE("global client infor is NULL, please check init client\n");
    return CMS_ERROR;
}

int cms_unsubcribe_topic(char * topic){
    if(global_client_infor != NULL){
        cms_msg_t msg = cms_create_message(CMS_UNSUBCRIBE_MESSAGE,
                                           global_client_infor->client_name,
                                           topic,
                                           "UNSUBCRIBE TOPIC");
        mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY );
        if(mqserver == CMS_ERROR){
            LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
            return CMS_ERROR;
        }
        int result_send = mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0);
        usleep(10);
        if(result_send == CMS_ERROR) {
            LOG_CLIENT_STATE(" mq send error\n");
            return CMS_ERROR;
        }
        mq_close(mqserver);
        cms_msg_t respone;
        while(1){
        int result = cms_client_receive(&respone);
        if(respone.tag == CMS_RESPONSE_MESSAGE){
            if(strcmp(respone.data,CMS_UNSUBCRIBE_SUCCESS)==0){
                LOG_CLIENT_STATE("UNUBCRIBE SUCCESS\n");
                return CMS_SUCCESS;
            }else{
                LOG_CLIENT_STATE("UNSUBCRIBE SUCCESS\n");
                return CMS_FAIL;
                }
            }
        }
    }
    LOG_CLIENT_STATE("global client infor is NULL, please check init client\n");
    return CMS_ERROR;
}


int cms_client_receive(cms_msg_t* message){
    if(global_client_infor != NULL){
        int ret;
        char result_send[MAX_NAME_LENGTH];
        while(1){
        ret = mq_receive(global_client_infor->my_mq, (char *) message, sizeof(cms_msg_t), NULL);
        if(ret != CMS_ERROR)
        {
            LOG_CLIENT_STATE("---------------------------------\n");
            LOG_CLIENT(message->source);
            LOG_CLIENT(message->topic);
            LOG_CLIENT(message->data);
            strcpy(result_send,CMS_RECEIVE_SUCCESS);
            break;
        }
        else{
                if(errno  != EAGAIN &&
                    errno  != EWOULDBLOCK && errno  != EINTR)
                    {
                        LOG_CLIENT_INT(errno);
                        strcpy(result_send,CMS_RECEIVE_FAIL);
                        break;
                    }
            }
        
        usleep(10);
        }

        if(strcmp(result_send,CMS_RECEIVE_FAIL)== 0){
        cms_msg_t msg = cms_create_message(CMS_RESPONSE_MESSAGE,
                                           global_client_infor->client_name,
                                           RESPONE_MESSAGE,
                                           result_send);
        
        mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
        
        if(mqserver == CMS_ERROR){
            LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
            return CMS_ERROR;
            }
        int result = mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0);
        if( result == CMS_ERROR) {
            LOG_CLIENT_STATE(" mq send error\n");
            mq_close(mqserver);
            return CMS_FAIL;
            }
        mq_close(mqserver);
        }
        
        return ret;
    }
    LOG_CLIENT_STATE("global client infor is NULL, please check init client\n");
    return CMS_ERROR;
};



