#include "hmxcms_server.h"

struct mq_attr attr;
long flags = 0;
long maxmsg = 10;
long msgsize = 320;
mqd_t mqd;
int i;
cms_msg_t msg, msg_b;
pthread_mutex_t cms_mutex;
int num_thread = 0;
cms_client_t *head = NULL;




void *msg_handler(void *arg)
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
                index = get_index_by_data(head, data);
                LOG_CLIENT_INT(index);
                if (index != CMS_ERROR) {
                    LOG_CLIENT_STATE("CLIENT ALREADY EXISTED\n");
                    mqdes = mq_open(msg->payload.mq_name, O_WRONLY);
                    cms_send(mqdes, RESPONSE_MESSAGE, "SERVER", SERVER_QUEUE_NAME, 0, "", CMS_CONFIG_SUCCESS);
                    mq_close(mqdes);
                } else {
                    mqdes = mq_open(msg->payload.mq_name, O_WRONLY);
                    if (add_client(&head, data) == CMS_ERROR) {
                        //fail
                        LOG_CLIENT_STATE("ADD CLIENT TO LINKED LIST FAIL\n");
                        cms_send(mqdes, RESPONSE_MESSAGE, "SERVER", SERVER_QUEUE_NAME, 0, "", CMS_CONFIG_FAIL);
                    } else {
                        //success
                        LOG_CLIENT_STATE("ADD CLIENT TO LINKED LIST SUCCESS\n");
                        cms_send(mqdes, RESPONSE_MESSAGE, "SERVER", SERVER_QUEUE_NAME, 0, "", CMS_CONFIG_SUCCESS);
                    }
                    mq_close(mqdes);
                }
            } else if (strcmp(msg->payload.data, CHANGE_CLIENT) == 0) {
                //code change data in linkedlist
                data_t *data = create_data(msg->payload.client_name, msg->payload.mq_name, msg->payload.type, msg->payload.topic);
                index = get_index_by_name(head, msg->payload.client_name);
                if (index == CMS_ERROR) {
                    LOG_CLIENT_STATE("ERROR: CLIENT NOT FOUND!\n");
                    pthread_mutex_unlock(&cms_mutex);
                    break;
                }
                mqdes = mq_open(msg->payload.mq_name, O_WRONLY);
                if (change_data_client(&head, index, data) == CMS_ERROR) {
                    //fail
                    LOG_CLIENT_STATE("CHANGE DATA OF CLIENT IN LINKED LIST FAIL\n");
                    cms_send(mqdes, RESPONSE_MESSAGE, "SERVER", SERVER_QUEUE_NAME, 0, "", CMS_CONFIG_FAIL);
                } else {
                    //success
                    LOG_CLIENT_STATE("CHANGE DATA OF CLIENT IN LINKED LIST SUCCESS\n");
                    cms_send(mqdes, RESPONSE_MESSAGE, "SERVER", SERVER_QUEUE_NAME, 0, "", CMS_CONFIG_SUCCESS);
                }
                mq_close(mqdes);
            } else if (strcmp(msg->payload.data, REMOVE_CLIENT) == 0) {
                index = get_index_by_name(head, msg->payload.client_name);
                if (index == CMS_ERROR) {
                    LOG_CLIENT_STATE("ERROR: CLIENT NOT FOUND!\n");
                    pthread_mutex_unlock(&cms_mutex);
                    break;
                } else {
                    mqdes = mq_open(msg->payload.mq_name, O_WRONLY);
                    if (delete_client(&head, index) == CMS_ERROR) {
                        LOG_CLIENT_STATE("REMOVE CLIENT FAIL\n");
                        cms_send(mqdes, RESPONSE_MESSAGE, "SERVER", SERVER_QUEUE_NAME, 0, "", CMS_CONFIG_FAIL);
                    } else {
                        LOG_CLIENT_STATE("REMOVE CLIENT SUCCESS\n");
                        cms_send(mqdes, RESPONSE_MESSAGE, "SERVER", SERVER_QUEUE_NAME, 0, "", CMS_CONFIG_SUCCESS);
                    }
                    mq_close(mqdes);
                }
            }
            print_list(head);
            pthread_mutex_unlock(&cms_mutex);
            break;
        case (REQUEST_MESSAGE_CLIENT):
            //request message to destination client
            index = get_index_by_name(head, msg->payload.topic);
            if (index == CMS_ERROR) {
                LOG_CLIENT_STATE("ERROR: CLIENT NOT FOUND!\n");
                break;
            }
            data_t *data = get_data_by_index(head, index);
            mqdes = mq_open(data->mq_name, O_WRONLY);
            retvl = cms_send(mqdes, REQUEST_MESSAGE_CLIENT, msg->payload.client_name, msg->payload.mq_name, msg->payload.type, msg->payload.topic, msg->payload.data);
            mq_close(mqdes);
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
                cms_send(mqsrc, RESPONSE_MESSAGE, data->client_name, data->mq_name, 0, "", CMS_SEND_SUCCESS);
            else
                cms_send(mqsrc, RESPONSE_MESSAGE, data->client_name, data->mq_name, 0, "", CMS_SEND_FAIL);
            mq_close(mqsrc);
            break;
        case (REQUEST_MESSAGE_GROUP) :
            //request message to group
            LOG_CLIENT_STATE("??? REQUEST_MESSAGE_GROUP\n");
            num_member = get_index_by_topic(head, msg->payload.topic, index_gr);
            if (num_member == CMS_ERROR) {
                LOG_CLIENT_STATE("ERROR: CLIENT NOT FOUND!\n");
                break;
            }
            for (i = 0; i < num_member; i++)
            {
                data_t *data = get_data_by_index(head, index_gr[i]);
                mqdes = mq_open(data->mq_name, O_WRONLY); 
                retvl += cms_send(mqdes, REQUEST_MESSAGE_GROUP, msg->payload.client_name, msg->payload.mq_name, msg->payload.type, msg->payload.topic, msg->payload.data);
                mq_close(mqdes);
            }
             LOG_CLIENT_STATE("??? REQUEST_MESSAGE_GROUP\n");
            //response message to source client
            mqsrc = mq_open(msg->payload.mq_name, O_WRONLY);
            if (retvl == CMS_SUCCESS)
                cms_send(mqsrc, RESPONSE_MESSAGE, msg->payload.topic, SERVER_QUEUE_NAME, 0, "", CMS_SEND_SUCCESS);
            else
                cms_send(mqsrc, RESPONSE_MESSAGE, msg->payload.topic, SERVER_QUEUE_NAME, 0, "", CMS_SEND_FAIL);
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
                cms_send(mqsrc, RESPONSE_MESSAGE, msg->payload.topic, SERVER_QUEUE_NAME, 0, "", CMS_SEND_SUCCESS);
            else
                cms_send(mqsrc, RESPONSE_MESSAGE, msg->payload.topic, SERVER_QUEUE_NAME, 0, "", CMS_SEND_FAIL);
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
    ret = pthread_create(&thread_id, NULL, msg_handler, (void *)msg);
    if (ret) {
        LOG_CLIENT_STATE("ERROR: CREATING THREAD FAIL");
        LOG_CLIENT_INT(ret);
    }
    //pthread_join(thread_id, NULL);
    ret = pthread_detach(thread_id);
    if (ret) {
        LOG_CLIENT_STATE("ERROR: THREAD DETACH FAIL\n");
    }
    num_thread++;
}

mqd_t cms_server_init()
{
    pthread_mutex_init(&cms_mutex, NULL);
    //set server message queue attributes
    // if(set_mq_attr(&attr, flags, maxmsg, msgsize) == CMS_ERROR) {
    //     printf("Set server message queue attributes fail\n");
    //     return -1;
    // }
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(cms_msg_t);
    attr.mq_curmsgs = 0;
    //create server message queue
    mq_unlink(SERVER_QUEUE_NAME);
    mqd = mq_open(SERVER_QUEUE_NAME, O_RDWR| O_CREAT | O_NONBLOCK, 0666, &attr);
    if (mqd == -1) {
        LOG_CLIENT_STATE("Create server message queue fail\n");
        return -1;
    }
    return mqd;
}

void cms_server_start()
{
    while (num_thread < MAX_THREADS)
    {
        //printf("giatri \n");
        //sleep(1);
        int read = cms_receive(mqd, &msg_b);
        if (read < 0) {
            LOG_CLIENT_STATE("Can not receive message from server queue \n");
        } else {
            cms_msg_t msgtest = msg_b;
            //printf(" gia tri %d %s\n", msg_b.tag, msg_b.payload.client_name);
            if (create_handling_pthread(&msgtest) == CMS_ERROR)
            {
                LOG_CLIENT_STATE("Create thread fail\n");
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
    LOG_CLIENT_STATE("mqserver id :");
    LOG_CLIENT_INT(cms_server_init());
    cms_server_start();
    //cms_server_stop();
    return 0;
}

