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
    *data_buf = buff;
    memcpy(buff, &size, sizeof(int)); //serialize to address
    memcpy(buff+sizeof(int), data->data, size);
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
        printf("data = %s\n",data->data);
        return data;
    }
    
}

int entry_to_buffer(struct entry_t *data, char **entry_buf){
    if(data == NULL || entry_buf == NULL){
        printf("[WARN] Invalid NULL argument!\n");
        return -1;
    }

    char **dataBuff = malloc(sizeof(int) + data->value->datasize);
    int dataBuffSize = data_to_buffer(data->value, dataBuff);
    
    int keyBuffSize = sizeof(char)*strlen(data->key)+1;
    char *keyBuff = malloc(sizeof(int) + keyBuffSize);
    memcpy(keyBuff, &keyBuffSize, sizeof(int)); 
    memcpy(keyBuff+sizeof(int), data->key, keyBuffSize);

    char *entryBuff = malloc(dataBuffSize + (sizeof(int)+keyBuffSize));
    memcpy(entryBuff, keyBuff, (sizeof(int)+keyBuffSize));
    memcpy(entryBuff+(sizeof(int)+keyBuffSize), dataBuff, dataBuffSize);
    
    *entry_buf = entryBuff;
    return dataBuffSize + (sizeof(int)+keyBuffSize);
}

struct entry_t *buffer_to_entry(char *entry_buf, int entry_buf_size){
    if(entry_buf == NULL || entry_buf_size < 1){
        printf("[ERROR] entry_buf received in NULL, can't proceed\n");
        return NULL;
    }
    else{
        char *key = malloc(sizeof(char));
        struct data_t *data;
        struct entry_t *entry;

        int *keySize = malloc(sizeof(int)); 
        memcpy(keySize, entry_buf, sizeof(int));

        memcpy(key, entry_buf+sizeof(int), *keySize); //TODO: check if *keySize is correct innstead of keySize

    
        int dataSize = entry_buf_size-sizeof(int)-*keySize;

        char *splitedBuffer = malloc(dataSize);
        memcpy(splitedBuffer, entry_buf+sizeof(int)+*keySize, dataSize);
        data = buffer_to_data(splitedBuffer, dataSize);
        //data = buffer_to_data(entry_buf+sizeof(int)+*keySize, entry_buf_size-sizeof(int)-*keySize);

        free(keySize);
        entry = entry_create(key, data);
        //free(key); if this free happen the entry will have key = ""
        return entry;
    }
}