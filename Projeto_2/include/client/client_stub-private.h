#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H

#include "client_stub.h"
#include "../table.h"

struct rtable_t{
    char *address;
    int port;
    struct table_t *table;
};

#endif