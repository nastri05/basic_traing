#include "lib/hmxcms_lib.h"


int main ()
{
    int result = cms_client_init(HMX_CLIENT_3);
    if(result == CMS_FAIL|| result == CMS_ERROR  ){
        LOG_CLIENT_STATE("CHAM HET\n");
        return 0;
    }
    //LOG_CLIENT_STATE("Het nuoc cham\n");
    result = cms_subcribe_topic("lan");
    long long count_message_reveice = 0;
    while (1)
    {
        /* code */
        cms_msg_t * message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
        int result = cms_client_receive(message);
        count_message_reveice++;
        if(count_message_reveice%600==0){
        printf("[HMX_CLIENT_3] Number message received : %lld data message: %s \n",count_message_reveice, message->data);
        }
        free(message);
        // LOG_CLIENT_INT(result);
        if(result == CMS_ERROR){
            break;
        }
        
    }
    result = cms_unsubcribe_topic("lan");
    cms_client_close();
    return 0;
}