#include <stdio.h>

#include "headers/data.h"
#include "src/data.c"

#include "headers/entry.h"
#include "src/entry.c"

int main(void){
    int x = 1;
    ++x;
    printf("%d",x);
    /*struct data_t *x = data_create2(2, NULL);
    char *key = NULL;
    struct entry_t *y = NULL;
    entry_initialize(y);*/
    //char str[] = "test\n";
    //printf("%s", str);
}