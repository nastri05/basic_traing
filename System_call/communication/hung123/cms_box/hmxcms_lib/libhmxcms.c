#include "libhmxcms.h"



// mqd_t mqserver;
cms_client_infor *cms_client_init(struct mq_attr *mq_attr,char *client_name)
{
    mqd_t mqdes;
    mq_unlink(client_name);
    mqdes = mq_open(client_name, O_RDONLY | O_CREAT | O_NONBLOCK , 0666, mq_attr);
    if(mqdes == -1) {
        LOG_CLIENT_STATE("CAN NOT INIT CLIENT\n");
        return NULL;
    }
    LOG_CLIENT_STATE("INIT CLIENT\n");
    cms_client_infor *client_infor = (cms_client_infor*)malloc(sizeof(cms_client_infor));
    strcpy(client_infor->client_name, client_name);
    client_infor->my_mq = mqdes;
    return client_infor;
}

void cms_client_close(const cms_client_infor *my_client) {
    mq_close(my_client->my_mq);
    mq_unlink(my_client->client_name);
}

int cms_client_send_to(const cms_client_infor *my_client, char *destination, char *data) {
    struct cms_msg_t msg = cms_create_message(CMS_REQUEST_SEND_TO_MESSAGE, my_client->client_name, destination, data);
    mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == CMS_ERROR) {
        LOG_CLIENT_STATE("CAN NOT OPEN SERVER \n");
        return CMS_ERROR;
    }
    int retvl = mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0);
    if(retvl == -1) {
        LOG_CLIENT_STATE("MQ SEND ERROR\n");
        return CMS_FAIL;
    }
    LOG_CLIENT_STATE("SEND MESSAGE SUCCESS\n");
    mq_close(mqserver);
    return CMS_SUCCESS;
}

int cms_client_send(const cms_client_infor *my_client, char *topic, char *data) {
    struct cms_msg_t msg = cms_create_message(CMS_REQUEST_SEND_MESSAGE, my_client->client_name, topic, data);
    mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == CMS_ERROR) {
        LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
        return CMS_ERROR;
    }
    int retvl = mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0);
    if(retvl == -1) {
        LOG_CLIENT_STATE("CAN'T OPEN SERVER\n");
        return CMS_FAIL;
    }
    LOG_CLIENT_STATE("SEND MESSAGE TO SERVER SUCCESS\n");
    mq_close(mqserver);
    return CMS_SUCCESS;
}

int cms_subcribe_topic(const cms_client_infor * my_client, char *topic) {
    
    cms_msg_t msg = cms_create_message(CMS_SUBCRIBE_MESSAGE, my_client->client_name, topic, "SUBCRIBE TOPIC");
    mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == CMS_ERROR){
        LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
        return CMS_ERROR;
    }
    int retvl = mq_send(mqserver, (char *)&msg, sizeof(cms_msg_t), 0);
    if(retvl == -1) {
        LOG_CLIENT_STATE(" mq send error\n");
        return CMS_ERROR;
    }
    mq_close(mqserver);
    cms_msg_t response;
    while(1){
        int result = cms_client_receive(my_client,&response);
        if (result == CMS_ERROR) {
            return CMS_ERROR;
        }
        if(response.tag == CMS_RESPONSE_MESSAGE) {
            if(strcmp(response.data, CMS_SUBCRIBE_SUCCESS) == 0) {
                LOG_CLIENT_STATE("SUBCRIBE SUCCESS\n");
                return CMS_SUCCESS;
            } else {
                LOG_CLIENT_STATE("SUBCRIBE FAIL\n");
                return CMS_FAIL;
            }
        }
    }
}

int cms_unsubcribe_topic(const cms_client_infor *my_client, char *topic) {
    
    cms_msg_t msg = cms_create_message(CMS_UNSUBCRIBE_MESSAGE, my_client->client_name, topic, "UNSUBCRIBE TOPIC");
    mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == CMS_ERROR) {
        LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
        return CMS_ERROR;
    }
    int retvl = mq_send(mqserver, (char *)&msg, sizeof(cms_msg_t), 0);
    if(retvl == -1) {
        LOG_CLIENT_STATE(" mq send error\n");
        return CMS_ERROR;
    }
    mq_close(mqserver);
    cms_msg_t response;
    while(1) {
        int result = cms_client_receive(my_client, &response);
        if(response.tag == CMS_RESPONSE_MESSAGE){
            if(strcmp(response.data, CMS_UNSUBCRIBE_SUCCESS) == 0) {
                LOG_CLIENT_STATE("UNUBCRIBE SUCCESS\n");
                return CMS_SUCCESS;
            } else {
                LOG_CLIENT_STATE("UNSUBCRIBE FAIL\n");
                return CMS_FAIL;
            }
        }
    }
}


int cms_client_receive(const cms_client_infor *my_client, cms_msg_t *message) {
    int ret, i=0;
    int count_err = 0;
    int empty_flag = 0;
    char result[MAX_NAME_LENGTH];

    int rcv = mq_receive(my_client->my_mq, (char *)message, sizeof(cms_msg_t), NULL);
    if (rcv == CMS_ERROR) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            //Client queue empty
            empty_flag = 1;
        } else {
            //Other error
            LOG_CLIENT_STATE("Error receiving message from client queue\n");
            //printf("errno: %d\n", errno);
            count_err ++;
            strcpy(result, CMS_RECEIVE_FAIL);
            if (count_err > 3) {
                LOG_CLIENT_STATE("While reading, too many errors happened in the same time. Stop client \n");
                return CMS_ERROR;
            }
        }
    } else {
        count_err = 0;
        i++;
        //LOG_CLIENT_INT(rcv);
        //LOG_CLIENT_INT(errno);
        LOG_CLIENT(message->data);
        strcpy(result, CMS_RECEIVE_SUCCESS);
    }
    // if (empty_flag == 0) {
    //     LOG_CLIENT(result);
    //     cms_msg_t msg = create_message(CMS_RESPONSE_MESSAGE,my_client->client_name,"",result);
    //     mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    //     if(mqserver == CMS_ERROR){
    //         LOG_CLIENT_STATE("CAN NOT OPEN SERVER\n");
    //         return CMS_ERROR;
    //         }
    //     if(mq_send(mqserver, (char *) &msg, sizeof(cms_msg_t), 0) == -1) {
    //         LOG_CLIENT_STATE(" mq send error\n");
    //         return CMS_ERROR;
    //         }
    //     mq_close(mqserver);
    // }
    return rcv;
};

struct mq_attr * cms_create_attr(long flag, long maxmsg, long msgsize) {
    struct mq_attr *mq_attr = (struct mq_attr *)malloc(sizeof(struct mq_attr));
    mq_attr->mq_flags = flag;
    mq_attr->mq_maxmsg = maxmsg;
    mq_attr->mq_msgsize = msgsize;
    mq_attr->mq_curmsgs = 0;
    return mq_attr;
}

struct cms_msg_t cms_create_message(int tag,const char* source,char* topic, char* data) {
    struct cms_msg_t message;
    message.tag = tag;
    strcpy(message.source, source);
    strcpy(message.topic, topic);
    strcpy(message.data, data);
    return message;
}
