#include "hmxcms_client_receive.h"


int main ()
{
    struct mq_attr *mq_attr = create_attr(0,10,sizeof(cms_msg_t));
    cms_client_infor * my_mq = init_client(mq_attr,SERVER_QUEUE_NAME);
    if(my_mq == NULL){
        LOG_CLIENT_STATE("CHAM HET\n");
        free(mq_attr);
        return 0;
    }
    cms_msg_t * message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    while (1)
    {
        /* code */
        mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT | O_NONBLOCK,0666,NULL);
        int result = mq_receive(mqserver, (char *) message, sizeof(cms_msg_t), NULL);
        if(result >0){
            LOG_CLIENT_STATE("HELLO BANJ NHo\n");
        if(strcmp(message->source,"/daten_1")==0){
            mqd_t client = mq_open("/daten_1", O_WRONLY);
            struct cms_msg_t msg = create_message(RESPONSE_MESSAGE, "server", "respone", CMS_SUBCRIBE_SUCCESS);
            mq_send(client, (char *) &msg, sizeof(cms_msg_t), 0);
        }}
        
    }
    free(mq_attr);
    free(my_mq);
    return 0;
}