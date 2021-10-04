#include "../headers/table.h"
#include "../headers/table-private.h" 
#include "../headers/data.h" 
#include "../headers/list.h" 
#include "../headers/list-private.h" 


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
    free(table->list);
    free(table);
}

//@requires: table != NULL
int table_put(struct table_t *table, char *key, struct data_t *value){
    char *key_copy = malloc(strlen(key)+1);
    strcpy(key_copy, key);
    struct data_t *data_copy = data_dup(value); //maybe isn't necessary use the copy of value
    /*for(int i=0 ; i<table->size ; i++){
        table->list[i]=
    }*/
    int index = hash(key) % table->size;
    struct entry_t *new_entry = entry_create(key_copy, data_copy);
    list_add(table->list[index], new_entry);
    //table->list[index]->nodes->current_entry = new_entry;
}
