#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "list.h"
#include <stddef.h> //NULLS
#include <stdio.h>
#include <string.h> //strcmp
#include <stdlib.h>

struct node_t {
    struct node_t *parent;
    struct node_t *child;
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
    node->parent = malloc(sizeof(struct node_t));
    node->child = malloc(sizeof(struct node_t));
    node->current_entry = entry_create(NULL, NULL); //initializing entry without values

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
    if(node->current_entry->key != NULL){ 
        if(strcmp(node->current_entry->key, key) == 0){ //the node received have the same key as the argument
            return node;
        }
        else {
            if(node->child != NULL){ //check if the node received have child
                getNodeIfKeyExist(node->child, key); //try call function again with child instead
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
        getNodeHead(nodes->parent);
    }
    return NULL; // this is unnecessary but we need to use just to dont caught warning
}

struct node_t *getNodeWithoutChild(struct node_t *node){
    if(node->child == NULL){
        return node;
    }
    else {
        getNodeWithoutChild(node->child);
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
        printf("[WARN] this node is not the tail");
        return NULL;
    }   
}


#endif