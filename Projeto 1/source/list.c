#include "../include/list.h"
#include "../include/list-private.h"
#include "data.c"
#include <stddef.h> //NULLS

struct list_t *list_create(){

    struct list_t *new_list;
    new_list = malloc(sizeof(struct list_t));
    new_list->size = 0; //does have at least 1 node (head)
    new_list->nodes = malloc(sizeof(struct node_t));
    initializeNode(new_list->nodes);
    new_list->nodes->parent = NULL; //first node don't have parent, is the HEAD
    return new_list;
}

void list_destroy(struct list_t *list){
    free(list);
}

int list_add(struct list_t *list, struct entry_t *entry){ 
    if(entry != NULL){
        struct node_t *comparable = getNodeIfKeyExist(list->nodes, entry->key);
        struct node_t *node = getNodeWithoutChild(list->nodes); //tail
        if(comparable == NULL){
            if(node->current_entry->key != NULL){ //key received do not exist but this node don't have childs but have a key
                node = addNewNode(node); //create another node connected
            }
            free(node->current_entry);
            node->current_entry = entry;
            ++list->size;
        }
        else { //void entry_replace(struct entry_t *entry, char *new_key, struct data_t *new_value){
            free(comparable->current_entry);
            comparable->current_entry = entry;
            /*entry_replace(comparable->current_entry, 
                          entry->key,
                          entry->value);  */
        }
                    
        return 0;
    }
    else {
        return -1;
    }
}

int list_remove(struct list_t *list, char *key){

    struct node_t *node = getNodeIfKeyExist(list->nodes, key);
    if(node != NULL){
        if(node->parent == NULL){
            node->child->parent = NULL;
            free(node->current_entry);
        }
        else if(node->child == NULL){
            node->parent->child = NULL;
            free(node->current_entry);
        }
        else if(node->parent != NULL){
            node->parent->child = node->child;
            free(node->current_entry);
        }
        
        
        --list->size;
        return 0;
    }
    else {
        printf("[ERROR] key not found in the node's list\n");
        return -1;
    }
}

struct entry_t *list_get(struct list_t *list, char *key){
    struct node_t *node = getNodeIfKeyExist(list->nodes, key);
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

char **list_get_keys(struct list_t *list){
    
    if(list == NULL){
        printf("[WARN] list received is NULL");
        return NULL;
    }

    char **array = malloc(sizeof(char)*(list->size)+1);
    struct node_t *nodesList = getNodeHead(list->nodes);
    int i = 0;
    
    while(i < list->size){
        array[i] = entry_dup(nodesList->current_entry)->key; //nodesList->current_entry->key;
        ++i;
        nodesList = nodesList->child;
    }
    printf("%d", list->size);
    array[list->size] = NULL;
    
    return array;
}

void list_free_keys(char **keys){
    free(keys);
}

void list_print(struct list_t *list){
    if(list == NULL){
        printf("NULL_LIST");
    }
    //struct node_t *nodesList = getNodeHead(list->nodes); //isn't necessary, we supose that list have the HEAD
    struct node_t *node = list->nodes;
    printf("list_t["); 
    while(node != NULL){
        printf("current_entry[key: %s, ", node->current_entry->key);
        data_print(node->current_entry->value);
        printf("]");
        node = node->child;
    }
    printf("] ");
}