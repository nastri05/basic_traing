#include "hmxcms_server.h"

int max_topic;
long max_msg;
mqd_t mqd;
cms_client_t *head_client = NULL;
cms_monitor_info_t *head_monitor = NULL;

void cms_sig_handler()
{
    LOG_SERVER_STATE("Receive signal SIGINT. Closing server ...\n");
    cms_server_stop();
    exit(EXIT_SUCCESS);
}

int cms_full_queue_handler(cms_msg_t *msg, mqd_t mqdes, char *client_name)
{
    cms_payload_t *pl;
    if ((msg->tag == CMS_SUBCRIBE_MESSAGE) || (msg->tag == CMS_UNSUBCRIBE_MESSAGE)) {
        pl = get_payload_monitor_list(&head_monitor, msg->source);
        if (pl == NULL) {
            //store message in crash list 
            cms_payload_t *cms_payload_tmp = create_payload(msg->topic, msg->source, msg->data, client_name);
            if (add_to_monitor(&head_monitor, cms_payload_tmp) == CMS_ERROR) { 
                LOG_SERVER_STATE("add message to crash list failed \n");
                return CMS_ERROR;
            } else {
                LOG_SERVER_STATE("add message to crash list successfully \n"); 
            }        
        } else {
            pl->send_count++;
        }
        //check client crash
        if ((pl->send_count >= max_msg) && ((strcmp(pl->source, msg->topic) + strcmp(pl->destination, msg->source) + strcmp(pl->data, msg->data)) == 0)) {
            //client crashed
            LOG_SERVER_STATE("Client is crashed \n");
            if (delete_from_monitor(&head_monitor, pl->destination) == CMS_ERROR)
                LOG_SERVER_STATE("delete node in crash list failed \n");
            else 
                LOG_SERVER_STATE("delete node in crash list successfully \n");
            return CMS_ERROR;
        } else {
            //remove oldest message and add the new one
            cms_msg_t *oldest_cms_msg;
            if (mq_receive(mqdes, (char *)oldest_cms_msg, sizeof(cms_msg_t), NULL) == -1) {
                LOG_SERVER_STATE("Remove oldest message failed\n");
                return CMS_ERROR;
            } else {
                LOG_SERVER_STATE("Remove oldest message successfully \n");
                if (mq_send(mqdes, (char *) msg, sizeof(cms_msg_t), 0) == -1) {
                    LOG_SERVER_STATE("Overwrite message failed\n");
                    return CMS_ERROR;
                } else {
                    LOG_SERVER_STATE("Overwrite message successfully\n");
                }
            }
        }
    } else if ((msg->tag == CMS_REQUEST_SEND_MESSAGE) || (msg->tag == CMS_REQUEST_SEND_TO_MESSAGE)) {
        print_monitor_list(head_monitor);
        pl = get_payload_monitor_list(&head_monitor, msg->topic);
        //printf(" %s %s %s %s %s %s\n", pl->source, msg->topic, pl->destination, msg->source, pl->data, msg->data);
        if (pl == NULL) {
            //store message in crash list 
            cms_payload_t *cms_payload_tmp = create_payload(msg->source, msg->topic, msg->data, client_name);
            if (add_to_monitor(&head_monitor, cms_payload_tmp) == CMS_ERROR) {
                LOG_SERVER_STATE("add message to crash list failed \n");
                return CMS_ERROR;
            } else {
                LOG_SERVER_STATE("add message to crash list successfully \n");
                printf("%s %s %s\n", msg->topic, msg->source, msg->data);
            } 
        } else {
            pl->send_count++;
        }
        //check client crash
        if ((pl != NULL) && (pl->send_count >= max_msg) && ((strcmp(pl->source, msg->source) + strcmp(pl->destination, msg->topic) + strcmp(pl->data, msg->data)) == 0)) {
            //client crashed
            LOG_SERVER_STATE("Client is crashed\n");
            char name_crash[64];
            strcpy(name_crash, pl->client_name);
            printf("%s", pl->destination);
            if (delete_from_monitor(&head_monitor, pl->destination) == CMS_ERROR)
                LOG_SERVER_STATE("delete node in crash list failed \n");
            else 
                LOG_SERVER_STATE("delete node in crash list successfully \n");
            int index_gr[get_length_list(head_client)];
            int i, ret = 0;
            int num1 = get_index_by_name(head_client, name_crash, index_gr);
            for (i = 0; i < num1; i++)
            {
                ret += delete_client(&head_client, index_gr[i]);
            }
            if (ret == CMS_SUCCESS) {
                LOG_SERVER_STATE("Delete crashed client successfully\n");
                print_list(head_client);
            } else {
                LOG_SERVER_STATE("Delete crashed client failed\n");
            }
            return CMS_ERROR;
        } else {
            //remove oldest message and add the new one
            cms_msg_t *oldest_cms_msg;
            if (mq_receive(mqdes, (char *)oldest_cms_msg, sizeof(cms_msg_t), NULL) == -1) {
                LOG_SERVER_STATE("Remove oldest message failed\n");
                return CMS_ERROR;
            } else {
                LOG_SERVER_STATE("Remove oldest message successfully \n");
                if (mq_send(mqdes, (char *) msg, sizeof(cms_msg_t), 0) == -1) {
                    LOG_SERVER_STATE("Overwrite message failed\n");
                    return CMS_ERROR;
                } else {
                    LOG_SERVER_STATE("Overwrite message successfully\n");
                }
            }
        }
    } else if (msg->tag == CMS_RESPONSE_MESSAGE) {
        // handle response message
    }
    return CMS_SUCCESS;
}

int cms_server_send(cms_msg_t *msg, mqd_t mqdes, char *client_name)
{
    if (mq_send(mqdes, (char*) msg, sizeof(cms_msg_t), 0) == CMS_ERROR) {
        LOG_SERVER_STATE("Error sending message\n");
        printf("%d\n", errno);
        if (errno == EAGAIN) {
            //Handle queue full
            LOG_SERVER_STATE("Message queue is full\n");
            if (cms_full_queue_handler(msg, mqdes, client_name) == CMS_ERROR)
                return CMS_ERROR;
        } else if (errno == EMSGSIZE) {
            //Message size reach limit
            LOG_SERVER_STATE("The message size exceeds the limit \n");
        } else {
            //Other error, resend message
            if (mq_send(mqdes, (char *) msg, sizeof(cms_msg_t), 0) == CMS_ERROR) {
                LOG_SERVER_STATE("Resend message failed\n");
                return CMS_ERROR;
            } else {
                LOG_SERVER_STATE("Resend message successful\n");
            }
        }
    }
    return CMS_SUCCESS;
}

cms_msg_t cms_server_create_message(int tag, char *source, char *topic, char *data)
{
    cms_msg_t cms_msg_dummy;
    cms_msg_dummy.tag = tag;
    strcpy(cms_msg_dummy.source, source);
    strcpy(cms_msg_dummy.topic, topic);
    strcpy(cms_msg_dummy.data, data);
    return cms_msg_dummy;
}

void cms_msg_handler(cms_msg_t *msg)
{
    int index, i, num_member;
    int length_list = get_length_list(head_client);
    int num_topic = count_topic(head_client);
    int ret = CMS_SUCCESS;
    int index_gr[length_list];
    cms_msg_t cms_msg_resp, cms_msg_send;
    mqd_t mqd_tmp;
    cms_data_t *data_tmp;
    switch(msg->tag)
    {
        case (CMS_SUBCRIBE_MESSAGE):
            if (num_topic == max_topic) {
                LOG_SERVER_STATE("Reach maximum topic. Can not subcribe more!\n");
                break;
            }
            data_tmp = create_data(msg->source, msg->topic);
            index = get_index_by_data(head_client, data_tmp);
            mqd_tmp = mq_open(msg->source, O_RDWR | O_NONBLOCK);
            if (index != CMS_ERROR) {
                LOG_SERVER_STATE("Client already subcribed topic\n");
                cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_SUBCRIBE_SUCCESS);
                cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
            } else {
                if (add_client(&head_client, data_tmp) == CMS_ERROR) {
                    LOG_SERVER_STATE("Client subcribe failed\n");
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_SUBCRIBE_FAIL);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                } else {
                    LOG_SERVER_STATE("Client subcribe successfully\n");
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_SUBCRIBE_SUCCESS);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                }
            }
            mq_close(mqd_tmp);
            print_list(head_client);
            break;
        case (CMS_UNSUBCRIBE_MESSAGE):
            mqd_tmp = mq_open(msg->source, O_RDWR | O_NONBLOCK);
            if (strcmp(msg->topic, "ALL") == 0) {
                //UNSUBCRIBE ALL
                num_member = get_index_by_name(head_client, msg->source, index_gr);
                for (i = 0; i < num_member; i++)
                {
                    ret += delete_client(&head_client, index_gr[i]);
                }
                if (ret == CMS_SUCCESS) {
                    LOG_SERVER_STATE("Unsubcribe client successfully\n");
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_UNSUBCRIBE_SUCCESS);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                } else {
                    LOG_SERVER_STATE("Unsubcribe client failed\n");
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_UNSUBCRIBE_FAIL);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                }
            } else {
                //UNSUBCRIBE TOPIC
                data_tmp = create_data(msg->source, msg->topic);
                index = get_index_by_data(head_client, data_tmp);
                if (delete_client(&head_client, index) == CMS_ERROR) {
                    LOG_SERVER_STATE("Unsubcribe client failed\n");
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_UNSUBCRIBE_FAIL);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                } else {
                    LOG_SERVER_STATE("Unsubcribe client successfully\n");
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_UNSUBCRIBE_SUCCESS);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                }
            }
            mq_close(mqd_tmp);
            print_list(head_client);
            break;
        case (CMS_REQUEST_SEND_MESSAGE): 
            if (strcmp(msg->topic, "ALL") == 0) {
                //SEND ALL
                for(i = 0; i < length_list; i++)
                {
                    data_tmp = get_data_by_index(head_client, i);
                    //doesn't send to yourself
                    if (strcmp(data_tmp->client_name, msg->source) == 0) continue;
                    //open client queue and send message
                    mqd_tmp = mq_open(data_tmp->client_name, O_RDWR | O_NONBLOCK);
                    ret += cms_server_send(msg, mqd_tmp, data_tmp->client_name);
                    mq_close(mqd_tmp);
                }
            } else {
                // SEND TOPIC
                num_member = get_index_by_topic(head_client, msg->topic, index_gr);
                if (num_member == CMS_ERROR) {
                    LOG_SERVER_STATE("ERROR: NOT FOUND ANY CLIENT IN THIS TOPIC!\n");
                    break;
                }
                for (i = 0; i < num_member; i++) 
                {
                    data_tmp = get_data_by_index(head_client, i);
                    //doesn't send to yourself
                    if (strcmp(data_tmp->client_name, msg->source) == 0) continue;
                    //open client queue and send message
                    mqd_tmp = mq_open(data_tmp->client_name, O_RDWR | O_NONBLOCK);
                    ret += cms_server_send(msg, mqd_tmp, data_tmp->client_name);
                    mq_close(mqd_tmp);
                }
            }
            if (ret == CMS_SUCCESS) 
                LOG_SERVER_STATE("Server forwarded request send message successfully\n");
            else 
                LOG_SERVER_STATE("Server forwarded request send message failed\n");
            break;
        case (CMS_REQUEST_SEND_TO_MESSAGE):
            mqd_tmp = mq_open(msg->topic, O_RDWR | O_NONBLOCK);
            if (cms_server_send(msg, mqd_tmp, msg->topic) == CMS_SUCCESS)
                LOG_SERVER_STATE("Server forwarded request send message successfully\n");
            else 
                LOG_SERVER_STATE("Server forwarded request send message failed\n");
            mq_close(mqd_tmp);
            break;
        default:
            break;
    }
}

mqd_t cms_server_init()
{
    struct mq_attr *attr = (struct mq_attr *)malloc(sizeof(struct mq_attr));
    char line[MAX_LINE_LENGTH];
    FILE *file = fopen("cms_server.conf", "r");
    if (file == NULL) {
        LOG_SERVER_STATE("Failed to open cms server config file\n");
        return CMS_ERROR;
    }
    while (fgets(line, sizeof(line), file)) 
    {
        char dummy[MAX_LINE_LENGTH];
        line[strcspn(line, "\n")] = '\0';
        //blank line
        if (sscanf(line, "%s", dummy) == EOF) continue;
        //comment #%*[^\0] %[#] add space if wrong ...
        if (sscanf(line, "%[#]", dummy) == 1) continue;
        //normal line
        if (sscanf(line, "mq_flags: %ld", &attr->mq_flags) == 1) continue;
        if (sscanf(line, "mq_maxmsg: %ld", &attr->mq_maxmsg) == 1) {
            max_msg = attr->mq_maxmsg;
            continue;
        }
        if (sscanf(line, "mq_msgsize: %ld", &attr->mq_msgsize) == 1) continue;
        if (sscanf(line, "max_topic: %d", &max_topic) == 1) continue;
    }
    printf("%d\n", sizeof(cms_msg_t));
    printf("%ld\n", attr->mq_msgsize);
    printf("%ld\n", attr->mq_maxmsg);
    // attr->mq_maxmsg = 10;
    // attr->mq_msgsize = sizeof(cms_msg_t);
    fclose(file);
    mqd_t mqd_tmp = mq_open(SERVER_QUEUE_NAME, O_RDWR | O_CREAT | O_NONBLOCK, 0666, attr);
    if (mqd_tmp == CMS_ERROR) {
        LOG_SERVER_STATE("Can not create server queue \n");
        return CMS_ERROR;
    } else {
        LOG_SERVER_STATE("Create server queue successfully\n");
        return mqd_tmp;
    }
}

void cms_server_start(mqd_t mqdes)
{
    cms_msg_t *cms_msg_recv;
    int count_err = 0;
    while (1)
    {
        int rcv = mq_receive(mqdes, (char *)cms_msg_recv, sizeof(cms_msg_t), NULL);
        if (rcv == CMS_ERROR) {
            //printf("Error receiving message from server queue \n");
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                //Server queue empty
            } else {
                //Other error
                LOG_SERVER_STATE("Error receiving message from server queue\n");
                printf("errno: %d\n", errno);
                count_err ++;
                if (count_err > 3) {
                    LOG_SERVER_STATE("While reading, too many errors happened in the same time. Stop server \n");
                    break;
                }
            }
        } else {
            count_err = 0;
            cms_msg_handler(cms_msg_recv);
        }
    }
}

void cms_server_stop() 
{
    free_list(&head_client);
    free_monitor_list(&head_monitor);
    mq_close(mqd);
    mq_unlink(SERVER_QUEUE_NAME);
    LOG_SERVER_STATE("Finished closing cms server\n");
    exit(EXIT_SUCCESS);
}

int main () 
{
    if (signal(SIGINT, cms_sig_handler) == SIG_ERR) {
		LOG_SERVER_STATE("Cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}
    mqd = cms_server_init();
    cms_server_start(mqd);
    cms_server_stop();
    return 0;
}
