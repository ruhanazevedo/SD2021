/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/list.h"
#include "../include/list-private.h"
#include "../include/data-private.h" 
#include "../include/data.h"
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
    if(list != NULL){
        if(list->nodes != NULL){
            neutralizeNode(list->nodes);
            free(list->nodes);
        }
        free(list);
    }
    
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
        else { 
            free(comparable->current_entry);
            comparable->current_entry = entry;
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
            if(node->child != NULL){
                node->child->parent = NULL;
                list->nodes = node->child;
            }
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
        printf("[WARN] list received is NULL\n");
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

/*******************************
****** FUNÇÕES AUXILIARES ******
********************************/

void initializeNode(struct node_t *node){
    node->parent = malloc(sizeof(struct node_t));
    node->child = malloc(sizeof(struct node_t));
    node->current_entry = malloc(sizeof(struct entry_t));

    node->child = NULL;
}

void neutralizeNode(struct node_t *node){
    if(node != NULL){
        free(node->parent);
        free(node->child);
        free(node->current_entry); //call entry_destroy 
    }
    
}

int thisNodeIsHead(struct node_t *node){
    if(node->parent == NULL){
        return 1; //true
    }    
    else {
        return 0; //false
    }
}

struct node_t *getNodeIfKeyExist(struct node_t *node, char *key){
    if(node->current_entry->key != NULL){ 
        if(strcmp(node->current_entry->key, key) == 0){ //the node received have the same key as the argument
            return node;
        }
        else {
            if(node->child != NULL){
                return getNodeIfKeyExist(node->child, key); //try call function again with child instead
            }
            else {
                return NULL; //if doesn't have child and dont have kill, this key doesn't exist, return NULL
            }
        }
    }
    else {
        return NULL; //if this node doesn't have a key, the next won't have
    } 
    return NULL; // this is unnecessary but we need to use just to dont caught warning
}

struct node_t *getNodeHead(struct node_t *nodes){
    if(nodes->parent == NULL && nodes != NULL){
        return nodes;
    }
    else {
        return getNodeHead(nodes->parent);
    }
    return NULL; // this is unnecessary but we need to use just to dont caught warning
}

struct node_t *getNodeWithoutChild(struct node_t *node){
    if(node->child == NULL){
        return node;
    }
    else {
        return getNodeWithoutChild(node->child);
    }
    return NULL; // this is unnecessary but we need to use just to dont caught warning
}

struct node_t *addNewNode(struct node_t *node){
    if(node->child == NULL){
        node->child = malloc(sizeof(struct node_t));
        initializeNode(node->child);
        node->child->parent = node;
        return node->child;
    }
    else {
        printf("[WARN] this node is not the tail\n");
        return NULL;
    }   
}

struct entry_t **list_get_entrys(struct list_t *list){
    
    if(list == NULL){
        printf("[WARN] list received is NULL\n");
        return NULL;
    }

    struct entry_t **array = malloc(sizeof(struct entry_t)*(list->size)+1);
    struct node_t *nodesList = getNodeHead(list->nodes);
    int i = 0;
    
    while(i < list->size){
        array[i] = entry_dup(nodesList->current_entry); //nodesList->current_entry->key;
        ++i;
        nodesList = nodesList->child;
    }
    printf("%d", list->size);
    array[list->size] = NULL;
    
    return array;
}