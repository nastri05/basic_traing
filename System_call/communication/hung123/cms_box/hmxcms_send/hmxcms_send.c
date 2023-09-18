#include "hmxcms_send.h"


int main ()
{
    int i = 0;
    struct mq_attr *mq_attr = cms_create_attr(0,10,sizeof(cms_msg_t));
    cms_client_infor * my_mq = cms_client_init(mq_attr,"/send2");
    if(my_mq == NULL){
        LOG_CLIENT_STATE("CHAM HET\n");
        free(mq_attr);
        return 0;
    }
    //LOG_CLIENT_STATE("Het nuoc cham\n");
    // int result = subcribe_topic(my_mq, "wan");
    cms_msg_t * message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    while (i<600)
    {
        /* code */
        // int result =receive(my_mq,message);
        // if(result == CMS_ERROR){
        //     break;
        // }
        int result = cms_client_send_to(my_mq, RECEIVE_QUEUE_NAME, "wan1");
        //printf("Send to: %d\n", result);
        int result1 = cms_client_send(my_mq, "wan", "saiwanroi");

        //printf("Send: %d\n", result1);
        int result2 = cms_client_send(my_mq, "lan", "lan1");
        int result3 = cms_client_send(my_mq, "lan", "sailanroi");
        i++;
    }
    cms_client_close(my_mq);
    free(mq_attr);
    free(my_mq);
    return 0;
}