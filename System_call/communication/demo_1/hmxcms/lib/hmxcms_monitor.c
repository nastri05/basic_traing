#include "hmxcms_monitor.h"

cms_payload_t* create_payload(char* source, char* topic, char* data, char *client_name) {
    cms_payload_t *payload_of_datagram = (cms_payload_t*) malloc(sizeof(cms_payload_t));
    strcpy(payload_of_datagram->source, source);
    strcpy(payload_of_datagram->topic, topic);
    strcpy(payload_of_datagram->data, data);
    strcpy(payload_of_datagram->client_name, client_name);
    payload_of_datagram->send_count = 0;
    return payload_of_datagram;
}

int add_to_monitor(cms_monitor_info_t **head_monitor_list, cms_payload_t* payload){
    cms_monitor_info_t * new_client_monitored = (cms_monitor_info_t*) malloc(sizeof(cms_monitor_info_t));
    new_client_monitored->payload = payload;
    new_client_monitored->next=NULL;

    if (new_client_monitored == NULL){
        LOG_MONITOR_STATE("---------------------------------\n");
        LOG_MONITOR_STATE("Create list of clients for monitoring failed \n");
        LOG_MONITOR_STATE("---------------------------------\n");
        return CMS_ERROR;        
    }
    cms_monitor_info_t *tmp = *head_monitor_list;
    if(*head_monitor_list == NULL){
        *head_monitor_list = new_client_monitored;
        LOG_MONITOR_STATE("---------------------------------\n");
        LOG_MONITOR_STATE("Add client to monitor list success \n");
        LOG_MONITOR_STATE("---------------------------------\n");
        return CMS_SUCCESS;
    }
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next= new_client_monitored;
    LOG_MONITOR_STATE("---------------------------------\n");
    LOG_MONITOR_STATE("Add client to monitor list success \n");
    LOG_MONITOR_STATE("---------------------------------\n");
    return CMS_SUCCESS;    
}

int delete_from_monitor(cms_monitor_info_t **head_monitor_list, char* client_name){
    cms_monitor_info_t * tmp = *head_monitor_list;
    cms_monitor_info_t * delete = NULL;

    if (tmp == NULL){
        LOG_MONITOR_STATE("---------------------------------\n");
        LOG_MONITOR_STATE("Monitor list is NULL \n");
        LOG_MONITOR_STATE("---------------------------------\n");
        return CMS_ERROR;        
    }

    if (strcmp(tmp->payload->client_name, client_name) == 0){
        *head_monitor_list= (*head_monitor_list)->next;
        LOG_MONITOR_STATE("---------------------------------\n");
        LOG_MONITOR_STATE("Delete from monitor list completed\n");
        LOG_MONITOR_STATE("---------------------------------\n");
        free(tmp->payload);
        tmp->payload = NULL;
        free(tmp);
        tmp = NULL;
        return CMS_SUCCESS;        
    }

    while (tmp->next != NULL) {
        if (strcmp(tmp->next->payload->client_name, client_name) == 0){

            cms_monitor_info_t* delete = tmp->next;
            tmp->next = delete->next;

            LOG_MONITOR_STATE("---------------------------------\n");
            LOG_MONITOR_STATE("Delete from monitor list completed\n");
            LOG_MONITOR_STATE("---------------------------------\n");
            free(delete->payload);
            delete->payload = NULL;
            free(delete);
            delete = NULL;
            return CMS_SUCCESS;  
        };
        tmp = tmp->next;
    }

    LOG_MONITOR_STATE("---------------------------------\n");
    LOG_MONITOR_STATE("Client name not founded in monitor list \n");
    LOG_MONITOR_STATE("---------------------------------\n");
    return CMS_ERROR;     
}

cms_payload_t* get_payload_monitor_list(cms_monitor_info_t *head_monitor_list, char *client_name){
    cms_monitor_info_t * tmp = head_monitor_list;
    cms_payload_t * ret_payload = NULL;
    while (tmp != NULL)
    {
        if(strcmp(tmp->payload->client_name, client_name) == 0){
            LOG_MONITOR_STATE("---------------------------------\n");
            LOG_MONITOR_STATE("Founded client name in monitor list\n");
            LOG_MONITOR_STATE("---------------------------------\n");           
            return tmp->payload;
        }
        tmp = tmp->next;
    }
    LOG_MONITOR_STATE("---------------------------------\n");
    LOG_MONITOR_STATE("Client name not founded in monitor list \n");
    LOG_MONITOR_STATE("---------------------------------\n");
    return NULL;
}

void free_monitor_list(cms_monitor_info_t **head_monitor_list){
    cms_monitor_info_t * tmp = *head_monitor_list;
    if(tmp == NULL){
        LOG_MONITOR_STATE("---------------------------------\n");
        LOG_MONITOR_STATE("Monitor list is NULL  \n");
        LOG_MONITOR_STATE("---------------------------------\n");
        return ;    
    }
    while (tmp != NULL)
    {
        cms_monitor_info_t *delete = tmp;
        tmp = tmp->next;
        free(delete->payload);
        delete->payload = NULL;
        free(delete);
        delete = NULL;
    }
    LOG_MONITOR_STATE("---------------------------------\n");
    LOG_MONITOR_STATE("Delete monitor list list succesed \n");
    LOG_MONITOR_STATE("---------------------------------\n");    
}

void print_monitor_list(cms_monitor_info_t *head_monitor_list){
    cms_monitor_info_t *tmp = head_monitor_list;
    if(tmp == NULL){
        LOG_MONITOR_STATE("List is NULL \n");
        return;
    }
    int index = 0;
    LOG_MONITOR_STATE("\n\n");
    LOG_MONITOR_STATE("Begin print list\n");
    while(tmp != NULL){
        LOG_MONITOR_STATE("---------------------------------\n");
        LOG_MONITOR_INT(index);
        LOG_MONITOR(tmp->payload->source);
        LOG_MONITOR(tmp->payload->topic);
        LOG_MONITOR(tmp->payload->data);
        LOG_MONITOR(tmp->payload->client_name);
        char sendcount[10];
        sprintf(sendcount,"%d",tmp->payload->send_count);
        LOG_MONITOR(sendcount);
        tmp = tmp->next;
        LOG_MONITOR_STATE("---------------------------------\n");
        index++;
    }
    LOG_MONITOR_STATE("End print list \n\n");
    return;
}