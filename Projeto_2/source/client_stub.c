#include "../include/client/client_stub.h"
#include "../include/client/client_stub-private.h"
#include "../proto/sdmessage.pb-c.h"
#include <stddef.h> //NULLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/serialization.h"
#include "../include/extra/inet.h"

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
    remote_table->server = malloc(sizeof(struct sockaddr_in *));

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
    struct data_t *data;
    if(rtable != NULL && strcmp(key, NULL) != 0){
        struct MessageT *msg, *msg_received;
        char *buf;
        unsigned len;
        message_t__init(&msg); 

        msg->opcode = MESSAGE_T__OPCODE__OP_GET;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
        msg->data_size = strlen(key);
        msg->data = key;

        len = message_t__get_packed_size(&msg);
        buf = malloc(len);

        message_t__pack(&msg, buf);        

        if(msg_received = network_send_receive(rtable, &msg) != NULL){
            char *received_buf;
            message_t__pack_to_buffer(msg_received, received_buf); 
            if(data = buffer_to_data(received_buf, sizeof(received_buf)) == 0){ //putting message response of network into a buffer to use buffer_to_data, comparing with 0 suposing that return 0 is OK
                return data;
            }
        }

    }
    return NULL;
}

int rtable_del(struct rtable_t *rtable, char *key){
    if(rtable != NULL && strcmp(key, NULL) != 0){
        struct MessageT *msg;
        char *buf;
        unsigned len;
        message_t__init(&msg); 

        msg->opcode = MESSAGE_T__OPCODE__OP_DEL;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
        msg->data_size = strlen(key);
        msg->data = key;

        if(network_send_receive(rtable, &msg) != NULL){
            return 0;
        }
        return -1;
    }
    return -1;
}

int rtable_size(struct rtable_t *rtable){
    if(rtable != NULL){
        struct MessageT *msg, *msg_received;
        char *buf;
        unsigned len;
        message_t__init(&msg); 

        msg->opcode = MESSAGE_T__OPCODE__OP_SIZE;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
        msg->data_size = 0;
        msg->data = NULL;

        if(msg_received = network_send_receive(rtable, &msg) != NULL){
            //im waiting for size into msg_received->data_size
            //return msg_received->data_size;

            //treating msg_received as a buffer
            int dataSize = msg_received->data_size;
            int *res = malloc(dataSize);
            memcpy(res, msg_received->data, dataSize);

            return &res;
        }
    }
    return -1;
}

char **rtable_get_keys(struct rtable_t *rtable){
    if(rtable != NULL){
        //TODO TRICKY
        struct MessageT *msg, *msg_received;
        char *buf;
        unsigned len;
        message_t__init(&msg);

        msg->opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
        msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;
        msg->data_size = 0; //don't need to send table in the msg, just the network need's to know
        msg->data = NULL;

        if(msg_received = network_send_receive() != NULL){
            int dataSize = msg_received->data_size;
            char **res = malloc(dataSize);
            memcpy(*res, msg_received->data, dataSize);

            return res;
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