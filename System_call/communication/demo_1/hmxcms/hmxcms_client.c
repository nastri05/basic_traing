#include "hmxcms_client.h"

mqd_t mqserver;



int main ()
{
    cms_msg_t *response = (cms_msg_t*)malloc(sizeof( cms_msg_t));
    struct mq_attr* mq_attr = (struct mq_attr*)malloc(sizeof(mq_attr));
    // mqd_t mqclient = cms_client_init(mq_attr, "/Tuan", "mq1", SEND_REQUEST, "ALL");
    int mqd_client = cms_client_init(mq_attr,"name","/mqclient", SEND_REQUEST, "wan");
    if (mqd_client == CMS_ERROR) {
        printf("INIT FAIL %d \n",mqd_client);
    }
    
    LOG_CLIENT_STATE("BAT DAU RECEIVE\n");
    int i;
    for( i=0 ; i <20 ; i++){
    int result = cms_send_to_server(REQUEST_MESSAGE_GROUP, "name", "/mqclient", i, "wan", "sairoigg");
    //cms_receive(mqd_client, response);
    }
    //printf("result = %d\n",result);
    // mq_close(mqserver);
    free(response);
    free(mq_attr);
    return 0;
    // cms_msg_t *msg = (cms_msg_t *) malloc (sizeof (cms_msg_t));
    // attr.mq_flags = 0;
    // attr.mq_maxmsg = 10;
    // attr.mq_msgsize = 320;
    // mqclient = mq_open("/mqueueclient", O_CREAT | O_RDWR , 0777, &attr);
    // if (mqclient == CMS_ERROR) {
    //     printf("Init error \n");
    // }

    // mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    // if(mqserver == -1){
    //     printf("Can not open send queue. Open failed or server not existed");
    //     return mqserver;
    // }
    // int ret;
    // ret = cms_send(mqserver, CONFIG_MESSAGE, "name_client", "/mqueueclient", SEND_REQUEST, "all", ADD_CLIENT);
    // if (ret == CMS_ERROR) {
    //     printf("ERROR\n");
    // } else 
    //     printf ("1\n");
    // // create_msg(msg, CONFIG_MESSAGE, "name_client", "/mqueueclient", SEND_REQUEST, "all", ADD_CLIENT);
    // // if(mq_send(mqclient, (char *) msg, MSG_SIZE, 0)==-1) {
    // //     printf(" mq send error\n");
    // //     return CMS_ERROR;
    // // }
    // mq_close(mqserver);
    // // Listen on client message queue
    // while (1) {
    //     cms_receive(mqclient, &response);
    //     if (response.tag == RESPONSE_MESSAGE) 
    //         return (strcmp(response.payload.data, CMS_CONFIG_FAIL) == 0) ? CMS_ERROR : CMS_SUCCESS;
    // }
}
