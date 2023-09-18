#include "libstatic/hmxcms_lib.h"

cms_client_infor * my_mq;

void cms_sig_handler()
{
    LOG_CLIENT_STATE("Receive signal SIGINT. Closing client ...\n");
    //close_client(my_mq);
    exit(EXIT_SUCCESS);
}

int main ()
{
    int i = 0;
    if (signal(SIGINT, cms_sig_handler) == SIG_ERR) {
        LOG_CLIENT_STATE("Cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }
    struct mq_attr *mq_attr = create_attr(0,10,sizeof(cms_msg_t));
    my_mq = init_client(mq_attr,"/mqa");
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
        
        int result = receive(my_mq, message);
        if (result != CMS_ERROR) {
            printf ("Message thu %d\n",i);
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