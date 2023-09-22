#include<stdlib.h>
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include "hmxcms_lib.h"
#include"hmxcms_log.h"
// #define MAX_NAME_LENGTH 32

typedef struct cms_data_t{
    char client_name[MAX_NAME_LENGTH];
    char topic[MAX_NAME_LENGTH];
} cms_data_t;


typedef struct cms_client_t{
    cms_data_t * data;
    struct cms_client_t *next;
} cms_client_t;

/*
@brief create pointer cms_data_t with informations in it
@param client_name is name of client that user want to name
@param topic is name topic that client want send or receive from it
@return the pointer cms_data_t contains the information that was passed into it
*/
cms_data_t *create_data(char * client_name, char * topic);

/*
@brief get client name form data
@param data contains information
@return client name
*/
char *get_client_name(cms_data_t * data);

/*
@brief get mq client form data
@param data contains information
@return mq name
*/

char *get_topic(cms_data_t * data);

/*
@brief change values in data
@param destination is data needs to be changed
@param source is data contains information want change into destination
@return void
*/
void change_data(cms_data_t * destination, cms_data_t * source);

/*
@brief print informations in data
@param data contains information
@return void
*/
void printf_data(cms_data_t * data);

/*
@brief create client to create linked list
@param data contains information for client
@return the pointer cms_client_t 
*/
cms_client_t *create_client(cms_data_t * data);

/*
@brief add client into linked list
@param head_client is header of linked list
@param data is contains information for add client
@return result of adding client in list
*/
int add_client( cms_client_t ** head_client , cms_data_t * data);

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
int change_data_client( cms_client_t ** head_client, int index, cms_data_t * data);

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
int get_index_by_name(cms_client_t *head_client, char *client_name , int * result);

/*
@brief get index of data in linked list
@param head is header of linked list
@param data data
@return index of data
*/
int get_index_by_data(cms_client_t *head, cms_data_t *data);

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
@return cms_data_t contains client's information 
*/
cms_data_t *get_data_by_name(cms_client_t *head_client, char * client_name);

/*
@brief get data by client's location in list
@param head_client is header linked list
@param index is client's location
@return cms_data_t contains client's information
 */
cms_data_t *get_data_by_index(cms_client_t *head_client, int index);

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
/*
@brief count number topic in list
@param head_client is head of linked list client
*/
int count_topic( cms_client_t *head_client);
