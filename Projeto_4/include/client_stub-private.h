/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/


#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H

#include "client_stub.h"
#include "table.h"
#include "extra/inet.h" 

struct rtable_t{
    char *address;
    int port;
    //struct table_t *table;
    int sockfd;
    struct sockaddr_in *server;
};

#endif