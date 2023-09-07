#include "hmxcms_client_receive.h"

mqd_t mqclient;



int main ()
{
    struct mq_attr* mq_attr = (struct mq_attr*)malloc(sizeof(mq_attr));
    cms_msg_t *response = (cms_msg_t*)malloc(sizeof( cms_msg_t));
    // mqd_t mqclient = cms_client_init(mq_attr, "/Tuan", "mq1", SEND_REQUEST, "ALL");
    int mqd_client = cms_client_init(mq_attr,"name3","/mqclient3", RECEIVE_REQUEST, "wan");
    if (mqd_client == CMS_ERROR) {
        printf("INIT FAIL %d \n",mqd_client);
    }
    //mqclient = mq_open("/mqclient1", O_RDONLY);
    LOG_CLIENT_STATE("BAT DAU RECEIVE\n");
    while(1)
    {
        cms_receive(mqd_client,response);
    }
    free(response);
    free(mq_attr);
    mq_close(mqclient);
    return 0;
}