#include <stdlib.h>
#include "../headers/data.h"

struct data_t *data_create(int size){
    struct data_t *new_data;
    new_data = malloc(sizeof(struct data_t));
    new_data->data = malloc(sizeof(size));
    new_data->datasize = size;
    return new_data;
}

struct data_t *data_create2(int size, void *data){
    struct data_t *new_data2;
    new_data2 = malloc(sizeof(struct data_t));
    //note: nao era preciso alocar o size para o new_data2->data ?!
    //new_data2->data = malloc(sizeof(size));
    new_data2->data = data;
    new_data2->datasize = size;
    return new_data2;
}

void data_destroy(struct data_t *data){
    free(data->data);
    //free(data->datasize); //talvez n seja necessario libertar visto que n foi alocado
    free(data);
}

struct data_t *data_dup(struct data_t *data){
    struct data_t *new_data;
    new_data = malloc(sizeof(struct data_t));
    new_data->data = malloc(sizeof(data->data));
    new_data->data = data->data;
    return new_data;
}

void data_replace(struct data_t *data, int new_size, void *new_data){
    free(data->data);
    //free(data->datasize); //probably doesnt need it
    data->data = new_data;
    data->datasize = new_size;
}