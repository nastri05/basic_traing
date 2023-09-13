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


int delete_client( cms_client_t ** head_client , int index){
    cms_client_t * tmp = *head_client;
    int length_list = get_length_list(*head_client);
    if(tmp == NULL){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("List is NULL \n");
        LOG_STATE("---------------------------------\n");
        return CMS_ERROR;
    }
    // header of list
    if(index == 0){
        *head_client = (*head_client)->next;
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Delete complete \n");
        printf_data(tmp->data);
        LOG_STATE("---------------------------------\n");
        free(tmp->data);
        free(tmp);
        return CMS_SUCCESS;
    } else if(index >= length_list){
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Index out length list\n");
        LOG_STATE("---------------------------------\n");
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
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Delete complete \n");
    LOG_INT(index);
    printf_data(delete->data);
    LOG_STATE("---------------------------------\n");
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
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Data is NULL \n");
        LOG_STATE("---------------------------------\n");
        return;
    }
    LOG_STATE("Print value in data \n");
    LOG_STATE("---------------------------------\n");
    LOG(get_client_name(data));
    LOG(get_topic(data));
    LOG_STATE("---------------------------------\n");
}
int change_data_client( cms_client_t ** head_client, int index, cms_data_t * data){
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
    change_data(tmp->data,data);
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
        if(strcmp(get_topic(tmp->data),topic)==0)
        {
            result[size_arr] = index;
            size_arr++;
        }
        tmp = tmp->next;
        index++;
    }
    if(size_arr == 0) {
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Not found topic in list \n");
        LOG_STATE("---------------------------------\n");
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
        LOG_STATE("---------------------------------\n");
        LOG_STATE("Not found client name in list \n");
        LOG_STATE("---------------------------------\n");
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
            LOG_STATE("---------------------------------\n");
            LOG_STATE("found data in list \n");
            LOG_STATE("---------------------------------\n");
            return index;
        }
        tmp = tmp->next;
        index ++;
    }
    LOG_STATE("---------------------------------\n");
    LOG_STATE("Not found data in list \n");
    LOG_STATE("---------------------------------\n");
    return CMS_ERROR;
}


cms_data_t *get_data_by_name(cms_client_t *head_client, char * client_name){
    cms_client_t * tmp = head_client;
    while (tmp != NULL)
    {
        if(strcmp(get_client_name(tmp->data),client_name)==0){
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

cms_data_t *get_data_by_index(cms_client_t *head_client, int index){
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
        LOG_STATE("List is NULL  \n");
        LOG_STATE("---------------------------------\n");
        return ;    
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
        LOG_STATE("List is NULL \n");
        return;
    }
    int index = 0;
    LOG_STATE("\n\n");
    LOG_STATE("Begin print list\n");
    while(tmp != NULL){
        LOG_STATE("---------------------------------\n");
        LOG_INT(index);
        LOG(get_client_name(tmp->data));
        LOG(get_topic(tmp->data));
        tmp = tmp->next;
        LOG_STATE("---------------------------------\n");
        index++;
    }
    LOG_STATE("End print list \n\n");
    return;
}

int count_topic( cms_client_t *head_client){
    
    int length_list = get_length_list(head_client); 
    LOG_INT(length_list);
    if(length_list == 0){
        return length_list;
    }
    int i;
    cms_client_t * tmp = head_client;
    char topic[length_list][MAX_NAME_LENGTH];
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


// int main(){
//     cms_client_t *list_client = NULL;
//     print_list(list_client);
//     free_list(&list_client);
    
//     cms_data_t *data = create_data("hoangtuan","wan");
//     int result = add_client(&list_client,data);
//     // print_list(list_client);
//     // LOG_STATE("Value of result : \n");
//     // LOG_INT(result);
//     data = create_data("pog3","wan");
//     result = add_client(&list_client,data);
    
//     data = create_data("dante","wan");
//     result = add_client(&list_client,data);
    
//     data = create_data("hxtuan","wan");
//     result = add_client(&list_client,data);
    
//     data = create_data("dante_hoang","wan");
//     result = add_client(&list_client,data);
    
//     data = create_data("ricon","wan");
//     result = add_client(&list_client,data);

//     data = create_data("Nhabao(nguoiY)","wan");
//     result = add_client(&list_client,data);

//     data = create_data("sol7","wan");
//     result = add_client(&list_client,data);

//     data = create_data("sol8","wan");
//     result = add_client(&list_client,data);

//     print_list(list_client);
//     int length = get_length_list(list_client);
//     LOG_INT(get_length_list(list_client));
//     // int i;
//     LOG_INT(count_topic(list_client));

//     //  print_list(list_client);
//     // data = create_data("sol7","wan");
//     // result = add_client(&list_client,data);
//     // result = get_index_by_name(list_client, "pog3");
    
//     // LOG_STATE("Value of result get_index_by_name : \n");
//     // LOG_INT(result);

//     // print_list(list_client);
//     // cms_data_t *cms_data_tmp = create_data("mck","rapital");
//     // change_data_client(&list_client,result,cms_data_tmp);
//     //print_list(list_client);
//     // result = get_length_list(list_client);
//     // LOG_STATE("Length of list\n");
//     // LOG_INT(result);
//     // int result_arr[result];
//     // data = create_data("hoangtuan","wan");
//     // result = get_index_by_data(list_client,data);
//     // LOG_STATE("Length of result_arr\n");
//     // LOG_INT(result);
//     // int i ;
//     // for(i = 0; i < length;i++){
//     // data = get_data_by_index(list_client,i);
//     // printf_data(data);
//     // }
//     // int i;
//     // for(i = 0; i < result ;i++){
//     //     LOG_INT(result_arr[i]);
//     //     data = get_data_by_index(list_client,result_arr[i]);
//     //     printf_data(data);
//     // }
//     // print_list(list_client);
//     // data = get_data_by_name(list_client,"pog3");
//     // //data = create_data("Nhabao(nguoiY)","truong_anhNgok_mq",3,"wifi");
//     //printf_data(data);
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
