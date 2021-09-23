#include <stdlib.h>
#include "../headers/data.h"

struct data_t *data_create(int size){
    struct data_t *new_data;
    new_data = malloc(size * sizeof(struct data_t));
    return new_data;
}

struct data_t *data_create2(int size, void *data); 

void data_destroy(struct data_t *data);

struct data_t *data_dup(struct data_t *data);

void data_replace(struct data_t *data, int new_size, void *new_data);