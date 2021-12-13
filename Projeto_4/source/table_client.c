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
#include <zookeeper/zookeeper.h>


#define BUFFERSIZE 50

static zhandle_t *zh;
char *server_endpoint;

int testInput(int argc){
    if (argc != 2){

	    printf("Uso: ./table_client <ip_servidor:porto_servidor> \n");
    	printf("Exemplo de uso: ./table_client 127.0.0.1:2181 \n");
        return -1;
    } 
    return 0;
}

void watcher_function(zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx){
	
	if (ZOK != zoo_wget_children(zh, "/kvstore", watcher_function, watcherCtx, NULL)) {
 		fprintf(stderr, "Error setting watch at %s!\n", "/kvstore/primary");
	}

	int primary = zoo_exists(zh, "/kvstore/primary", 0, NULL);
	char *server_endpoint_tmp[64];
    int bufferlen1 = sizeof(server_endpoint_tmp);

    int flag1 = zoo_get (zh, "/kvstore/primary", 0,
                    server_endpoint_tmp, & bufferlen1, NULL);
    

    if (ZOK == primary) {
		if(strcmp(server_endpoint_tmp, server_endpoint) != 0){
            char **input = calloc(1, sizeof(char*)*2);
            input[0] = calloc(1, sizeof(char));
            input[0] = "./table_client";
            input[1] = calloc(1, sizeof(char));
            input[1] = "127.0.0.1:2181";
            main(2, input);
        }
	} 
}


int main(int argc, char** argv) { 
    server_endpoint = malloc(sizeof(char));
    
   struct rtable_t *remote_table;

   if(testInput(argc) < 0) return -1;

   // ligar ao zookeeper
   zh = zookeeper_init(argv[1], watcher_function, 2000, 0, NULL, 0);
   // zoo_exists(primary)
   char *primary_path = "/kvstore/primary";
   int primary = zoo_exists(zh, primary_path, 0, NULL);
   char *server_endpoint_tmp[64];
   if(primary == ZOK){
    int bufferlen1 = sizeof(server_endpoint_tmp);

    int flag1 = zoo_get (zh, primary_path, 0,
                      server_endpoint_tmp, & bufferlen1, NULL);
    
    server_endpoint = server_endpoint_tmp;
   }
   else {
       printf("\n[WARN] De momento não há servidor disponível, tente mais tarde\n");
       return;
   }
    printf("connecting to %s..\n", server_endpoint);
    

    if((remote_table = rtable_connect(server_endpoint)) == NULL){
        return -1;
    }

    char *buffer[BUFFERSIZE];
    printf("Enter a message: \n");
    while(fgets((char *)buffer, BUFFERSIZE , stdin)){
        char *text = calloc(1,1);
        text = realloc( text, strlen(text)+1+strlen((char*)buffer) );
        if( !text ){
            printf("Falha ao receber messagem");
        }
        strcat( text, (char*) buffer ); 
        char *split = strtok(text, "\n");
        char *comparator = strtok(split, " ");

        if(strcmp(text, "\n")== 0){
            printf("Comando não reconhecido\nexperimente usar o comando help para listar as operações disponíveis da tabela remota\n");
        }
        else if(strcmp(split, "quit") == 0){
            printf("disconnecting..\n");
            rtable_disconnect(remote_table);
            break;
        }
        
        else if((strcmp(comparator, "put")) == 0){
            char *argKey = strtok(NULL, " ");
            
            char *argData = strtok(NULL, " ");
            if(argKey == NULL || argData == NULL){
                printf("Comando put precisa de mais argumentos para execução\nDigite help\n");
                continue;
            }

            int datasize = strlen(argData) + 1;
            struct data_t *data = data_create2(datasize, argData);
            struct entry_t *entry = entry_create(argKey, data);
            int res = -99;
            if((res = rtable_put(remote_table, entry)) == 0){
                printf("Inserindo conjunto {key: %s, value:%s} na tabela\n", argKey, argData);
            }
            else{
                printf("falha na execução do comando put\n");
            }
        }

        else if(strcmp(comparator, "get") == 0){
            char *argKey = strtok(NULL, " ");
            if(argKey == NULL){
                printf("Comando get precisa de mais argumentos para execução\nDigite help\n");
                continue;
            }
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
            if(argKey == NULL){
                printf("Comando del precisa de mais argumentos para execução\nDigite help\n");
                continue;
            }
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
            printf("Executando comando getKeys..\n");
            if((res = rtable_get_keys(remote_table)) != NULL){
                
                if(res != NULL){
                    if(res[0] == NULL){
                        printf("Não existem keys na remote_table\n");
                    }
                    else{
                        printf("keys obtidas:\n");
                        int size_res = (size_t) res[0];//strtol(res[0], aux, 10);
                        for(int i=1 ; i<size_res+1  ; i++){
                            printf("%s\n", res[i]);
                        }
                    }
                }
                else {
                    printf("Não existem keys na remote_table\n");
                }
                
            }
            else {
                printf("falha na execução do comando getKeys\n");
            }
        }
        else if(strcmp(comparator, "stats") == 0){
            struct statistics *stats;
            printf("Executando comando stats..\n");
            if((stats = rtable_stats(remote_table)) != NULL){
                printf("Estatística da tabela remota:\n");
                printf("\tNúmero de vezes que o comando put foi realizado: %d\n", stats->n_put);
                printf("\tNúmero de vezes que o comando get foi realizado: %d\n", stats->n_get);
                printf("\tNúmero de vezes que o comando del foi realizado: %d\n", stats->n_del);
                printf("\tNúmero de vezes que o comando size foi realizado: %d\n", stats->n_size);
                printf("\tNúmero de vezes que o comando getkeys foi realizado: %d\n", stats->n_getkeys);
                printf("\tNúmero de vezes que o comando table_print foi realizado: %d\n", stats->n_table_print);
                printf("\tTempo médio de execução de todas as operações na tabela remota: %f\n", stats->avg_time);
            }
            else {
                printf("falha na execução do comando stats\n");
            }
        }
        else if(strcmp(comparator, "help") == 0){
            printf("Lista de operações disponíveis:\n");
            printf("\tput <key> <data>\n");
            printf("\t\tInsere um conjunto de key data na tabela remota\n");
            printf("\tsize\n");
            printf("\t\tRetorna o tamanho da tabela remota\n");
            printf("\tdel <key>\n");
            printf("\t\tApaga a key <key> da tabela remota\n");
            printf("\tget <key>\n");
            printf("\t\tRetorna o data pertencente à key <key> da tabela remota\n");
            printf("\tgetkeys\n");
            printf("\t\tRetorna todas as keys existentes na tabela remota\n");
            printf("\ttable_print\n");
            printf("\t\timprime tabela remota\n");
            printf("\tstats\n");
            printf("\t\tEstatísticas da tabela remota\n");
            printf("\tquit\n");
            printf("\t\tTermina a conexão com o servidor\n");
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
            printf("Comando não reconhecido\nexperimente usar o comando help para listar as operações disponíveis da tabela remota\n");
            
        }
        free(text);
    }

 }