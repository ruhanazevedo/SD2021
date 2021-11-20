/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/
#include "../include/network_client.h"
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

//MessageT *msg_received;

struct rtable_t *rtable_connect(const char *address_port){
    struct rtable_t *remote_table = malloc(sizeof(struct rtable_t));
    char *endpoint = malloc(sizeof(char)*strlen(address_port));
    strcpy(endpoint, address_port);
    strtok(endpoint, ":");
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
    MessageT *msg, *msg_received;

    msg = malloc(sizeof(MessageT));

    message_t__init(msg); 

    msg->opcode = MESSAGE_T__OPCODE__OP_PUT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_ENTRY;

    msg->n_entries = 1;
    msg->entries = malloc(sizeof(MessageT__Entry *));

    MessageT__Entry *entry_tmp;
    entry_tmp = malloc(sizeof(MessageT__Entry));
    message_t__entry__init(entry_tmp);
    entry_tmp->key = entry->key;
    entry_tmp->data.len = entry->value->datasize;
    entry_tmp->data.data = malloc(entry->value->datasize);
    entry_tmp->data.data = entry->value->data;
    

    msg->entries[0] = entry_tmp;

    

    if((msg_received = network_send_receive(rtable, msg))!= NULL){
        //printf("recebeu mensagem %d\n", msg_received->opcode);
        free(msg);
        free(entry_tmp);
        message_t__free_unpacked(msg_received, NULL);
        return 0;
    }
    free(msg);
    return -1;
}

struct data_t *rtable_get(struct rtable_t *rtable, char *key){
    struct data_t *data;
    if(rtable != NULL && key != NULL){
        MessageT *msg, *msg_received;

        msg = malloc(sizeof(MessageT));

        message_t__init(msg); 

        msg->opcode = MESSAGE_T__OPCODE__OP_GET;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
        msg-> n_keys = 1;
        msg->keys = malloc(sizeof(char*));
        msg->keys[0] = malloc(strlen(key)+1);
        strcpy(msg->keys[0], key);     
        msg_received = malloc(sizeof(MessageT));
        if((msg_received = network_send_receive(rtable, msg)) != NULL){
            printf("saiu do network_send_receive\n");
            free(msg);
            if(msg_received->data.len != 0){
                printf("entrou no if data existe\n");
                data = data_create(msg_received->data.len);
                memcpy(data->data, msg_received->data.data, msg_received->data.len);
            }
            else{
                printf("entrou no else\n");
                data = data_create(0);
                printf("data: %p\n", data);
            }
            message_t__free_unpacked(msg_received, NULL);
            return data;
        }
        free(msg);

    }
    return NULL;
}

int rtable_del(struct rtable_t *rtable, char *key){
    if(rtable != NULL && key != NULL){
        MessageT *msg, *msg_received;

        msg = malloc(sizeof(MessageT));

        message_t__init(msg); 

        msg->opcode = MESSAGE_T__OPCODE__OP_DEL;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
        msg->n_keys = 1;
        msg->keys = malloc(sizeof(char*));
        msg->keys[0] = malloc(strlen(key)+1);
        strcpy(msg->keys[0], key);

        if((msg_received = network_send_receive(rtable, msg)) != NULL){
            free(msg);
            if(msg_received->opcode == MESSAGE_T__OPCODE__OP_DEL + 1){
                printf("chave %s apagada com sucesso.\n", key);
                message_t__free_unpacked(msg_received, NULL);
                return 0;
            }
            if(msg_received->opcode == MESSAGE_T__OPCODE__OP_ERROR){
                message_t__free_unpacked(msg_received, NULL);
                printf("chave %s não existe.\n", key);
            }
        }
        free(msg);
        return -1;
    }
    return -1;
}

int rtable_size(struct rtable_t *rtable){
    if(rtable != NULL){
        MessageT *msg, *msg_received;
        msg = malloc(sizeof(MessageT));

        message_t__init(msg); 
        msg->opcode = MESSAGE_T__OPCODE__OP_SIZE;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

        //msg_received = malloc(sizeof(MessageT));
        //printf("vou chamar network_send_receive\n");
        //MessageT *msg_received = network_send_receive(rtable, msg);
        //printf("endereço da resposta em client_stub %p\n", msg_received);
        if((msg_received = network_send_receive(rtable, msg)) != NULL){
            //printf("saiu do network_send_receive\n");
            free(msg);
            //printf("fez unpack e retornou mensagem %d\n", msg_received->opcode);
            
            if(msg_received->opcode == 11){
                //printf("recebeu mensagem %d\n", msg_received->opcode);
                int result = msg_received->result;
                message_t__free_unpacked(msg_received, NULL);
                //passar result para variavel para poder fazer free ao msg_received
                return result;
            }
            
        }
        free(msg);
    }
    printf("rtable = NULL\n");
    return -1;
}

char **rtable_get_keys(struct rtable_t *rtable){
    char **keys;
    if(rtable != NULL){
        MessageT *msg, *msg_received;

        msg = malloc(sizeof(MessageT));

        message_t__init(msg);

        msg->opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

        msg_received = malloc(sizeof(MessageT));

        if((msg_received = network_send_receive(rtable, msg)) != NULL){
            if(msg_received->c_type == MESSAGE_T__C_TYPE__CT_KEYS){
                if(msg_received->n_keys == 0){
                    return NULL;
                }
                keys = malloc(sizeof(char)*msg_received->n_keys);
                
                //printf("msg_received->n_keys = %d\n", msg_received->n_keys);
                for(int i=0 ; i<msg_received->n_keys ; i++){
                    keys[i] = msg_received->keys[i];
                    //printf("%s\n", keys[i]);
                    //printf("msg_received->keys[%d] = %s\n", i, msg_received->keys[i]);
                    //strcpy(keys[i], msg_received->keys[i]);
                }
                //message_t__free_unpacked(msg_received, NULL);
                return keys;
            }
        }
        free(msg);
    }
    return NULL;
}

void rtable_free_keys(char **keys){ 
    printf("entrei no rtable_free_keys\n");
    if(keys != NULL){
        printf("entrei no if keys !=NULL\n");
        free(keys);
        //table_free_keys(keys);
    }
    printf("[ERROR] invalid argument keys\n");
}

void rtable_print(struct rtable_t *rtable){
    MessageT *msg, *msg_received;
    msg = malloc(sizeof(MessageT));
    message_t__init(msg); 
    
    msg->opcode = MESSAGE_T__OPCODE__OP_PRINT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

    msg_received = malloc(sizeof(MessageT));
    if((msg_received = network_send_receive(rtable, msg)) != NULL){
        if(msg_received->opcode == (MESSAGE_T__OPCODE__OP_PRINT + 1)){
            printf("msg_received->n_entries = %d\n", msg_received->n_entries);
            
            printf("remote_table: {\n");
            for(int i=0 ; i<msg_received->n_entries ; i++){
                printf("\tremote_entry_%d:[\n", i+1);
                printf("\t\tkey: %s,\n", (char*)msg_received->entries[i]->key);
                printf("\t\tdatasize: %d,\n",(int) msg_received->entries[i]->data.len);
                printf("\t\tdata: %s\n", (char*) msg_received->entries[i]->data.data);
                printf("\t]");
                if(i+1 != msg_received->n_entries){
                    printf(",\n");
                }
            }
            printf("}\n");
        }
    }
    free(msg);
    message_t__free_unpacked(msg_received, NULL);
}

