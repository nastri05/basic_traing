#include "hmxcms_server.h"

int max_topic;
long max_msg;
mqd_t mqd;
cms_client_t *head_client = NULL;
cms_monitor_info_t *head_monitor = NULL;
cms_msg_t *oldest_cms_msg;

void cms_sig_handler()
{
    CMS_INFO_LOG("Receive signal SIGINT. Closing server ...\n");
    cms_server_stop();
    exit(EXIT_SUCCESS);
}

int cms_full_queue_handler(cms_msg_t *msg, mqd_t mqdes, char *client_name)
{
    cms_payload_t *pl = get_payload_monitor_list(head_monitor, client_name);
    if ((msg->tag == CMS_RESPONSE_MESSAGE)) {
        if (pl == NULL) {
            //store message in crash list 
            cms_payload_t *cms_payload_tmp = create_payload(msg->topic, msg->source, msg->data, client_name);
            if (add_to_monitor(&head_monitor, cms_payload_tmp) == CMS_ERROR) { 
                CMS_DEBUG_LOG("add message to crash list failed \n");
                return CMS_ERROR;
            } else {
                CMS_DEBUG_LOG("add message to crash list successfully \n"); 
            }        
        } else {
            pl->send_count++;
        }
        //remove oldest message and add the new one
        int retvl = mq_receive(mqdes, (char *)oldest_cms_msg, sizeof(cms_msg_t), NULL);
        if (retvl == -1) {
            CMS_WARNING_LOG("Remove oldest message failed\n");
            return CMS_ERROR;
        } else {
            CMS_DEBUG_LOG("Remove oldest message successfully \n");
            if (mq_send(mqdes, (char *) msg, sizeof(cms_msg_t), 0) == -1) {
                CMS_DEBUG_LOG("Overwrite message failed\n");
                return CMS_ERROR;
            } else {
                CMS_DEBUG_LOG("Overwrite message successfully\n");
            }
        }
        //check client crash
        if ((pl != NULL) && (pl->send_count >= max_msg) && ((strcmp(pl->source, oldest_cms_msg->source) == 0) && (strcmp(pl->topic, oldest_cms_msg->source) == 0) && (strcmp(pl->data, oldest_cms_msg->data)== 0))) {
            //client crashed
            CMS_WARNING_LOG("Client is crashed \n");
            if (delete_from_monitor(&head_monitor, client_name) == CMS_ERROR)
                CMS_DEBUG_LOG("delete node in crash list failed \n");
            else 
                CMS_DEBUG_LOG("delete node in crash list successfully \n");
            //free(oldest_cms_msg);
            return CMS_ERROR;
        }
    } else if ((msg->tag == CMS_REQUEST_SEND_MESSAGE) || (msg->tag == CMS_REQUEST_SEND_TO_MESSAGE)) {
        print_monitor_list(head_monitor);
        if (pl == NULL) {
            //store message in crash list 
            cms_payload_t *cms_payload_tmp = create_payload(msg->source, msg->topic, msg->data, client_name);
            if (add_to_monitor(&head_monitor, cms_payload_tmp) == CMS_ERROR) {
                CMS_DEBUG_LOG("add message to crash list failed \n");
                return CMS_ERROR;
            } else {
                CMS_DEBUG_LOG("add message to crash list successfully \n");
            } 
        } else {
            pl->send_count++;
        }
        //remove oldest message and add the new one
        int retvl = mq_receive(mqdes, (char *)oldest_cms_msg, sizeof(cms_msg_t), NULL);
        if (retvl == -1) {
            //LOG_SERVER_INT(errno);
            CMS_WARNING_LOG("Remove oldest message failed\n");
            return CMS_ERROR;
        } else {
            CMS_DEBUG_LOG("Remove oldest message successfully \n");
            if (mq_send(mqdes, (char *) msg, sizeof(cms_msg_t), 0) == -1) {
                CMS_DEBUG_LOG("Overwrite message failed\n");
                return CMS_ERROR;
            } else {
                CMS_DEBUG_LOG("Overwrite message successfully\n");
            }
        }
        
        if ((pl != NULL) && 
            (pl->send_count >= max_msg) && 
            (strcmp(pl->source, oldest_cms_msg->source) == 0) &&
            (strcmp(pl->topic, oldest_cms_msg->topic) == 0) && 
            (strcmp(pl->data, oldest_cms_msg->data) == 0)) 
            {
            //client crashed
                CMS_WARNING_LOG("Client is crashed\n");
                char name_crash[64];
                strcpy(name_crash, pl->client_name);
                //printf("%s", pl->topic);
                if (delete_from_monitor(&head_monitor, client_name) == CMS_ERROR)
                    CMS_DEBUG_LOG("delete node in crash list failed \n");
                else 
                    CMS_DEBUG_LOG("delete node in crash list successfully \n");
                int index_gr[get_length_list(head_client)];
                int i, ret = 0;
                int num1 = get_index_by_name(head_client, name_crash, index_gr);
                for (i = (num1-1); i >= 0; i--)
                {
                    ret += delete_client(&head_client, index_gr[i]);
                }
                if (ret == CMS_SUCCESS) {
                    CMS_DEBUG_LOG("Delete crashed client successfully\n");
                    mq_unlink(name_crash);
                    print_list(head_client);
                } else {
                    CMS_DEBUG_LOG("Delete crashed client failed\n");
                }
                return CMS_ERROR;
        }
    }
    return CMS_SUCCESS;
}

int cms_server_send(cms_msg_t *msg, mqd_t mqdes, char *client_name)
{
    int ret = mq_send(mqdes, (char*) msg, sizeof(cms_msg_t), 0);
    if (ret == CMS_ERROR) {
        CMS_WARNING_LOG("Sending message failed\n");
        //printf("%d\n", errno);
        if (errno == EAGAIN) {
            //Handle queue full
            CMS_WARNING_LOG("Message queue is full\n");
            if (cms_full_queue_handler(msg, mqdes, client_name) == CMS_ERROR)
                return CMS_ERROR;
        } else if (errno == EMSGSIZE) {
            //Message size reach limit
            CMS_ERROR_LOG("The message size exceeds the limit \n");
            return CMS_ERROR;
        } else {
            // Other error, resend message
            if (mq_send(mqdes, (char *) msg, sizeof(cms_msg_t), 0) == CMS_ERROR) {
                CMS_ERROR_LOG("Resend message failed\n");
                return CMS_ERROR;
            } else {
                CMS_DEBUG_LOG("Resend message successful\n");
            }
            return CMS_ERROR;
        }
    } else {
        cms_payload_t *pl = get_payload_monitor_list(head_monitor, client_name);
        if (pl != NULL) {
            CMS_DEBUG_LOG("remove node in crash list\n");
            if (delete_from_monitor(&head_monitor, client_name) == CMS_ERROR)
                CMS_DEBUG_LOG("delete node in crash list failed \n");
            else 
                CMS_DEBUG_LOG("delete node in crash list successfully \n");
        }
        return CMS_SUCCESS;
    }
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
    int foo;
    int ret = CMS_SUCCESS;
    int index_gr[length_list];
    cms_msg_t cms_msg_resp, cms_msg_send;
    mqd_t mqd_tmp;
    cms_data_t *data_tmp;
    switch(msg->tag)
    {
        case (CMS_SUBCRIBE_MESSAGE):
            foo = get_index_by_topic(head_client, msg->topic, index_gr);
            mqd_tmp = mq_open(msg->source, O_RDWR | O_NONBLOCK);
            if ((num_topic == max_topic) && (foo == CMS_ERROR)) {
                CMS_WARNING_LOG("Reach maximum %d topic. Can not subcribe topic %s !\n", max_topic, msg->topic);
                cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_SUBCRIBE_FAIL);
                cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                mq_close(mqd_tmp);
                print_list(head_client);
                break;
            }
            data_tmp = create_data(msg->source, msg->topic);
            index = get_index_by_data(head_client, data_tmp);
            if (index != CMS_ERROR) {
                CMS_WARNING_LOG("Client %s already subcribed topic %s\n", msg->source, msg->topic);
                cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_SUBCRIBE_SUCCESS);
                cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
            } else {
                if (add_client(&head_client, data_tmp) == CMS_ERROR) {
                    CMS_WARNING_LOG("Client %s subcribe topic %s failed\n", msg->source, msg->topic);
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_SUBCRIBE_FAIL);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                } else {
                    CMS_INFO_LOG("Client %s subcribe topic %s successfully\n", msg->source, msg->topic);
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
                if (num_member == CMS_ERROR) {
                    CMS_WARNING_LOG("Not found client %s. Already unsubcribe all\n", msg->source);
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_UNSUBCRIBE_SUCCESS);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                    break;
                }
                for (i = (num_member-1); i >= 0; i--)
                {
                    ret += delete_client(&head_client, index_gr[i]);
                }
                if (ret == CMS_SUCCESS) {
                    CMS_INFO_LOG("Unsubcribe all topic client %s successfully\n", msg->source);
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_UNSUBCRIBE_SUCCESS);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                } else {
                    CMS_WARNING_LOG("Unsubcribe all topic client %s failed\n", msg->source);
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_UNSUBCRIBE_FAIL);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                }
            } else {
                //UNSUBCRIBE TOPIC
                data_tmp = create_data(msg->source, msg->topic);
                index = get_index_by_data(head_client, data_tmp);
                if (index == CMS_ERROR) {
                    CMS_WARNING_LOG("Not found topic %s. Unsubcribe failed.\n", msg->topic);
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_UNSUBCRIBE_FAIL);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                    break;
                }
                if (delete_client(&head_client, index) == CMS_ERROR) {
                    CMS_WARNING_LOG("Unsubcribe client %s topic %s failed\n", msg->source, msg->topic);
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_UNSUBCRIBE_FAIL);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                } else {
                    CMS_INFO_LOG("Unsubcribe client %s topic %s successfully\n", msg->source, msg->topic);
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
                for(i = (length_list-1); i >= 0 ; i--)
                {
                    data_tmp = get_data_by_index(head_client, i);
                    if (data_tmp == NULL) {
                        CMS_WARNING_LOG("Not found data at index %d in client list\n", i);
                        continue;
                    }
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
                    CMS_WARNING_LOG("Not found any client at topic %s!\n", msg->topic);
                    break;
                }
                for (i = (num_member-1); i >= 0; i--) 
                {
                    data_tmp = get_data_by_index(head_client, index_gr[i]);
                    if (data_tmp == NULL) {
                        CMS_WARNING_LOG("Not found data at index %d in client list\n", i);
                        continue;
                    }
                    //doesn't send to yourself
                    if (strcmp(data_tmp->client_name, msg->source) == 0) continue;
                    //open client queue and send message
                    mqd_tmp = mq_open(data_tmp->client_name, O_RDWR | O_NONBLOCK);
                    ret += cms_server_send(msg, mqd_tmp, data_tmp->client_name);
                    //printf("Message gui thu: %s %s %s\n" , msg->source, msg->topic, msg->data);
                    mq_close(mqd_tmp);
                }
            }
            if (ret == CMS_SUCCESS) 
                CMS_INFO_LOG("Client %s -> Topic %s: %s\n", msg->source, msg->topic, msg->data);
            else 
                CMS_WARNING_LOG("Forward message 'Client %s -> Topic %s: %s' failed\n", msg->source, msg->topic, msg->data);
            break;
        case (CMS_REQUEST_SEND_TO_MESSAGE):
            mqd_tmp = mq_open(msg->topic, O_RDWR | O_NONBLOCK);
            if (cms_server_send(msg, mqd_tmp, msg->topic) == CMS_SUCCESS)
                CMS_INFO_LOG("Client %s -> Client %s: %s\n", msg->source, msg->topic, msg->data);
            else 
                CMS_WARNING_LOG("Forward message 'Client %s -> Client %s: %s' failed\n", msg->source, msg->topic, msg->data);
            mq_close(mqd_tmp);
            break;
        case (CMS_RESPONSE_MESSAGE):
            if (strcmp(msg->topic, "") == 0) {
                CMS_INFO_LOG("Client %s -> Server: Receive message from client %s failed\n", msg->source, msg->data);
            } else {
                CMS_INFO_LOG("Client %s -> Server: Receive message topic %s from client %s failed\n", msg->source, msg->data, msg->topic);
            }
            break;
        case (CMS_CONFIG_SERVER):
            mqd_tmp = mq_open(msg->source, O_RDWR | O_NONBLOCK);
            if (strcmp(msg->topic, CMS_CTRL_SET_LOG) == 0) {
                if (strcmp(msg->data, "CMS_LOG_LEVEL_DEBUG") == 0) {
                    cms_set_log_level(CMS_LOG_LEVEL_DEBUG);
                } else if (strcmp(msg->data, "CMS_LOG_LEVEL_INFO") == 0) {
                    cms_set_log_level(CMS_LOG_LEVEL_INFO);
                } else if (strcmp(msg->data, "CMS_LOG_LEVEL_WARNING") == 0) {
                    cms_set_log_level(CMS_LOG_LEVEL_WARNING);
                } else if (strcmp(msg->data, "CMS_LOG_LEVEL_ERROR") == 0) {
                    cms_set_log_level(CMS_LOG_LEVEL_ERROR);
                } else if (strcmp(msg->data, "CMS_LOG_LEVEL_NONE") == 0) {
                    cms_set_log_level(CMS_LOG_LEVEL_NONE);
                } else {
                    CMS_WARNING_LOG("Unknown log level\n");
                    cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_CONFIG_FAIL);
                    cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                    mq_close(mqd_tmp);
                    break;
                }
                CMS_INFO_LOG("Set log level server: %s\n", msg->data);
                cms_msg_resp = cms_server_create_message(CMS_RESPONSE_MESSAGE, msg->source, msg->topic, CMS_CONFIG_SUCCESS);
                cms_server_send(&cms_msg_resp, mqd_tmp, msg->source);
                CMS_INFO_LOG("Response control: %d %s \n", cms_msg_resp.tag, cms_msg_resp.data);
                mq_close(mqd_tmp);
            }
            break;
        default:
            break;
    }
}

mqd_t cms_server_init()
{
    // CMS_CTRL_SET_LOG_level(CMS_LOG_LEVEL_INFO);
    struct mq_attr *attr = (struct mq_attr *)malloc(sizeof(struct mq_attr));
    oldest_cms_msg = (cms_msg_t *)malloc(sizeof(cms_msg_t));
    char line[MAX_LINE_LENGTH];
    FILE *file = fopen("cms_server.conf", "r");
    if (file == NULL) {
        CMS_ERROR_LOG("Failed to open cms server config file\n");
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
    CMS_INFO_LOG("%d\n", sizeof(cms_msg_t));
    CMS_INFO_LOG("%ld\n", attr->mq_msgsize);
    CMS_INFO_LOG("%ld\n", attr->mq_maxmsg);
    CMS_INFO_LOG("%d \n", max_topic);
    fclose(file);
    mqd_t mqd_tmp = mq_open(SERVER_QUEUE_NAME, O_RDWR | O_CREAT | O_NONBLOCK, 0666, attr);
    if (mqd_tmp == CMS_ERROR) {
        CMS_ERROR_LOG("Can not create server queue \n");
        return CMS_ERROR;
    } else {
        CMS_INFO_LOG("Create server queue successfully\n");
        return mqd_tmp;
    }
}

void cms_server_start(mqd_t mqdes)
{
    cms_msg_t cms_msg_recv;
    int count_err = 0;
    int i = 0;
    while (1)
    {
        usleep(500);
        int rcv = mq_receive(mqdes, (char *)&cms_msg_recv, sizeof(cms_msg_t), NULL);
        if (rcv == CMS_ERROR) {
            //printf("Error receiving message from server queue \n");
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                //Server queue empty
            } else {
                //Other error
                CMS_WARNING_LOG("Receiving message failed from server queue\n");
                //printf("errno: %d\n", errno);
                count_err ++;
                if (count_err > 3) {
                    CMS_ERROR_LOG("While reading, too many errors happened in the same time. Stop server \n");
                    break;
                }
            }
        } else {
            count_err = 0;
            i++;
            cms_msg_handler(&cms_msg_recv);
        }
    }
}

void cms_server_stop() 
{
    free_list(&head_client);
    free_monitor_list(&head_monitor);
    //mq_close(mqd);
    //mq_close(SERVER_QUEUE_NAME);
    mq_unlink(SERVER_QUEUE_NAME);
    CMS_INFO_LOG("Finished closing cms server\n");
    exit(EXIT_SUCCESS);
}

int main () 
{
    if (signal(SIGINT, cms_sig_handler) == SIG_ERR) {
		CMS_ERROR_LOG("Cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}
    mqd = cms_server_init();
    if (mqd == CMS_ERROR) {
        CMS_ERROR_LOG("Init server failed \n");
        exit(EXIT_FAILURE);
    }
    cms_server_start(mqd);
    cms_server_stop();
    return 0;
}

