/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/data.h"
#include "../include/entry.h"
#include <stddef.h> //NULLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int data_to_buffer(struct data_t *data, char **data_buf){
    if(data == NULL || data_buf == NULL){
        printf("[WARN] invalid arguments\n");
        return -1;
    }

    int size = data->datasize; //data's length
    char *buff = malloc(sizeof(int) + size);
    memcpy(buff, &size, sizeof(int)); //serialize to address
    memcpy(buff+sizeof(int), data->data, size);
    *data_buf = buff;
    return sizeof(int) + size;
}

struct data_t *buffer_to_data(char *data_buf, int data_buf_size){
    if(data_buf == NULL || data_buf_size < 1){
        printf("[ERROR] data_buf received is NULL or size is invalid, can't proceed\n");
        return NULL;
    }
    else{
        struct data_t *data;
        int dataSize = data_buf_size-sizeof(int);
        void *aux = malloc(dataSize);
        memcpy(aux, data_buf+sizeof(int), dataSize);
        data = data_create2(dataSize, aux);
        return data;
    }
    
}

int entry_to_buffer(struct entry_t *data, char **entry_buf){
    if(data == NULL || entry_buf == NULL){
        printf("[WARN] Invalid NULL argument!\n");
        return -1;
    }
    
    int *keySize = malloc(sizeof(int));
    *keySize = sizeof(char)*strlen(data->key)+1;
    //data
    char **dataBuff = malloc(sizeof(int)+data->value->datasize);
    int dataBuffSize = data_to_buffer(data->value, dataBuff);

    char *entryBuffer = malloc(sizeof(int)+*keySize+dataBuffSize);

    memcpy(entryBuffer, keySize, sizeof(int)); //copy the size of key into entryBuffer
    memcpy(entryBuffer+sizeof(int), data->key, *keySize); // copy the content of key to entryBuffer
    memcpy(entryBuffer+sizeof(int)+*keySize, *dataBuff, sizeof(int)); //copy the size of data into entryBuffer
    memcpy(entryBuffer+sizeof(int)+*keySize+sizeof(int), *dataBuff+sizeof(int), dataBuffSize-sizeof(int));

    *entry_buf = entryBuffer;
    return sizeof(int)+*keySize+dataBuffSize;
}

struct entry_t *buffer_to_entry(char *entry_buf, int entry_buf_size){
    if(entry_buf == NULL || entry_buf_size < 1){
        printf("[ERROR] entry_buf received in NULL, can't proceed\n");
        return NULL;
    }
    else{
        char *key;
        struct data_t *value;
        struct entry_t *entry;

        int *keySize = malloc(sizeof(int)); 
        memcpy(keySize, entry_buf, sizeof(int));

        key = malloc(*keySize);

        memcpy(key, entry_buf+sizeof(int), *keySize); 

    
        int *dataSize = malloc(sizeof(int));
        memcpy(dataSize, entry_buf+sizeof(int)+*keySize, sizeof(int));

        void *data = malloc(*dataSize);
        memcpy(data, entry_buf+sizeof(int)+*keySize+sizeof(int), *dataSize);
        value = data_create2(*dataSize, data);
        
        entry = entry_create(key, value);
        return entry;
    }
}