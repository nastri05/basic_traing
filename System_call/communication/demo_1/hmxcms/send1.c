#include "hmxcms_client.h"

mqd_t mqserver, mqd_client;
cms_msg_t response;

int main ()
{
    struct mq_attr* mq_attr = (struct mq_attr*)malloc(sizeof(mq_attr));
    // mqd_t mqclient = cms_client_init(mq_attr, "/Tuan", "mq1", SEND_REQUEST, "ALL");
    mqd_client = cms_client_init(mq_attr,"name6","/mqclient6", SEND_REQUEST, "ALL");
    if (mqd_client == CMS_ERROR) {
        printf("INIT FAIL %d \n",mqd_client);
    }
    mqserver = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(mqserver == -1){
        printf("Can not open send queue. Open failed or server not existed\n");
        return mqserver;
    }
    //printf("number of mq_servat %d",mqserver);
    int result = cms_send(mqserver, REQUEST_MESSAGE_ALL, "name6", "/mqclient6", LOST_WAN, "", "daylatinnhanall");
    //printf("result = %d\n",result);
    mq_close(mqserver);
    mqd_client = mq_open("/mqclient6", O_RDONLY);
    do {
        cms_receive(mqd_client, &response);
        if (response.tag == RESPONSE_MESSAGE) {
            if (strcmp(response.payload.data, CMS_SEND_SUCCESS) == 0) {
                printf("SERVER: Send successful\n");
                break;
            } else if (strcmp(response.payload.data, CMS_SEND_FAIL) == 0) {
                printf("SERVER: Send failure\n");
                break;
            }
        }
    } while (1);
    mq_close(mqd_client);
    free(mq_attr);
    return 0;
}

