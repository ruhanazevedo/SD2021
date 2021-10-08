#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "list.h"
#include <stdlib.h>
#include <stdlib.h>

struct table_t {
    int size;
    struct list_t **list;
};

int hash(unsigned char *str){
    unsigned long hash = 1;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

#endif
