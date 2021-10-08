//#include "../headers/data.h"
#include "../include/entry.h"
#include <stddef.h> //NULLS
#include <string.h> //strcmp
#include <stdlib.h>
#include <stdio.h>

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

void entry_destroy(struct entry_t *entry){
    data_destroy(entry->value); //can't be removed if isn't necessary
    free(entry);
}

struct entry_t *entry_dup(struct entry_t *entry){
    struct entry_t *new_entry;
    new_entry = entry_create(entry->key, entry->value);
    if(new_entry != NULL){
        return new_entry;
    }
    else {
        printf("[WARN] function did not do what was expected\n");
        return NULL;
    }
    
}

void entry_replace(struct entry_t *entry, char *new_key, struct data_t *new_value){
    data_destroy(entry->value);
    entry->key = new_key;
    entry->value = new_value;
}

int entry_compare(struct entry_t *entry1, struct entry_t *entry2){
    int comparator = strcmp(entry1->key, entry2->key);
    if(comparator > 0){
        return 1;
    }
    else if(comparator < 0){
        return -1;
    }
    else {
        return 0; //equal
    }
}