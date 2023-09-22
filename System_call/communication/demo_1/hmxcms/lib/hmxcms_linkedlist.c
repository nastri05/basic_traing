#include"hmxcms_linkedlist.h"


cms_data_t *create_data(char * client_name, char * topic){
    cms_data_t * data_client = (cms_data_t*) malloc(sizeof(cms_data_t));
    strcpy(data_client->client_name,client_name);
    strcpy(data_client->topic,topic);
    return data_client;
}


char *get_client_name(cms_data_t * data){
    return data->client_name;
}


char *get_topic(cms_data_t * data){
    return data->topic;
}



cms_client_t * create_client( cms_data_t * data){
    cms_client_t * new_client = ( cms_client_t*) malloc(sizeof( cms_client_t));
    new_client->data = data;
    new_client->next = NULL;
    return new_client;
}


int add_client( cms_client_t ** head_client , cms_data_t * data){
    cms_client_t * new_client = create_client(data);
    if(new_client == NULL){
        // CMS_DEBUG_LOG("---------------------------------\n");
        CMS_DEBUG_LOG("creater List client fail \n");
        // CMS_DEBUG_LOG("---------------------------------\n");
        return CMS_ERROR;
     }
    cms_client_t * tmp = *head_client;
    if(*head_client == NULL){
        *head_client = new_client;
        // CMS_DEBUG_LOG("---------------------------------\n");
        CMS_DEBUG_LOG("Add client success \n");
        // CMS_DEBUG_LOG("---------------------------------\n");
        return CMS_SUCCESS;
    }
    
    while(tmp->next != NULL ){
        tmp = tmp->next;
    }
    tmp->next= new_client;
    // CMS_DEBUG_LOG("---------------------------------\n");
    CMS_DEBUG_LOG("Add client %s topic %s to client list successfully \n", data->client_name, data->topic);
    // CMS_DEBUG_LOG("---------------------------------\n");
    return CMS_SUCCESS;
}


int delete_client( cms_client_t ** head_client , int index){
    cms_client_t * tmp = *head_client;
    int length_list = get_length_list(*head_client);
    if(tmp == NULL){
        // CMS_INFO_LOG("---------------------------------\n");
        CMS_INFO_LOG("Client list is NULL \n");
        // CMS_INFO_LOG("---------------------------------\n");
        return CMS_ERROR;
    }
    // header of list
    if(index == 0){
        *head_client = (*head_client)->next;
        CMS_INFO_LOG("---------------------------------\n");
        CMS_INFO_LOG("Delete client %s topic %s from client list complete \n", tmp->data->client_name, tmp->data->topic);
        CMS_INFO_LOG("---------------------------------\n");
        free(tmp->data);
        free(tmp);
        return CMS_SUCCESS;
    } else if(index >= length_list){
        // CMS_ERROR_LOG("---------------------------------\n");
        CMS_ERROR_LOG("Index out length list\n");
        // CMS_ERROR_LOG("---------------------------------\n");
        return CMS_ERROR;
    } 
    int i ;
    for(i = 0 ; i < index-1 ; i ++){
        tmp = tmp->next;
    }
    cms_client_t * delete = tmp->next;
    if(index == length_list -1){ // tail of list
        tmp->next = NULL;
    }else{
        tmp->next = delete->next;
    }
    // LOG_STATE("---------------------------------\n");
    CMS_INFO_LOG("Delete client %s topic %s from client list complete \n", tmp->data->client_name, tmp->data->topic);
    // LOG_STATE("---------------------------------\n");
    free(delete->data);
    free(delete);

    return CMS_SUCCESS;
}


void change_data(cms_data_t * destination, cms_data_t * source){
    strcpy(destination->client_name,source->client_name);
    strcpy(destination->topic,source->topic);
}

void printf_data(cms_data_t * data){
    if(data == NULL){
        CMS_INFO_LOG("---------------------------------\n");
        CMS_INFO_LOG("Data is NULL \n");
        CMS_INFO_LOG("---------------------------------\n");
        return;
    }
    CMS_INFO_LOG("Print value in data \n");
    CMS_INFO_LOG("---------------------------------\n");
    CMS_INFO_LOG("get client name: %s\n",get_client_name(data));
    CMS_INFO_LOG("get topic : %s \n",get_topic(data));
    CMS_INFO_LOG("---------------------------------\n");
}
int change_data_client( cms_client_t ** head_client, int index, cms_data_t * data){
    cms_client_t * tmp = *head_client;
    if(tmp == NULL){
        CMS_DEBUG_LOG("Client list is NULL \n");
        free(data);
        return CMS_ERROR;
    }
    int i ;
    for(i = 0 ; i < index ; i ++){
        if(tmp->next != NULL)
        {
            tmp = tmp->next;
        }else{
            // CMS_DEBUG_LOG("---------------------------------\n");
            CMS_DEBUG_LOG("Index out length list \n");
            // CMS_DEBUG_LOG("---------------------------------\n");
            free(data);
            return CMS_ERROR;
        }
    }
    change_data(tmp->data,data);
    // CMS_DEBUG_LOG("---------------------------------\n");
    CMS_DEBUG_LOG("Change data in client complete \n");
    // CMS_DEBUG_LOG("---------------------------------\n");
    free(data);
    return CMS_SUCCESS;
}


int get_index_by_topic(cms_client_t *head_client, char *topic, int *result){
    int size_arr = 0;
    cms_client_t * tmp = head_client;
    int index = 0;
    while(tmp != NULL){
        // temporarily treat the value of the receive type as 1
        if(strcmp(get_topic(tmp->data),topic)==0)
        {
            result[size_arr] = index;
            size_arr++;
        }
        tmp = tmp->next;
        index++;
    }
    if(size_arr == 0) {
        // CMS_DEBUG_LOG("---------------------------------\n");
        CMS_DEBUG_LOG("Not found topic in list \n");
        // CMS_DEBUG_LOG("---------------------------------\n");
        return CMS_ERROR;
    }
    return size_arr;
}

int get_length_list(cms_client_t *head_client){
    int length = 0;
    cms_client_t * tmp = head_client;
    while (tmp!=NULL)
    {
        length++;
        tmp = tmp->next;
    }
    
    return length;
}

int get_index_by_name(cms_client_t *head_client, char *client_name , int * result){
    cms_client_t * tmp = head_client;
    int index = 0;
    int size_arr = 0;
    while (tmp != NULL)
    {
        if(strcmp(get_client_name(tmp->data),client_name)==0){
            result[size_arr] = index;
            size_arr++;
        }
        tmp = tmp->next;
        index ++;
    }
    if(size_arr == 0 ){
        // CMS_DEBUG_LOG("---------------------------------\n");
        CMS_DEBUG_LOG("Not found client name in list \n");
        // CMS_DEBUG_LOG("---------------------------------\n");
        return CMS_ERROR;
    }
    return size_arr;
}

int get_index_by_data(cms_client_t *head_client, cms_data_t *data) 
{
    cms_client_t *tmp = head_client;
    int index = 0;
    while (tmp != NULL)
    {
        if (strcmp(get_client_name(data), get_client_name(tmp->data))== 0 &&
            strcmp(get_topic(data), get_topic(tmp->data)) == 0 )
        {
            // CMS_DEBUG_LOG("---------------------------------\n");
            CMS_DEBUG_LOG("found data in list \n");
            // CMS_DEBUG_LOG("---------------------------------\n");
            return index;
        }
        tmp = tmp->next;
        index ++;
    }
    // CMS_DEBUG_LOG("---------------------------------\n");
    CMS_DEBUG_LOG("Not found data in list 'client %s topic %s' in client list \n", data->client_name, data->topic);
    // CMS_DEBUG_LOG("---------------------------------\n");
    return CMS_ERROR;
}


cms_data_t *get_data_by_name(cms_client_t *head_client, char * client_name){
    cms_client_t * tmp = head_client;
    while (tmp != NULL)
    {
        if(strcmp(get_client_name(tmp->data),client_name)==0){
            // CMS_DEBUG_LOG("---------------------------------\n");
            CMS_DEBUG_LOG("found client name completed \n");
            // CMS_DEBUG_LOG("---------------------------------\n");            
            return tmp->data;
        }
        tmp = tmp->next;
    }
    // CMS_DEBUG_LOG("---------------------------------\n");
    CMS_DEBUG_LOG("Not found client %s in list \n", client_name);
    // CMS_DEBUG_LOG("---------------------------------\n");    
    return NULL;
}

cms_data_t *get_data_by_index(cms_client_t *head_client, int index){
    cms_client_t * tmp = head_client;
    if(tmp == NULL){
            // CMS_DEBUG_LOG("---------------------------------\n");
            CMS_DEBUG_LOG("List is NULL \n");
            // CMS_DEBUG_LOG("---------------------------------\n");
            return NULL;
    }
    int i;
    for(i = 0 ; i < index ; i ++){
        if(tmp->next != NULL)
        {
            tmp = tmp->next;
        }else{
            // CMS_DEBUG_LOG("---------------------------------\n");
            CMS_DEBUG_LOG("Index out length list \n");
            // CMS_DEBUG_LOG("---------------------------------\n");
            return NULL;
        }
    }
    return tmp->data;
}

void free_list(cms_client_t **head_client){
    cms_client_t * tmp = *head_client;
    if(tmp == NULL){
        // CMS_DEBUG_LOG("---------------------------------\n");
        CMS_DEBUG_LOG("List is NULL  \n");
        // CMS_DEBUG_LOG("---------------------------------\n");
        return ;    
    }
    while (tmp != NULL)
    {
        cms_client_t *delete = tmp;
        tmp = tmp->next;
        free(delete->data);
        free(delete);
    }
    // CMS_DEBUG_LOG("---------------------------------\n");
    CMS_DEBUG_LOG("Delete list succesed \n");
    // CMS_DEBUG_LOG("---------------------------------\n");
}

void print_list( cms_client_t *head_client){
    cms_client_t * tmp = head_client;
    if(tmp == NULL){
        CMS_INFO_LOG("List is NULL \n");
        return;
    }
    int index = 0;
    CMS_INFO_LOG("\n\nBegin print list\n");
    while(tmp != NULL){
        CMS_INFO_LOG("---------------------------------\n");
        CMS_INFO_LOG("Index: %d\n", index);
        CMS_INFO_LOG("Client name: %s\n", get_client_name(tmp->data));
        CMS_INFO_LOG("Topic: %s\n", get_topic(tmp->data));
        tmp = tmp->next;
        CMS_INFO_LOG("---------------------------------\n");
        index++;
    }
    CMS_INFO_LOG("End print list \n\n");
    return;
}

int count_topic( cms_client_t *head_client){
    
    int length_list = get_length_list(head_client); 
    if(length_list == 0){
        return length_list;
    }
    int i;
    cms_client_t * tmp = head_client;
    char topic[length_list][MAX_NAME_LENGTH];
    memset(topic, '\0', sizeof(topic));
    int count_topic= 0;
    for(i = 0 ; i <length_list; i++){
        int j;
        for(j = 0; j <= count_topic;j++){
            if(strcmp(topic[j],tmp->data->topic)==0){
                break;
            }
            if(j == count_topic){
                strcpy(topic[count_topic],tmp->data->topic);
                count_topic++;
                break;
            }
        }
        tmp =  tmp->next;
    }
    return count_topic;
}


