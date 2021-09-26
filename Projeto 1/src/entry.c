//#include "../headers/data.h"
#include "../headers/entry.h"

struct entry_t *entry_create(char *key, struct data_t *data){
    struct entry_t *new_entry;
    new_entry = malloc(sizeof(struct entry_t));
    new_entry->key = key;
    new_entry->value = data;
    return new_entry;
}

void entry_initialize(struct entry_t *entry){
    if(entry != NULL){
        entry->key = NULL;
        entry->value = NULL;
    }
    else {
        printf("[ERROR] function cannot initializate entry with NULL argument\n");
    }
}


