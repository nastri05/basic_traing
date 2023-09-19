#include "lib/hmxcms_lib.h"


int main ()
{

    struct mq_attr *mq_attr = cms_create_attr(0,10,sizeof(cms_msg_t));
    cms_client_infor * my_mq = cms_client_init(mq_attr,HMX_CLIENT_3);
    if(my_mq == NULL){
        LOG_CLIENT_STATE("CHAM HET\n");
        free(mq_attr);
        return 0;
    }
    //LOG_CLIENT_STATE("Het nuoc cham\n");
    int i ;
    for(i = 0; i <110; i ++){
            char str[32];
            sprintf(str,"wan %d",i);
            LOG_CLIENT(str);
            int result = cms_subcribe_topic(my_mq, str);
            
    }
    long long count_message_reveice = 0;
    while (1)
    {
        /* code */
        cms_msg_t * message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
        int result = cms_client_receive(my_mq,message);
        LOG_CLIENT_INT(count_message_reveice);
        count_message_reveice++;
        if(count_message_reveice%600==0){
        // printf("[HMX_CLIENT_3] Number message received : %lld data message: %s \n",count_message_reveice, message->data);
        }
        free(message);
        LOG_CLIENT_INT(result);
        if(result == CMS_ERROR){
            break;
        }
        
    }
    // result = subcribe_topic(my_mq, "lan");
    free(mq_attr);
    free(my_mq);
    return 0;
}