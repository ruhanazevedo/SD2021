/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

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

void list_print(struct list_t* list); 


/**
 * Inicializa o nó e suas devidas dependencias
 */
void initializeNode(struct node_t *node);

/**
 * Destrói o nó e suas devidas dependencias
*/
void neutralizeNode(struct node_t *node);

/*
* Função que verifica se o nó recebido é HEAD
*/
int thisNodeIsHead(struct node_t *node);

/*
* Devolve o node_t que está relacionado a key ou NULL caso não exista
*/
struct node_t *getNodeIfKeyExist(struct node_t *node, char *key);

/*
* Retorna o nó HEAD
*/
struct node_t *getNodeHead(struct node_t *nodes);

/*
* Devolve o tail do node_t recebido
*/
struct node_t *getNodeWithoutChild(struct node_t *node);

/*
* Adiciona um novo nó ao node_t recebido
*/
struct node_t *addNewNode(struct node_t *node);

/* Função que devolve um array de entry* com a cópia de todas as keys da 
 * tabela, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária.
 */
struct entry_t **list_get_entrys(struct list_t *list);



#endif