#include "hmxcms_client_receive.h"


int main ()
{
    struct mq_attr *mq_attr = create_attr(0,10,sizeof(cms_msg_t));
    cms_client_infor * my_mq = init_client(mq_attr,"/daten_1");
    if(my_mq == NULL){
        LOG_CLIENT_STATE("CHAM HET\n");
        free(mq_attr);
        return 0;
    }
    //LOG_CLIENT_STATE("Het nuoc cham\n");
    int result = subcribe_topic(my_mq, "wan");
    cms_msg_t * message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    while (1)
    {
        /* code */
        int result =receive(my_mq,message);
        if(result == CMS_ERROR){
            break;
        }
        
    }
    free(mq_attr);
    free(my_mq);
    return 0;
}