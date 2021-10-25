#include "../include/client/client_stub.h"
#include "../include/client/client_stub-private.h"
#include "../proto/sdmessage.pb-c.h"
#include <stddef.h> //NULLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/serialization.h"

//"19012392103921:0909"
struct rtable_t *rtable_connect(const char *address_port){
    struct rtable_t *remote_table = malloc(sizeof(struct rtable_t));
    char *endpoint = malloc(sizeof(char)*strlen(address_port));
    strcpy(endpoint, address_port);
    char *splitedEndpoint = strtok(endpoint, ":");
    remote_table->address = malloc(sizeof(char)); // can be necessary to *strlen(hostname) (=) strlen(strtok(address_port, ":"))
    remote_table->address = endpoint;
    remote_table->port = strtol(strtok(NULL,""), NULL, 10);
    //remote_table->table = table_create(1); //criar a table com 1 lista
    remote_table->server = malloc(sizeof(struct sockaddr_in));

    char str[MAX_MSG];
    int count, nbytes;

    if ((remote_table->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket TCP");
        return -1;
    }
    struct sockaddr_in server_aux;
    server_aux.sin_family = AF_INET;
    server_aux.sin_port = htons(remote_table->port);
    if (inet_pton(AF_INET, remote_table->address, &server_aux.sin_addr) < 1) {
        printf("Erro ao converter IP\n");
        close(remote_table->sockfd);
        return -1;
    }
    remote_table->server = &server_aux;
    free(endpoint);//look if this modify the remote_table
    return remote_table;
}

int rtable_disconnect(struct rtable_t *rtable){
    if(rtable != NULL){
        if(rtable->address != NULL){
            free(rtable->address);
        }
        network_close(rtable);
        return 0;
    }
    return -1;
}

int rtable_put(struct rtable_t *rtable, struct entry_t *entry){
    struct MessageT *msg;
    char *buf;
    unsigned len;
    //msg = malloc(sizeof(struct MessageT)); //may be necessary alocate memory before message_t_init()
    message_t__init(&msg); 

    //msg->base = NULL; // ??????? not necessary as the professor example
    msg->opcode = MESSAGE_T__OPCODE__OP_PUT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_ENTRY;
    msg->data_size = sizeof(entry);//tamanho da entry
    
    char *entry_buf; //falta inicializar, mas talvez nao seja necessario

    entry_to_buffer(entry, &entry_buf);

    msg->data = entry_buf;

    len = message_t__get_packed_size(&msg);
    buf = malloc(len);

    message_t__pack(&msg, buf);

    if(network_send_receive(rtable, msg)!= NULL){
        //n falhou
        return 0;
    }
    else {
        //falhou
        return -1;
    }
}

struct data_t *rtable_get(struct rtable_t *rtable, char *key){
    if(rtable != NULL){
        if(rtable->table != NULL && strcmp(key, NULL) != 0){
            return table_get(rtable->table, key);
        }
        printf("[WARN] invalid NULL argument");
    }
    return NULL;
}

int rtable_del(struct rtable_t *rtable, char *key){
    if(rtable != NULL){
        if(rtable->table != NULL && strcmp(key, NULL) != 0){
            table_del(rtable->table, key);
            return 0;
        }
        printf("[WARN] invalid NULL argument");
    }
    return -1;
}

int rtable_size(struct rtable_t *rtable){
    if(rtable != NULL){
        if(rtable->table != NULL){
            return table_size(rtable->table);
        }
        printf("[WARN] invalid NULL argument");
    }
    return -1;
}

char **rtable_get_keys(struct rtable_t *rtable){
    if(rtable != NULL){
        if(rtable->table != NULL){
            return table_get_keys(rtable->table);
        }
        printf("[WARN] invalid NULL argument");
    }
    return NULL;
}

void rtable_free_keys(char **keys){
    if(keys != NULL){
        table_free_keys(keys);
    }
}

void rtable_print(struct rtable_t *rtable){
    struct MessageT *msg;
    //msg = malloc(sizeof(struct MessageT)); //may be necessary alocate memory before message_t_init()
    message_t__init(msg); 
    //msg->base = NULL; // ???????
    msg->opcode = MESSAGE_T__OPCODE__OP_PRINT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    msg->data_size = 0;
    msg->data = NULL;
    network_send_receive(rtable, msg);
}