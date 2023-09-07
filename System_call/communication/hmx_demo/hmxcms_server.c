#include "inc/hmxcms_server.h"

#define MAX_THREADS 20

struct mq_attr attr;
long flags = O_NONBLOCK;
long maxmsg = 20;
long msgsize = 320;
mqd_t mqd;
int i;
cms_msg_t msg;
pthread_mutex_t cms_mutex;
int num_thread = 0;
 cms_client_t* head = NULL;

static void *msg_handler(void *arg)
{
    cms_msg_t *msg = (cms_msg_t*)arg;
    int leng_list = get_length_list(head);
    int index, num_member;
    int index_gr[leng_list];
    mqd_t mqsrc, mqdes;
    int retvl = 0;
    switch (msg->tag)
    {
        case (CONFIG_MESSAGE):
            //configuration message
            pthread_mutex_lock(&cms_mutex);
            if (strcmp(msg->payload.data, ADD_CLIENT) == 0) {
                data_t *data = create_data(msg->payload.client_name, msg->payload.mq_name, msg->payload.type, msg->payload.topic);
                mqdes = mq_open(msg->payload.mq_name, O_WRONLY);
                if (add_client(&head, data) == CMS_ERROR) {
                    //fail
                    printf("add client to linked list fail\n");
                    cms_send(mqdes, RESPONSE_MESSAGE, "server", SERVER_QUEUE_NAME, 0, NULL, CMS_CONFIG_FAIL);
                } else {
                    //success
                    printf("add client to linked list success\n");
                    cms_send(mqdes, RESPONSE_MESSAGE, "server", SERVER_QUEUE_NAME, 0, NULL, CMS_CONFIG_SUCCESS);
                }
                mq_close(mqdes);
            } else if (strcmp(msg->payload.data, CHANGE_CLIENT) == 0) {
                //code change data in linkedlist
            } else if (strcmp(msg->payload.data, REMOVE_CLIENT) == 0) {
                index = get_index_by_name(head, msg->payload.client_name);
                if (index == CMS_ERROR) {
                    printf("get index by name error \n");
                    exit(EXIT_FAILURE);
                } else {
                    mqdes = mq_open(msg->payload.mq_name, O_WRONLY);
                    if (delete_client(&head, index) == CMS_ERROR) {
                        printf("remove client fail\n");
                        cms_send(mqdes, RESPONSE_MESSAGE, "server", SERVER_QUEUE_NAME, 0, NULL, CMS_CONFIG_FAIL);
                    } else {
                        printf("remove client fail\n");
                        cms_send(mqdes, RESPONSE_MESSAGE, "server", SERVER_QUEUE_NAME, 0, NULL, CMS_CONFIG_SUCCESS);
                    }
                    mq_close(mqdes);
                }
            }
            pthread_mutex_unlock(&cms_mutex);
            break;
        case (REQUEST_MESSAGE_CLIENT):
            //request message to destination client
            index = get_index_by_name(head, msg->payload.topic);
            data_t *data = get_data_by_index(head, index);
            mqdes = mq_open(data->mq_name, O_WRONLY);
            retvl = cms_send(mqdes, REQUEST_MESSAGE_CLIENT, msg->payload.client_name, msg->payload.mq_name, msg->payload.type, msg->payload.topic, msg->payload.data);
            //response message to source client
            mqsrc = mq_open(msg->payload.mq_name, O_WRONLY);
            if (retvl == CMS_SUCCESS)
                cms_send(mqsrc, RESPONSE_MESSAGE, data->client_name, data->mq_name, 0, NULL, CMS_SEND_SUCCESS);
            else
                cms_send(mqsrc, RESPONSE_MESSAGE, data->client_name, data->mq_name, 0, NULL, CMS_SEND_FAIL);
            mq_close(mqdes);
            mq_close(mqsrc);
            break;
        case (REQUEST_MESSAGE_GROUP) :
            //request message to group
            num_member = get_index_by_topic(head, msg->payload.topic, index_gr);
            for (i = 0; i < num_member; i++)
            {
                data_t *data = get_data_by_index(head, index_gr[i]);
                mqdes = mq_open(data->mq_name, O_WRONLY); 
                retvl += cms_send(mqdes, REQUEST_MESSAGE_GROUP, msg->payload.client_name, msg->payload.mq_name, msg->payload.type, msg->payload.topic, msg->payload.data);
                mq_close(mqdes);
            }
            //response message to source client
            mqsrc = mq_open(msg->payload.mq_name, O_WRONLY);
            if (retvl == CMS_SUCCESS)
                cms_send(mqsrc, RESPONSE_MESSAGE, msg->payload.topic, SERVER_QUEUE_NAME, 0, NULL, CMS_SEND_SUCCESS);
            else
                cms_send(mqsrc, RESPONSE_MESSAGE, msg->payload.topic, SERVER_QUEUE_NAME, 0, NULL, CMS_SEND_FAIL);
            mq_close(mqsrc);
            break;
        case (REQUEST_MESSAGE_ALL):
            //request message to all
            for (i = 0; i < leng_list; i++)
            {
                data_t *data = get_data_by_index(head, i);
                mqdes = mq_open(data->mq_name, O_WRONLY); 
                retvl += cms_send(mqdes, REQUEST_MESSAGE_ALL, msg->payload.client_name, msg->payload.mq_name, msg->payload.type, msg->payload.topic, msg->payload.data);
                mq_close(mqdes); 
            }
            //response message to source client
            mqsrc = mq_open(msg->payload.mq_name, O_WRONLY);
            if (retvl == CMS_SUCCESS)
                cms_send(mqsrc, RESPONSE_MESSAGE, msg->payload.topic, SERVER_QUEUE_NAME, 0, NULL, CMS_SEND_SUCCESS);
            else
                cms_send(mqsrc, RESPONSE_MESSAGE, msg->payload.topic, SERVER_QUEUE_NAME, 0, NULL, CMS_SEND_FAIL);
            mq_close(mqsrc); 
            break;
        default:
            break;
    }
    num_thread--;
    pthread_exit(NULL);
}

int create_handling_pthread(cms_msg_t *msg)
{
    pthread_t thread_id;
    int ret;
    ret = pthread_create(&thread_id, NULL, &msg_handler, &msg);
    if (ret) {
        printf("Error creating thread, return code: %d \n", ret);
    }
    ret = pthread_detach(thread_id);
    if (ret) {
        printf("Thread detach failed\n");
    }
    num_thread++;
}

mqd_t cms_server_init()
{
    pthread_mutex_init(&cms_mutex, NULL);
    //set server message queue attributes
    if(set_mq_attr(&attr, flags, maxmsg, msgsize) == CMS_ERROR) {
        printf("Set server message queue attributes fail\n");
        return -1;
    }
    //create server message queue
    mqd = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDWR, 0777, &attr);
    if (mqd == -1) {
        printf("Create server message queue fail\n");
        return -1;
    }
    return mqd;
}

void cms_server_start()
{
    while (num_thread<MAX_THREADS)
    {
        if (cms_receive(mqd, &msg) == CMS_ERROR) {
            printf("Can not receive message from server queue \n");
        } else {
            if (create_handling_pthread(&msg) == CMS_ERROR)
            {
                printf("Create thread fail\n");
            }
        }
    }
}

void cms_server_stop()
{
    free_list(&head);
    mq_unlink(SERVER_QUEUE_NAME);
    pthread_mutex_destroy(&cms_mutex);
    exit(EXIT_SUCCESS);
}

int main ()
{
   
    cms_server_init();
    cms_server_start();
    //cms_server_stop();
    return 0;
}

