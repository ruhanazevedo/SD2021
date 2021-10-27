#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "../../Projeto_1/source/data.c"
//#include "../../Projeto_1/source/entry.c"
#include "../include/client_stub.h"
#include "data.c"
#include "entry.c"
#include "client_stub.c"
#include "../include/extra/inet.h"

#define BUFFERSIZE 50

int main(int argc, char** argv) { 
    struct rtable_t *remote_table;
    if(argc == 2){
        printf("%s\n", argv[1]);
        remote_table = rtable_connect(argv[1]);
    }

    char *text = calloc(1,1), buffer[BUFFERSIZE];
    printf("Enter a message: \n");
    while(fgets(buffer, BUFFERSIZE , stdin)){
        text = realloc( text, strlen(text)+1+strlen(buffer) );
        if( !text ){
            printf("fail");
        }
        strcat( text, buffer ); 
        //printf("%s\n", buffer);   
        char *split = strtok(text, "\n");
        if(strcmp(split, "quit") == 0){
            break;
        }
        else if(strcmp(strtok(split, " "), "put") == 0){
            char *argKey = strtok(NULL, " ");
            char *argData = strtok(NULL, " ");

            int datasize = strlen(argData);
            struct data_t *data = data_create2(datasize, argData);
            struct entry_t *entry = entry_create(argKey, data);
            //rtable_put();
        }

        else if(strcmp(strtok(split, " "), "get") == 0){
            char *argKey = strtok(NULL, " ");
        }

        else if(strcmp(strtok(split, " "), "del") == 0){
            char *argKey = strtok(NULL, " ");
        }

        else if(strcmp(split, "table_print") == 0);
        else if(strcmp(split, "getkeys") == 0);
        else if(strcmp(split, "size") == 0);
    }

 }