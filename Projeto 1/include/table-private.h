/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H
#define CAPACITY 5000 // Size of the Hash Table

#include "list.h"
#include <stdlib.h>
#include <stdlib.h>

struct table_t {
    int nListas;
    int size;
    struct list_t **list;
};

/*
* Função hash usada para transformar cada chave num índice 
* de um array (list) onde ficará armazenado o par chave-valor.
*/
int hash(unsigned char *str);

#endif
