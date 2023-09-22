#include "lib/hmxcms_lib.h"


int main ()
{

    int result = cms_client_init(HMX_CLIENT_4);
    if(result == CMS_ERROR || result == CMS_FAIL){
        LOG_CLIENT_STATE("CHAM HET\n");
        return 0;
    }
    //LOG_CLIENT_STATE("Het nuoc cham\n");
    result = cms_subcribe_topic("gpon");
    int i;
    for(i = 0; i <105; i ++){
            char str[32];
            sprintf(str,"wan %d",i);
            LOG_CLIENT(str);
            int result = cms_subcribe_topic(str);
            
            
    }
    long long count_message_reveice = 0;
    printf("[GPON] RUN RECEIVE\n");
    while (1)
    {
        /* code */
        cms_msg_t * message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
        result = cms_client_receive(message);
        count_message_reveice++;
        if(count_message_reveice%600==0){
        printf("[HMX_CLIENT_4] Number message received : %lld data message: %s \n",count_message_reveice, message->data);
        }
        free(message);
        if(result == CMS_ERROR){
            break;
        }
        
    }
    result = cms_unsubcribe_topic("gpon");
    cms_client_close();
    return 0;
}