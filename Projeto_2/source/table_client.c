#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/client_stub.h"
//#include "data.c" 
//#include "entry.c"
#include "../include/data.h"
#include "../include/entry.h"
//#include "/include/client_stub.h"
#include "../include/extra/inet.h"
#include "../include/data-private.h"
#include "../include/network_client.h"

#define BUFFERSIZE 50

int main(int argc, char** argv) { 
    struct rtable_t *remote_table;
    if(argc >= 2){
        printf("trying to reach %s\n", argv[1]);
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
            printf("disconnecting..\n");
            break;
        }
        else if((strcmp(strtok(split, " "), "put")) == 0){
            char *argKey = strtok(NULL, " ");
            char *argData = strtok(NULL, " ");

            int datasize = strlen(argData);
            struct data_t *data = data_create2(datasize, argData);
            struct entry_t *entry = entry_create(argKey, data);
            int res = -99;
            if((res = rtable_put(remote_table, entry)) == 0){
                printf("Inserindo conjunto {key: %s, values:%s to the table}\n", argKey, argData);
            }
            else{
                printf("falha na execução do comando put\n");
            }
        }

        else if(strcmp(strtok(split, " "), "get") == 0){
            char *argKey = strtok(NULL, " ");
            struct data_t *data;
            if((data = rtable_get(remote_table, argKey)) != NULL){
                printf("comando get realizado para a key: %s", argKey);
                data_print(data);
            }
            else{
                printf("falha na execução do comando get\n");
            }
        }

        else if(strcmp(strtok(split, " "), "del") == 0){
            char *argKey = strtok(NULL, " ");

            int res = -99;
            if((res = rtable_del(remote_table, argKey)) == 0){
                printf("Apagando entrada na tabela remota para a key: %s\n", argKey);
            }
            else {
               printf("falha na execução do comando del\n");
            }
        }

        else if(strcmp(split, "table_print") == 0){
            printf("imprimindo tabela remota:\n");
            rtable_print(remote_table);
        }

        else if(strcmp(split, "getkeys") == 0){
            char **res;
            if((res = rtable_get_keys(remote_table)) != NULL){
                printf("Executando comando getKeys..\n");
                printf("keys obtidas:\n");
                for(int i=0 ; i<sizeof(res) ; i++){
                    printf("%s\n", res[i]);
                }
            }
            else {
                printf("falha na execução do comando geyKeys\n");
            }
        }

        else if(strcmp(split, "size") == 0){
            printf("Executando comando size..\n");
            int res = -99;
            if((res = rtable_size(remote_table)) != -1){
                printf("size obtido: %d\n", res);
            }
            else {
                printf("falha na execução do comando size\n");
            }
        }
    }

 }