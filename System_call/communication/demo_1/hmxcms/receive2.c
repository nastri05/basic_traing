#include "hmxcms_client_receive.h"

mqd_t mqclient;

cms_msg_t response;

int main ()
{
    struct mq_attr* mq_attr = (struct mq_attr*)malloc(sizeof(mq_attr));
    // mqd_t mqclient = cms_client_init(mq_attr, "/Tuan", "mq1", SEND_REQUEST, "ALL");
    int mqd_client = cms_client_init(mq_attr,"name4","/mqclient3", RECEIVE_REQUEST, "wan");
    if (mqd_client == CMS_ERROR) {
        printf("INIT FAIL %d \n",mqd_client);
    }
    mqclient = mq_open("/mqclient3", O_RDONLY);
    while(1)
    {
        int read = cms_receive(mqclient, &response);
        if (read < 0) {
            printf("Can not receive message from client queue \n");
        } else {
            //printf("1\n");
            printf(" gia tri %d %s %d %s\n", response.tag, response.payload.client_name, response.payload.type, response.payload.data);
        }
        sleep(2);
    }
    mq_close(mqclient);
    return 0;
}