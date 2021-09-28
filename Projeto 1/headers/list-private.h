#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "list.h"
#include <stddef.h> //NULLS

struct node_t {
    struct node_t *parent;
    struct node_t *child;
    struct node_t *current_node;
    struct entry_t *current_entry;
};

struct list_t {
    int size;
    struct node_t *nodes;
};

void list_print(struct list_t* list); //check if this function really belongs here


/**
 * alocate memory to node and initialize parent and child with NULL value
 **/
void initializeNode(struct node_t *node){
    node = malloc(sizeof(struct node_t));
    node->parent = malloc(sizeof(struct node_t));
    node->child = malloc(sizeof(struct node_t));
    node->current_node = malloc(sizeof(struct node_t));
    node->current_entry = entry_create(NULL, NULL); //initializing entry without values

    node->parent = NULL;
    node->child = NULL;
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
    if(node->current_entry->key == key){
        return node; //true   
    }
    else if(node->child != NULL){
        getNodeIfKeyExist(node->child, key);    
    } 
    else {
        return NULL;
    }
}

struct node_t *getNodeHead(struct node_t *nodes){
    if(nodes->parent == NULL && nodes->current_node != NULL){
        return nodes->current_node;
    }
    else {
        getNodeHead(nodes->parent);
    }
}

#endif
