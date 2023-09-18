#include "lib/hmxcms_lib.h"


int main ()
{   
    int count_error =0;
    mq_unlink(SERVER_QUEUE_NAME);
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
        mqd_t client;
        //mqd_t mqserver = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT | O_NONBLOCK,0666,NULL);
        int result = mq_receive(my_mq->my_mq, (char *) message, sizeof(cms_msg_t), NULL);
        if(result >0){
            LOG_CLIENT_STATE("HELLO BANJ NHo\n");
        if(strcmp(message->source,"/daten_1")==0){
            client = mq_open("/daten_1", O_WRONLY| O_NONBLOCK );
            struct cms_msg_t msg = create_message(CMS_RESPONSE_MESSAGE, "server", "respone", CMS_SUBCRIBE_SUCCESS);
            mq_send(client, (char *) &msg, sizeof(cms_msg_t), 0);
            mq_close(client);
            break;
        }}
    }
    mqd_t client = mq_open("/daten_1", O_WRONLY| O_NONBLOCK );
    int j = 0;
    while (j < 10 )
        {
            int i;
            for(i = 0 ;i <10;i++){
            char str[20];
            sprintf(str, "wan %d : %d", j, i);
            struct cms_msg_t msg = create_message(CMS_REQUEST_SEND_MESSAGE, "server", "wan", str);
            LOG_CLIENT(msg.data);
            int result = mq_send(client, (char *) &msg, sizeof(cms_msg_t), 0);
            LOG_CLIENT_INT(result);
            if(result == CMS_ERROR){
                usleep(100);
                count_error++;
                result = mq_send(client, (char *) &msg, sizeof(cms_msg_t), 0);
                LOG_CLIENT_INT(result);
                if(result == CMS_ERROR){
                    count_error++;
                }
                
            }
            usleep(100);
            
            struct cms_msg_t msg_1 = create_message(CMS_REQUEST_SEND_MESSAGE, "server", "lan", str);
            LOG_CLIENT(msg_1.data);
            
            result = mq_send(client, (char *) &msg_1, sizeof(cms_msg_t), 0);
            LOG_CLIENT_INT(result);
            
             if(result == CMS_ERROR){
                usleep(100);
                count_error++;
                result = mq_send(client, (char *) &msg_1, sizeof(cms_msg_t), 0);
                LOG_CLIENT_INT(result);
                if(result == CMS_ERROR){
                    count_error++;
                }
            }
            usleep(100);
        }
         sleep(1);
        // break;
        j++;
        }
        LOG_CLIENT_INT(count_error);
        mq_close(client);
    free(mq_attr);
    free(my_mq);
    return 0;
}