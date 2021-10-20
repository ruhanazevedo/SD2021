#include <stdio.h>

#include "../source/client_stub.c"
#include "../include/client/client_stub-private.h"
#include "../include/client/client_stub.h"

int main(int argc, char **argv){
    printf("\nHello world\n");
    struct rtable_t *x = rtable_connect("endpoint:123");
}
