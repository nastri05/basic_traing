#include "hmxcms_monitor.h"

cms_payload_t* create_payload(char* source, char* destination, char* data, char *client_name) {
    cms_payload_t *payload_of_datagram = (cms_payload_t*) malloc(sizeof(cms_payload_t));
    strcpy(payload_of_datagram->source, source);
    strcpy(payload_of_datagram->destination, destination);
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
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Create list of clients for monitoring failed \n");
        LOG_STATE("---------------------------------\n");
        return CMS_ERROR;        
    }
    cms_monitor_info_t *tmp = *head_monitor_list;
    if(*head_monitor_list == NULL){
        *head_monitor_list = new_client_monitored;
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Add client to monitor list success \n");
        LOG_STATE("---------------------------------\n");
        return CMS_SUCCESS;
    }
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next= new_client_monitored;
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Add client to monitor list success \n");
    LOG_STATE("---------------------------------\n");
    return CMS_SUCCESS;    
}

int delete_from_monitor(cms_monitor_info_t **head_monitor_list, char* destination){
    cms_monitor_info_t * tmp = *head_monitor_list;
    cms_monitor_info_t * delete = NULL;

    if(tmp == NULL){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Monitor list is NULL \n");
        LOG_STATE("---------------------------------\n");
        return CMS_ERROR;        
    }

    if(strcmp(tmp->payload->destination,destination) == 0){
        *head_monitor_list= (*head_monitor_list)->next;
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Delete from monitor list completed\n");
        LOG_STATE("---------------------------------\n");
        free(tmp->payload);
        tmp->payload = NULL;
        free(tmp);
        tmp = NULL;
        return CMS_SUCCESS;        
    }

    while(tmp->next != NULL){
        if(strcmp(tmp->next->payload->destination, destination) == 0){

            cms_monitor_info_t* delete = tmp->next;
            tmp->next = delete->next;

            LOG_STATE("---------------------------------\n");
            LOG_STATE("Delete from monitor list completed\n");
            LOG_STATE("---------------------------------\n");
            free(delete->payload);
            delete->payload = NULL;
            free(delete);
            delete = NULL;
            return CMS_SUCCESS;  
        };
        tmp = tmp->next;
    }

    LOG_STATE("---------------------------------\n");
    LOG_STATE("Destination not founded in monitor list \n");
    LOG_STATE("---------------------------------\n");
    return CMS_ERROR;     
}

cms_payload_t* get_payload_monitor_list(cms_monitor_info_t **head_monitor_list, char *destination){
    cms_monitor_info_t * tmp = *head_monitor_list;
    cms_payload_t * ret_payload = NULL;
    if (tmp == NULL){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Monitor list is empty \n");
        LOG_STATE("---------------------------------\n");
        return NULL;
    }
    do{
        if(strcmp(tmp->payload->destination, destination) == 0){
            LOG_STATE("---------------------------------\n");
            LOG_STATE("Destination founded in monitor list\n");
            LOG_STATE("---------------------------------\n");
            LOG(tmp->payload->destination);
            ret_payload = (cms_payload_t*)tmp->payload;
            return ret_payload;
        }
        tmp = tmp->next;
    }while( tmp->next != NULL);
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Destination not founded in monitor list \n");
    LOG_STATE("---------------------------------\n");
    return NULL;
}

void free_monitor_list(cms_monitor_info_t **head_monitor_list){
    cms_monitor_info_t * tmp = *head_monitor_list;
    if(tmp == NULL){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Monitor list is NULL  \n");
        LOG_STATE("---------------------------------\n");
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
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Delete monitor list list succesed \n");
    LOG_STATE("---------------------------------\n");    
}

void print_monitor_list(cms_monitor_info_t *head_monitor_list){
    cms_monitor_info_t *tmp = head_monitor_list;
    if(tmp == NULL){
        LOG_STATE("List is NULL \n");
        return;
    }
    int index = 0;
    LOG_STATE("\n\n");
    LOG_STATE("Begin print list\n");
    while(tmp != NULL){
        LOG_STATE("---------------------------------\n");
        LOG_INT(index);
        LOG(tmp->payload->source);
        LOG(tmp->payload->destination);
        LOG(tmp->payload->data);
        LOG(tmp->payload->client_name);
        char sendcount[10];
        sprintf(sendcount,"%d",tmp->payload->send_count);
        LOG(sendcount);
        tmp = tmp->next;
        LOG_STATE("---------------------------------\n");
        index++;
    }
    LOG_STATE("End print list \n\n");
    return;
}