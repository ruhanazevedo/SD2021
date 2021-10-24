#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H

#include "client_stub.h"
#include "../table.h"
#include "../extra/inet.h" //podemos acrescentar o inet.h aqui?

struct rtable_t{
    char *address;
    int port;
    //struct table_t *table;
    int sockfd;
    struct sockaddr_in *server;
};

#endif