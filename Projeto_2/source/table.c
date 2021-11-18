/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/table.h"
#include "../include/table-private.h" 
#include "list.c"
#include <string.h> //strcmp


struct table_t *table_create(int n){
    struct table_t *new_table;
    new_table = malloc(sizeof(struct table_t));
    new_table->nListas = n;
    new_table->size = 0;
    new_table->list = malloc(n * sizeof(struct list_t));
    for(int i=0 ; i<n ; i++){
        new_table->list[i] = list_create();
    }
    return new_table;
}

void table_destroy(struct table_t *table){
    if(table != NULL){   
        if(table->list != NULL){
            for(int i=0 ; i<table->nListas ; i++){
                list_destroy(table->list[i]);
            }
            free(table->list);
        }
        free(table);
    }
}

//@requires: table != NULL
int table_put(struct table_t *table, char *key, struct data_t *value){
    if(value == NULL || key == NULL || table == NULL || (strcmp(key, "") == 0)){
        printf("[WARN] some argument provided is NULL, can't proceed\n");
        return -1;
    }
    char *key_copy = malloc(strlen(key)+1);
    strcpy(key_copy, key);
    struct data_t *data_copy = data_dup(value);
    
    int index = hash((unsigned char *) key) % table->nListas;
    struct entry_t *new_entry = entry_create(key_copy, data_copy);
    if(list_get(table->list[index], key_copy) == NULL){
        ++table->size;
    }
    list_add(table->list[index], new_entry);
    return 0;
}

struct data_t *table_get(struct table_t *table, char *key){
    if(table == NULL || key == NULL || strcmp(key, "") == 0){
        printf("[ERROR] the argument received is invalid\n");
        return NULL;
    }
    int index = hash((unsigned char *) key) % table->nListas;
    if(table->list[index] != NULL){
        struct node_t *node = getNodeIfKeyExist(table->list[index]->nodes, key);
        if(node != NULL){
            return node->current_entry->value;
        }
    }
    printf("[ERROR] internal_error: if this was hit, something is wrong\n");
    return NULL;
}

int table_del(struct table_t *table, char *key){
    printf("entrou no table_del\n");
    if(table == NULL || key == NULL || strcmp(key, "") == 0){
        printf("[ERROR] Unexpected NULL argument\n");
        return -1;
    }

    int index = hash((unsigned char *) key) % table->nListas;
    
    if(table->list[index] == NULL){
        printf("[WARN] key not found\n");
        return -1;
    }
    
    if(list_remove(table->list[index], key) == 0){
        --table->size;
        return 0;
    }
    printf("[ERROR] internal_error: If this was hit, something is wrong!\n");
    return -1;
}

int table_size(struct table_t *table){
    if(table != NULL){
        return table->size; 
    }
    return -1;
}

char **table_get_keys(struct table_t *table){
    char **keys = malloc(sizeof(char)*table->size + sizeof(NULL)); 
    int k = 0;
    for(int i=0 ; i<table->size ; i++){
        if(table->list[i] != NULL){
            int listSize = list_size(table->list[i]); 
            char **list_keys = list_get_keys(table->list[i]); 
            for(int j=0 ; j<listSize ; j++){
                if(list_keys[j] != NULL){
                    keys[k] =  list_keys[j];
                    ++k;
                }
            }
        }
    }
    //keys[k] = NULL;
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
    for(int i=0 ; i<table->nListas ; i++){
        if(table->list[i] != NULL){
            list_print(table->list[i]);
        }
    }
    printf("]\n");
    
}

/*******************************
****** FUNÇÕES AUXILIARES ******
********************************/

int hash(unsigned char *str){
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return abs(i % CAPACITY);
}
/*
char **table_get_entrys(struct table_t *table){

    struct entry_t **entries = malloc(sizeof(struct entry_t)*table->size + sizeof(NULL)); 
    int k = 0;
    for(int i=0 ; i<table->size ; i++){
        if(table->list[i] != NULL){
            int listSize = list_size(table->list[i]); 
            char **list_entries = (char **) list_get_entrys(table->list[i]); 
            for(int j=0 ; j<listSize ; j++){
                if(list_entries[j] != NULL){
                    entries[k] =  list_entries[j];
                    ++k;
                }
            }
        }
    }
    entries[k] = NULL;
    return entries;
}
*/