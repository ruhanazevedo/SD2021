#include "../headers/data.h"
#include "../headers/entry.h"
#include <stddef.h> //NULLS

//TREAT NULL CASE
int data_to_buffer(struct data_t *data, char **data_buf){
    //OPTION 1
    /*int size = data->datasize; //data's length
    char *buff = malloc(sizeof(int) + size);
    memcpy(buff, &size, sizeof(int)); //serialize to address
    memcpy(buff+size, data->data, size);
    return 0;*/

    //OPTION 2
    int size = data->datasize;
    memcpy(data_buf, &size, sizeof(int)); //serialize to address
    memcpy(data_buf+size, data->data, size);
}

struct data_t *buffer_to_data(char *data_buf, int data_buf_size){
    if(data_buf == NULL){
        printf("[ERROR] data_buf received in NULL, can't proceed");
        return NULL;
    }
    else{
        struct data_t *data;
        void *aux = malloc(data_buf_size);
        memcpy(aux, &data_buf, data_buf_size);
        data = data_create2(data_buf_size, aux);
        return data;
    }
    
}

// TODO: TREAT NULL CASE
int entry_to_buffer(struct entry_t *data, char **entry_buf){
    int size = data->value->datasize;
    memcpy(entry_buf, &size, sizeof(int)); //serialize to address
    memcpy(entry_buf+size, data->value->data, size);
}

struct entry_t *buffer_to_entry(char *entry_buf, int entry_buf_size){
    if(entry_buf == NULL){
        printf("[ERROR] entry_buf received in NULL, can't proceed");
        return NULL;
    }
    else{
        struct entry_t *entry;
        void *aux = malloc(entry_buf_size);
        memcpy(aux, &entry_buf, entry_buf_size);
        entry = data_create2(entry_buf_size, aux);
        return entry;
    }
}