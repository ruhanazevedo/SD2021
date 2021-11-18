/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/client_stub.h"
#include "../include/data.h"
#include "../include/entry.h"
#include "../include/extra/inet.h"
#include "../include/data-private.h"
#include "../include/network_client.h"

#define BUFFERSIZE 50

int testInput(int argc){
    if (argc != 2){

	    printf("Uso: ./table_client <ip_servidor:porto_servidor> \n");
    	printf("Exemplo de uso: ./table_client 127.0.0.1:12345 \n");
        return -1;
    } 
    return 0;
}

int main(int argc, char** argv) { 
    struct rtable_t *remote_table;

    if(testInput(argc) < 0) return -1;

    printf("trying to reach %s\n", argv[1]);
    
    if((remote_table = rtable_connect(argv[1])) == NULL){
        return -1;
    }

    char *buffer[BUFFERSIZE];
    printf("Enter a message: \n");
    while(fgets((char *)buffer, BUFFERSIZE , stdin)){
        char *text = calloc(1,1);
        text = realloc( text, strlen(text)+1+strlen((char*)buffer) );
        if( !text ){
            printf("fail");
        }
        strcat( text, (char*) buffer ); 
        char *split = strtok(text, "\n");
        char *comparator = strtok(split, " ");
        if(strcmp(split, "quit") == 0){
            printf("disconnecting..\n");
            break;
        }
        
        else if((strcmp(comparator, "put")) == 0){
            printf("entrou no put\n");
            char *argKey = strtok(NULL, " ");
            printf("split de key %s\n", argKey);
            char *argData = strtok(NULL, " ");
            printf("split de data %s\n", argData);
            printf("pointer de argData %p\n", argData);
            printf("strlen %ld\n", (strlen(argData)+1));


            int datasize = strlen(argData) + 1;
            printf("datasize = %d\n", datasize);
            struct data_t *data = data_create2(datasize, argData);
            struct entry_t *entry = entry_create(argKey, data);
            printf("entry: key = %s\n", entry->key);
            data_print(entry->value);
            printf("\n");
            int res = -99;
            if((res = rtable_put(remote_table, entry)) == 0){
                printf("Inserindo conjunto {key: %s, value:%s na tabela}\n", argKey, argData);
            }
            else{
                printf("falha na execução do comando put\n");
            }
        }

        else if(strcmp(comparator, "get") == 0){
            char *argKey = strtok(NULL, " ");
            struct data_t *data;
            if((data = rtable_get(remote_table, argKey)) != NULL){
                printf("comando get realizado para a key: %s\n", argKey);
                data_print(data);
                printf("\n");
                data_destroy(data);
            }
            else{
                printf("chave não encontrada na tabela\n");
            }
        }

        else if(strcmp(comparator, "del") == 0){
            char *argKey = strtok(NULL, " ");

            int res = -99;
            if((res = rtable_del(remote_table, argKey)) == 0){
                printf("Apagando entrada na tabela remota para a key: %s\n", argKey);
            }
            else {
               printf("falha na execução do comando del\n");
            }
        }

        else if(strcmp(comparator, "table_print") == 0){
            printf("imprimindo tabela remota:\n");
            rtable_print(remote_table);
        }

        else if(strcmp(comparator, "getkeys") == 0){
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
        else {
            printf("Comando não reconhecido\n");
            
        }
        free(text);
    }

 }