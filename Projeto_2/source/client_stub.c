/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/client_stub-private.h"
#include "../include/sdmessage.pb-c.h"
#include <stddef.h> //NULLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/serialization.h"
#include "serialization.c"
#include "table.c"
#include "../include/extra/inet.h"

struct rtable_t *rtable_connect(const char *address_port){
    struct rtable_t *remote_table = malloc(sizeof(struct rtable_t));
    char *endpoint = malloc(sizeof(char)*strlen(address_port));
    strcpy(endpoint, address_port);
    char *splitedEndpoint = strtok(endpoint, ":");
    remote_table->address = malloc(sizeof(char)); 
    
    memcpy(remote_table->address, endpoint, strlen(endpoint));
    remote_table->port = strtol(strtok(NULL,""), NULL, 10); 
    
    remote_table->server = malloc(sizeof(struct sockaddr_in ));

    if ((remote_table->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket TCP");
        return NULL;
    }
    struct sockaddr_in server_aux;
    server_aux.sin_family = AF_INET;
    server_aux.sin_port = htons(remote_table->port);
    if (inet_pton(AF_INET, remote_table->address, &server_aux.sin_addr) < 1) {
        printf("Erro ao converter IP\n");
        close(remote_table->sockfd);
        return NULL;
    }
    remote_table->server = &server_aux;
    free(endpoint);//look if this modify the remote_table
    network_connect(remote_table);
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

    msg = malloc(sizeof(struct MessageT));

    message_t__init(msg); 

    msg->opcode = MESSAGE_T__OPCODE__OP_PUT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_ENTRY;

    msg->n_entries = 1;

    struct MessageT__Entry *entry_tmp;
    entry_tmp = malloc(sizeof(struct MessageT__Entry));
    message_t__entry__init(entry_tmp);
    entry_tmp->key = entry->key;
    entry_tmp->data.data = entry->value->data;
    entry_tmp->data.len = entry->value->datasize;

    msg->entries[0] = &entry_tmp;

    if(network_send_receive(rtable, msg)!= NULL){
        return 0;
    }
    return -1;
}

struct data_t *rtable_get(struct rtable_t *rtable, char *key){
    struct data_t *data;
    if(rtable != NULL && strcmp(key, NULL) != 0){
        struct MessageT *msg, *msg_received;

        msg = malloc(sizeof(struct MessageT));

        message_t__init(msg); 

        msg->opcode = MESSAGE_T__OPCODE__OP_GET;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
        msg-> n_keys = 1;
        strcpy(msg->keys[0], key);     
        msg_received = malloc(sizeof(struct MessageT));
        if(msg_received = network_send_receive(rtable, msg) != NULL){
            data = malloc(sizeof(struct data_t));
            memcpy(data->data, &msg_received->data.data, msg_received->data.len);
            memcpy(data->datasize, &msg_received->data.len, sizeof(msg_received->data.len));
            return data;
        }

    }
    return NULL;
}

int rtable_del(struct rtable_t *rtable, char *key){
    if(rtable != NULL && strcmp(key, NULL) != 0){
        struct MessageT *msg, *msg_received;

        msg = malloc(sizeof(struct MessageT));

        message_t__init(msg); 

        msg->opcode = MESSAGE_T__OPCODE__OP_DEL;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
        msg->n_keys = 1;
        strcpy(msg->keys[0], key);
        
        msg_received = malloc(sizeof(struct MessageT));
        if(msg_received = network_send_receive(rtable, msg) != NULL){
            if(msg_received->opcode != MESSAGE_T__OPCODE__OP_ERROR){
                //n retorna erro
                return 0;
            }
        }
        return -1;
    }
    return -1;
}

int rtable_size(struct rtable_t *rtable){
    if(rtable != NULL){
        struct MessageT *msg, *msg_received;
        msg = malloc(sizeof(struct MessageT));

        message_t__init(msg); 
        msg->opcode = MESSAGE_T__OPCODE__OP_SIZE;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

        msg_received = malloc(sizeof(struct MessageT));
        
        if((msg_received = network_send_receive(rtable, msg)) != NULL){
            if(msg_received->c_type == MESSAGE_T__C_TYPE__CT_RESULT){
                return msg_received->result;
            }
            
        }
    }
    return -1;
}

char **rtable_get_keys(struct rtable_t *rtable){
    char **keys;
    if(rtable != NULL){
        //TODO TRICKY
        struct MessageT *msg, *msg_received;

        msg = malloc(sizeof(struct MessageT));

        message_t__init(msg);

        msg->opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
        msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;

        msg_received = malloc(sizeof(struct MessageT));

        if(msg_received = network_send_receive(rtable, msg) != NULL){
            if(msg_received->c_type == MESSAGE_T__C_TYPE__CT_KEYS){
                keys = malloc(sizeof(char)*msg_received->n_keys);
                for(int i=0 ; i<msg_received->n_keys ; i++){
                    strcpy(keys[i], msg_received->keys[i]);
                }
                return keys;
            }
        }
    }
    return NULL;
}

void rtable_free_keys(char **keys){ //FALTA receber o TABLE
    if(keys != NULL){
        table_free_keys(keys);
    }
    printf("[ERROR] invalid argument keys");
}

void rtable_print(struct rtable_t *rtable){
    struct MessageT *msg, *msg_received;
    msg = malloc(sizeof(struct MessageT));
    message_t__init(msg); 
    
    msg->opcode = MESSAGE_T__OPCODE__OP_PRINT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

    msg_received = malloc(sizeof(struct MessageT));
    if(msg_received = network_send_receive(rtable, msg) != NULL){
        if(msg_received->opcode == MESSAGE_T__OPCODE__OP_PRINT){
            printf("{\n");
            printf("%5dtabela_remote: {", 2);
            for(int i=0 ; i<msg_received->n_entries ; i++){
                printf("%5d'key': '%s'",3,msg_received->entries[i]->key);
                printf("%5d'data': ",3);
                
                printf("%5d'datasize': '%d'",4, msg_received->entries[i]->data.len);
                printf("%5d'data': '%s'",4, msg_received->entries[i]->data.data);
            }
            printf("%5d}", 2);
            printf("}\n");
        }
    }
}

