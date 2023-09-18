#include "libstatic/hmxcms_lib.h"


int main ()
{
    int i = 0;
    int count_error = 0;
    struct mq_attr *mq_attr = create_attr(0,10,sizeof(cms_msg_t));
    cms_client_infor * my_mq = init_client(mq_attr,"/send2");
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
        int result = send(my_mq, "wan", "wan1");
        if(result == CMS_ERROR || result == CMS_FAIL){
                    count_error++;
                }
        result =send(my_mq, "wan", "saiwanroi");
        if(result == CMS_ERROR || result == CMS_FAIL){
                    count_error++;
                }
        result = send(my_mq, "lan", "lan1");
        if(result == CMS_ERROR || result == CMS_FAIL){
                    count_error++;
                }
        result = send(my_mq, "lan", "sailanroi");
        if(result == CMS_ERROR || result == CMS_FAIL){
                    count_error++;
                }
        i++;
    }
    LOG_CLIENT_INT(count_error);
    close_client(my_mq);
    free(mq_attr);
    free(my_mq);
    return 0;
}