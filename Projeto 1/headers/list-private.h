#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "list.h"

struct node_t {
    node_t *child;
    node_t *parent;
    node_t *current_node;
    entry_t *current_entry;
};

struct list_t {
    int size;
    node_t *nodes;
};

void list_print(struct list_t* list);

int thisNodeIsHead(node_t *node){
    if(node->parent == NULL){
        return 1; //true
    }    
    else {
        return 0; //false
    }
}

node_t *getNodeIfKeyExist(node_t *node, char *key){
    if(node->current_entry->key == key){
        return node; //true   
    }
    else if(node->child != NULL){
        existKeyOnNode(node->child, key);    
    } 
    else {
        return NULL;
    }
}

#endif
