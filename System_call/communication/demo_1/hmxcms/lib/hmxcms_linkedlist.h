#include<stdlib.h>
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include "hmxcms_lib.h"

#define CMS_SUCCESS     0
#define CMS_ERROR       -1

#define DEBUG_LIST

#ifdef DEBUG_LIST
#define STR(x)   #x
#define LOG(x) printf("[DEBUG] [%s]         %s = %s\n",__FUNCTION__,STR(x),x)
#define LOG_INT(x) printf("[DEBUG] [%s]         %s = %d\n",__FUNCTION__,STR(x),x)
#define LOG_STATE(x) printf("[DEBUG] [%s]   %s",__FUNCTION__, x)
#else
#define LOG(x)
#define LOG_INT(x)
#define LOG_STATE(x)
#endif

typedef struct data_t{
    char client_name[64];
    char mq_name[64];
    int type;
    char topic[64];
} data_t;


typedef struct cms_client_t{
    data_t * data;
    struct cms_client_t *next;
} cms_client_t;

/*
@brief create pointer data_t with informations in it
@param client_name is name of client that user want to name
@param mq_client is name of client's server that user want to name
@param type is the type in which the client wants to operate RECEIVE_REQUEST or SEND_REQUEST
@param topic is name topic that client want send or receive from it
@return the pointer data_t contains the information that was passed into it
*/
data_t *create_data(char * client_name, char * mq_client, int type, char * topic);

/*
@brief get client name form data
@param data contains information
@return client name
*/
char *get_client_name(data_t * data);

/*
@brief get mq client form data
@param data contains information
@return mq name
*/
char *get_mq_name(data_t * data);

/*
@brief get type form data
@param data contains information
@return type
*/
int  get_type(data_t * data);

/*
@brief get topic from data
@param data contains information
@return topic
*/
char *get_topic(data_t * data);

/*
@brief change values in data
@param destination is data needs to be changed
@param source is data contains information want change into destination
@return void
*/
void set_data(data_t * destination, data_t * source);

/*
@brief print informations in data
@param data contains information
@return void
*/
void printf_data(data_t * data);

/*
@brief create client to create linked list
@param data contains information for client
@return the pointer cms_client_t 
*/
cms_client_t *create_client(data_t * data);

/*
@brief add client into linked list
@param head_client is header of linked list
@param data is contains information for add client
@return result of adding client in list
*/
int add_client( cms_client_t ** head_client , data_t * data);

/*
@brief remove client at index location in list 
@param head_client is header of linked list
@param index is client's location need remove
@return result of delete client in list
*/
int delete_client( cms_client_t ** head_client, int index); 

/*
@brief change data informations of client
@param head_client is header of linked list
@param index is client's location need change
@param data contains information for change data client
@return result of change data of client
*/
int change_data_client( cms_client_t ** head_client, int index, data_t * data);

/*
@brief get length array and array contain location of client want reiceve information form topic
@param head_client is header of linked list
@param topic is name topic want to find
@param result is array contain location of found client 
@return length of array result
*/
int get_index_by_topic(cms_client_t *head_client, char *topic, int *result);

/*
@brief get client's location in linked list
@param head_client is header of linked list
@param client_name is client name need find
@return location of client
*/
int get_index_by_name(cms_client_t *head_client, char *client_name);

/*
@brief get index of data in linked list
@param head is header of linked list
@param data data
@return index of data
*/
int get_index_by_data(cms_client_t *head, data_t *data);

/*
@brief get length of linked list
@param head_client is header of linked list
@return length of linked list
*/
int get_length_list(cms_client_t *head_client);

/*
@brief get data by name in list
@param head_client is header linked list
@param client_name is client name need find
@return data_t contains client's information 
*/
data_t *get_data_by_name(cms_client_t *head_client, char * client_name);

/*
@brief get data by client's location in list
@param head_client is header linked list
@param index is client's location
@return data_t contains client's information
 */
data_t *get_data_by_index(cms_client_t *head_client, int index);

/*
@brief delete linked list
@param head_client is header linked list
@return void
*/
void free_list(cms_client_t **head_client); // pass

/*
@brief print list of clients
@param head_client is header linked list
@return void
*/
void print_list( cms_client_t *head_client); //pass