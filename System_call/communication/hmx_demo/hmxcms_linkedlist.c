#include"inc/hmxcms_linkedlist.h"


data_t *create_data(char * client_name, char * mq_client, int type, char * topic){
    data_t * data_client = (data_t*) malloc(sizeof(data_t));
    strcpy(data_client->client_name,client_name);
    strcpy(data_client->mq_name,mq_client);
    data_client->type = type;
    strcpy(data_client->topic,topic);
    return data_client;
}


char *get_client_name(data_t * data){
    return data->client_name;
}


char *get_mq_name(data_t * data){
    return data->mq_name;
}


int  get_type(data_t * data){
    return data->type;
}


char *get_topic(data_t * data){
    return data->topic;
}



cms_client_t * create_client( data_t * data){
    cms_client_t * new_client = ( cms_client_t*) malloc(sizeof( cms_client_t));
    new_client->data = data;
    new_client->next = NULL;
    return new_client;
}


int add_client( cms_client_t ** head_client , data_t * data){
    cms_client_t * new_client = create_client(data);
    if(new_client == NULL){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("creater List client fail \n");
        LOG_STATE("---------------------------------\n");
        return CMS_ERROR;
     }
    cms_client_t * tmp = *head_client;
    if(*head_client == NULL){
        *head_client = new_client;
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Add client success \n");
        LOG_STATE("---------------------------------\n");
        return CMS_SUCCESS;
    }
    
    while(tmp->next != NULL ){
        tmp = tmp->next;
    }
    tmp->next= new_client;
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Add client success \n");
    LOG_STATE("---------------------------------\n");
    return CMS_SUCCESS;
}


int delete_client( cms_client_t ** head_clien , int index){
    cms_client_t * tmp = *head_clien;
    
    if(tmp == NULL){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("List is NULL \n");
        LOG_STATE("---------------------------------\n");
        return CMS_ERROR;
    }
    // header of list
    if(index == 0){
        *head_clien = (*head_clien)->next;
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Delete complete \n");
        printf_data(tmp->data);
        LOG_STATE("---------------------------------\n");
        free(tmp->data);
        free(tmp);
        return CMS_SUCCESS;
    }
    int i ;
    for(i = 0 ; i < index-1 ; i ++){
        if(tmp->next != NULL)
        {
            tmp = tmp->next;
            
        }else{
            LOG_STATE("---------------------------------\n");
            LOG_STATE("Index out length list\n");
            LOG_STATE("---------------------------------\n");
            return CMS_ERROR;
        }
    }
    cms_client_t * delete = NULL;
    if(tmp->next == NULL){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Index out length list\n");
        LOG_STATE("---------------------------------\n");
        return CMS_ERROR;
    }else{
        delete = tmp->next;
        tmp->next = delete->next;
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Delete complete \n");
        printf_data(delete->data);
        LOG_STATE("---------------------------------\n");
        free(delete->data);
        free(delete);
    }
    return CMS_SUCCESS;
}


void set_data(data_t * destination, data_t * source){
    strcpy(destination->client_name,source->client_name);
    strcpy(destination->client_name,source->client_name);
    strcpy(destination->client_name,source->client_name);
    destination->type = source->type;
}

void printf_data(data_t * data){
    if(data == NULL){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Data is NULL \n");
        LOG_STATE("---------------------------------\n");
        return;
    }
    LOG_STATE("Print value in data \n");
    LOG_STATE("---------------------------------\n");
    LOG_INT(get_type(data));
    LOG(get_client_name(data));
    LOG(get_mq_name(data));
    LOG(get_topic(data));
    LOG_STATE("---------------------------------\n");
}
int change_data_client( cms_client_t ** head_client, int index, data_t * data){
    cms_client_t * tmp = *head_client;
    if(tmp == NULL){
        LOG_STATE("List is NULL \n");
        free(data);
        return CMS_ERROR;
    }
    int i ;
    for(i = 0 ; i < index ; i ++){
        if(tmp->next != NULL)
        {
            tmp = tmp->next;
        }else{
            LOG_STATE("---------------------------------\n");
            LOG_STATE("Index out length list \n");
            LOG_STATE("---------------------------------\n");
            free(data);
            return CMS_ERROR;
        }
    }
    set_data(tmp->data,data);
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Change data in client complete \n");
    LOG_STATE("---------------------------------\n");
    free(data);
    return CMS_SUCCESS;

}


int get_index_by_topic(cms_client_t *head_client, char *topic, int *result){
    int size_arr = 0;
    cms_client_t * tmp = head_client;
    int index = 0;
    while(tmp != NULL){
        // temporarily treat the value of the receive type as 1
        if(strncmp(get_topic(tmp->data),topic,strlen(topic))==0 &&
            (get_type(tmp->data) == RECEIVE_REQUEST))
        {
            result[size_arr] = index;
            size_arr++;
        }
        tmp = tmp->next;
        index++;
    }
    if(size_arr==0){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Not found topic in list \n");
        LOG_STATE("---------------------------------\n");
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

int get_index_by_name(cms_client_t *head_client, char *client_name){
    cms_client_t * tmp = head_client;
    int index = 0;
    while (tmp != NULL)
    {
        if(strncmp(get_client_name(tmp->data),client_name,strlen(client_name))==0){
            LOG_STATE("---------------------------------\n");
            LOG_STATE("found client name \n");
            LOG_STATE("---------------------------------\n");
            return index;
        }
        tmp = tmp->next;
        index ++;
    }
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Not found client name in list \n");
    LOG_STATE("---------------------------------\n");
    return CMS_ERROR;
    
}


data_t *get_data_by_name(cms_client_t *head_client, char * client_name){
    cms_client_t * tmp = head_client;
    while (tmp != NULL)
    {
        if(strncmp(get_client_name(tmp->data),client_name,strlen(client_name))==0){
            LOG_STATE("---------------------------------\n");
            LOG_STATE("found client name completed \n");
            LOG_STATE("---------------------------------\n");            
            return tmp->data;
        }
        tmp = tmp->next;
    }
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Not found client name in list \n");
    LOG_STATE("---------------------------------\n");    
    return NULL;
}

data_t *get_data_by_index(cms_client_t *head_client, int index){
    cms_client_t * tmp = head_client;
    if(tmp == NULL){
            LOG_STATE("---------------------------------\n");
            LOG_STATE("List is NULL \n");
            LOG_STATE("---------------------------------\n");
            return NULL;
    }
    int i;
    for(i = 0 ; i < index ; i ++){
        if(tmp->next != NULL)
        {
            tmp = tmp->next;
        }else{
            LOG_STATE("---------------------------------\n");
            LOG_STATE("Index out length list \n");
            LOG_STATE("---------------------------------\n");
            return NULL;
        }
    }
    return tmp->data;
}

void free_list(cms_client_t **head_client){
    cms_client_t * tmp = *head_client;
    if(tmp == NULL){
    LOG_STATE("---------------------------------\n");
    LOG_STATE("List is empty \n");
    LOG_STATE("---------------------------------\n");        
    }
    while (tmp != NULL)
    {
        cms_client_t *delete = tmp;
        tmp = tmp->next;
        free(delete->data);
        free(delete);
    }
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Delete list succesed \n");
    LOG_STATE("---------------------------------\n");
}

void print_list( cms_client_t *head_client){
    cms_client_t * tmp = head_client;
    if(tmp == NULL){
        LOG_STATE("List empty \n");
        return;
    }
    int index = 0;
    LOG_STATE("\n\n\n\n");
    LOG_STATE("Begin print list\n");
    while(tmp != NULL){
        LOG_STATE("---------------------------------\n");
        LOG_INT(index);
        LOG_INT(get_type(tmp->data));
        LOG(get_client_name(tmp->data));
        LOG(get_mq_name(tmp->data));
        LOG(get_topic(tmp->data));
        tmp = tmp->next;
        LOG_STATE("---------------------------------\n");
        index++;
    }
    LOG_STATE("End print list \n \n \n\n\n\n");
    return;
}

// int main(){
//     cms_client_t *list_client = NULL;
//     print_list(list_client);
//     free_list(&list_client);
//     data_t *data = create_data("hoangtuan","hoangtuan_mq",1,"wan");
//     int result = add_client(&list_client,data);
//     // print_list(list_client);
//     // LOG_STATE("Value of result : \n");
//     // LOG_INT(result);
//     data = create_data("pog3","pog3_mq",3,"wifi");
//     result = add_client(&list_client,data);
//     data = create_data("dante","dante_mq",2,"wan");
//     result = add_client(&list_client,data);
//     data = create_data("hxtuan","htx_mq",3,"wan");
//     result = add_client(&list_client,data);
//     // print_list(list_client);
//     // LOG_STATE("Value of result : \n");
//     // LOG_INT(result);
//     data = create_data("dante_hoang","dante_hoang_mq",2,"wifi");
//     result = add_client(&list_client,data);
    
//     data = create_data("ricon","ricon_mq",1,"wifi");
//     result = add_client(&list_client,data);

//     data = create_data("Nhabao(nguoiY)","truong_anhNgok_mq",3,"wifi");
//     result = add_client(&list_client,data);

//     data = create_data("sol7","sol7_mq",3,"wan");
//     result = add_client(&list_client,data);

//     result = delete_client(&list_client,8);
    
//     result = get_index_by_name(list_client, "dante_hoang");
    
//     LOG_STATE("Value of result get_index_by_name : \n");
//     LOG_INT(result);

//     print_list(list_client);

//     result = get_length_list(list_client);
//     LOG_STATE("Length of list\n");
//     LOG_INT(result);
//     int result_arr[result];
//     result = get_index_by_topic(list_client,"wan", result_arr);
//     LOG_STATE("Length of result_arr\n");
//     LOG_INT(result);
//     int i;
//     for(i = 0; i < result ;i++){
//         LOG_INT(result_arr[i]);
//         data = get_data_by_index(list_client,result_arr[i]);
//         printf_data(data);
//     }
//     print_list(list_client);
//     data = get_data_by_name(list_client,"sol7");
//     //data = create_data("Nhabao(nguoiY)","truong_anhNgok_mq",3,"wifi");
//     printf_data(data);
//     free_list(&list_client);
//     /* test delete_client*/
//     // result = delete_client(&list_client,1);
//     // print_list(list_client);
//     // LOG_STATE("Value of result : \n");
//     // LOG_INT(result);
//     /* test change data client*/
//     // while(1){
//     /* test change_data_client*/
//     /*
//         data = create_data("Nhabao(nguoiY)","truong_anhNgok_mq",3,"wifi");
//         result = change_data_client(&list_client,3,data);
//         LOG_STATE("Value of result : \n");
//         LOG_INT(result);
//         print_list(list_client);
//     // }
//     */
//     return CMS_SUCCESS;
// }
