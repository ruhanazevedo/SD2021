#include "../headers/list.h"
#include "../headers/list-private.h"

struct list_t *list_create(){
    struct list_t *new_list;
    new_list = malloc(sizeof(struct list_t));
    new_list->size = 0;
    new_list->nodes->child = NULL;
    new_list->nodes->parent = NULL;
    new_list->nodes->current_node = NULL;
    new_list->nodes->current_entry = NULL;
    return new_list;
}

void list_destroy(struct list_t *list){
    free(list);
}

int list_add(struct list_t *list, struct entry_t *entry){
    if(entry != NULL){
        list->nodes->child->current_entry = entry;
        list->nodes->child->parent = list->nodes->current_node;
        list->nodes->child->child = NULL;
        ++list->size;
        return 0;
    }
    else {
        return -1;
    }
}

int list_remove(struct list_t *list, char *key){
    node_t *node = getNodeIfKeyExist(list->nodes, key);
    if(node != NULL){
        //node->current_entry = NULL;
        free(node->current_entry);
        return 0;
    }
    else {
        printf("[ERROR] key not found in the node's list\n");
        return -1;
    }
}

struct entry_t *list_get(struct list_t *list, char *key){
    node_t *node = getNodeIfKeyExist(list->nodes, key);
    if(node != NULL){
        return node->current_entry;
    }
    else {
        printf("[ERROR] key not found in the node's list\n");
        return NULL;
    }    
}

int list_size(struct list_t *list){
    if(list != NULL){
        return list->size;
    } 
    else {
        printf("[ERROR] cannot get the size of NULL\n");
        return -1;
    }
}
