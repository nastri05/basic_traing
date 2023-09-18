#include "hmxcms_receive2.h"

cms_client_infor *my_mq;

void cms_sig_handler()
{
    LOG_CLIENT_STATE("Receive signal SIGINT. Closing client ...\n");
    cms_unsubcribe_topic(my_mq, "lan");
    cms_client_close(my_mq);
    exit(EXIT_SUCCESS);
}

int main ()
{
    int i = 0;
    if (signal(SIGINT, cms_sig_handler) == SIG_ERR) {
        LOG_CLIENT_STATE("Cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }
    struct mq_attr *mq_attr = cms_create_attr(0, 10, sizeof(cms_msg_t));
    my_mq = cms_client_init(mq_attr,"/mqb");
    if(my_mq == NULL){
        LOG_CLIENT_STATE("CHAM HET\n");
        free(mq_attr);
        return 0;
    }
    //LOG_CLIENT_STATE("Het nuoc cham\n");
    int result = cms_subcribe_topic(my_mq, "lan");
    cms_msg_t * message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    while (1)
    {
        /* code */
        
        int result = cms_client_receive(my_mq, message);
        if (result != CMS_ERROR) {
            printf ("[LAN] Message thu %d\n",i);
            i++;
        }
        // if(result == CMS_ERROR){
        //     break;
        // }
        
    }
    // result = subcribe_topic(my_mq, "lan");
    free(mq_attr);
    free(my_mq);
    return 0;
}