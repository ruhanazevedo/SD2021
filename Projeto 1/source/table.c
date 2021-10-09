#include "../include/table.h"
#include "../include/table-private.h" 
//#include "../include/data.h" 
//#include "../include/list.h" 
//#include "../include/list-private.h" 
#include "list.c"
#include <string.h> //strcmp


struct table_t *table_create(int n){
    struct table_t *new_table;
    new_table = malloc(sizeof(struct table_t));
    new_table->size = n;
    new_table->list = malloc(n * sizeof(struct list_t));
    for(int i=0 ; i<n ; i++){
        //new_table->list[i] = malloc(sizeof(struct list_t));
        new_table->list[i] = list_create();
    }
    return new_table;
}

void table_destroy(struct table_t *table){
    if(table != NULL){   
        if(table->list != NULL){
            free(table->list);
        }
        free(table);
    }
}

//@requires: table != NULL
int table_put(struct table_t *table, char *key, struct data_t *value){
    char *key_copy = malloc(strlen(key)+1);
    strcpy(key_copy, key);
    if(value == NULL || key == NULL || table == NULL || (strcmp(key, "")!= 0)){
        printf("[WARN] some argument provided is NULL, can't proceed\n");
        return -1;
    }
    struct data_t *data_copy = data_dup(value);
    int index = hash((unsigned char *) key) % table->size;
    struct entry_t *new_entry = entry_create(key_copy, data_copy);
    free(key_copy); //isn't used anymore
    free(data_copy);
    list_add(table->list[index], new_entry);
    ++table->size;
    return 0;
    
     //maybe isn't necessary use the copy of value
    /*for(int i=0 ; i<table->size ; i++){
        table->list[i]=
    }*/
    
    //table->list[index]->nodes->current_entry = new_entry;
}

struct data_t *table_get(struct table_t *table, char *key){
    if(table == NULL || key == NULL || strcmp(key, "") != 0){
        printf("[ERROR] the argument received is invalid");
        return NULL;
    }
    int index = hash((unsigned char *) key) % table->size;
    if(table->list[index] != NULL){
        struct node_t *node = getNodeIfKeyExist(table->list[index]->nodes, key);
        return node->current_entry->value;
    }
    printf("[ERROR] internal_error: If this was hitted, something is wrong");
    return NULL;
}

int table_del(struct table_t *table, char *key){
    if(table == NULL || key == NULL || strcmp(key, "") != 0){
        printf("[ERROR] Unexpected NULL argument");
        return -1;
    }

    int index = hash((unsigned char *) key) % table->size;
    
    if(table->list[index] == NULL){
        printf("[WARN] key not found");
        return -1;
    }
    
    struct entry_t *entry = table->list[index]->nodes->current_entry;
    if(entry != NULL){
        if(entry->key != NULL && entry->value != NULL){
            free(entry->value);
            free(entry->key);
        }
        free(entry);
        return 0;
    }
    printf("[ERROR] internal_error: If this was hitted, something is wrong!");
    return -1;
}

int table_size(struct table_t *table){
    if(table != NULL){
        return table->size; //check if this is correct (case of add keys in the same index of list connected to another node)
    }
    return -1;
}

char **table_get_keys(struct table_t *table){
    char **keys = malloc(sizeof(char)*table->size + sizeof(NULL)); 
    int k = 0;
    for(int i=0 ; i<table->size ; i++){
        if(table->list[i] != NULL){
            int listSize = list_size(table->list[i]); 
            char **list_keys = list_get_keys(table->list[i]); //check if malloc of char** is necessary here
            for(int j=0 ; j<listSize ; j++){
                if(list_keys[j] != NULL){
                    keys[k] =  list_keys[j];
                    ++k;
                }
            }
        }
    }
    keys[k] = NULL;
    return keys;
}

void table_free_keys(char **keys){
    if(keys != NULL){
        free(keys);
    }
}

void table_print(struct table_t *table){
    if(table == NULL){
        printf("NULL_TABLE");
        return;
    }

    printf("table_t[");
    printf("size: %d ; ", table->size);
    for(int i=0 ; i<table->size ; i++){
        if(table->list[i] != NULL){
            list_print(table->list[i]);
        }
    }
    printf("]\n");
    
}
