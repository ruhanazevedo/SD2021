#include "../include/client/client_stub.h"
#include "../include/client/client_stub-private.h"
#include <stddef.h> //NULLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rtable_t *rtable_connect(const char *address_port){
    struct rtable_t *remote_table = malloc(sizeof(struct rtable_t));
    char *endpoint = malloc(sizeof(char)*strlen(address_port));
    strcpy(endpoint, address_port);
    char *splitedEndpoint = strtok(endpoint, ":");
    remote_table->address = malloc(sizeof(char)); // can be necessary to *strlen(hostname) (=) strlen(strtok(address_port, ":"))
    remote_table->address = endpoint;
    remote_table->port = strtol(strtok(NULL,""), NULL, 10);
    remote_table->table = table_create(1); //criar a table com 1 lista
    return remote_table;
}

int rtable_disconnect(struct rtable_t *rtable){
    if(rtable != NULL){
        if(rtable->address != NULL){
            free(rtable->address);
        }
        if(rtable->table != NULL){
            table_destroy(rtable->table);
        }
        free(rtable);
        return 0;
    }
    return -1;
}

int rtable_put(struct rtable_t *rtable, struct entry_t *entry){
    if(rtable != NULL && entry != NULL){
        if(rtable->table != NULL && entry->value != NULL && strcmp(entry->key,NULL) != 0 ){
            table_put(rtable->table, entry->key, entry->value);
            return 0;
        }
        printf("[ERROR] if this was hit, something can be wrong");
    }
    
    return -1;
}

struct data_t *rtable_get(struct rtable_t *rtable, char *key){
    if(rtable != NULL){
        if(rtable->table != NULL && strcmp(key, NULL) != 0){
            return table_get(rtable->table, key);
        }
        printf("[WARN] invalid NULL argument");
    }
    return NULL;
}

int rtable_del(struct rtable_t *rtable, char *key){
    if(rtable != NULL){
        if(rtable->table != NULL && strcmp(key, NULL) != 0){
            table_del(rtable->table, key);
            return 0;
        }
        printf("[WARN] invalid NULL argument");
    }
    return -1;
}

int rtable_size(struct rtable_t *rtable){
    if(rtable != NULL){
        if(rtable->table != NULL){
            return table_size(rtable->table);
        }
        printf("[WARN] invalid NULL argument");
    }
    return -1;
}

char **rtable_get_keys(struct rtable_t *rtable){
    if(rtable != NULL){
        if(rtable->table != NULL){
            return table_get_keys(rtable->table);
        }
        printf("[WARN] invalid NULL argument");
    }
    return NULL;
}

void rtable_free_keys(char **keys){
    if(keys != NULL){
        table_free_keys(keys);
    }
}

void rtable_print(struct rtable_t *rtable){
    if(rtable != NULL){
        if(rtable->table != NULL){
            table_print(rtable->table);
        }
    }
}