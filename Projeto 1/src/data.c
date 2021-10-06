#include <stdlib.h>
#include "../headers/data.h"

struct data_t *data_create(int size){
    struct data_t *new_data;
    new_data = malloc(sizeof(struct data_t));
    new_data->data = malloc(size);
    new_data->datasize = size;
    return new_data;
}

// DUVIDA: Como criar um data sem reservar memoria?
struct data_t *data_create2(int size, void *data){
    struct data_t *new_data2;
    new_data2 = malloc(sizeof(struct data_t));
    //new_data2->data = malloc(sizeof(size));
    new_data2->data = data;
    new_data2->datasize = size;
    return new_data2;
}

void data_destroy(struct data_t *data){
    if(data != NULL){
        if(data->data != NULL){
            free(data->data);
        }
        free(data);
    }
    
    
}

struct data_t *data_dup(struct data_t *data){
    if(data == NULL || data->data == NULL){
        printf("[WARN] the data argument provided is NULL, cant proceed\n");
        return NULL;
    }
    struct data_t *new_data;
    new_data = data_create2(data->datasize, data->data);
    return new_data;
}

void data_replace(struct data_t *data, int new_size, void *new_data){
    free(data->data);
    data->data = new_data;
    data->datasize = new_size;
    // CASE NULL TO BE TREATED
}