#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H
#define CAPACITY 5000 // Size of the Hash Table

#include "list.h"
#include <stdlib.h>
#include <stdlib.h>

struct table_t {
    int size;
    struct list_t **list;
};

int hash(unsigned char *str){
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return abs(i % CAPACITY);
}

#endif
