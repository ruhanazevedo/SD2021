/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include <stdio.h>
#include "../include/network_client.h"
#include "../include/extra/inet.h"
#include <errno.h>
#include "../include/client_stub-private.h"
#include "message.c"


int network_connect(struct rtable_t *rtable){
    
    if (connect(rtable->sockfd, (struct sockaddr *) rtable->server, sizeof(struct sockaddr_in)) < 0) {
        perror("Erro ao conectar-se ao servidor");
        close(rtable->sockfd);
        return -1;
    }
    
    printf("ligação feita\n");
    return 0;
}

MessageT *network_send_receive(struct rtable_t * rtable,
                                       MessageT *msg){
    int nbytes, msgsize, msgsizeAux;
    
    int len = message_t__get_packed_size(msg);
    printf("tamanho de MessageT: %d\n", len);
    unsigned char *buf, *bufAux;
    buf = malloc(len);
    
    message_t__pack(msg, buf);
    
    // Envia tamanho da mensagem
    uint32_t network_byte_order = htonl(len); 
    if((nbytes = write_all(rtable->sockfd, &network_byte_order, sizeof(len))) != sizeof(len)){
        perror("Erro ao enviar dados ao servidor");
        close(rtable->sockfd);
        return NULL;
    }
    
    // Envia mensagem
    if((nbytes = write_all(rtable->sockfd, buf, len)) != len){
        perror("Erro ao enviar dados ao servidor");
        close(rtable->sockfd);
        return NULL;
    }
    
    free(buf);
     
    //recebe o tamanho da mensagem
    if((nbytes = read_all(rtable->sockfd, &msgsizeAux, sizeof(int))) != sizeof(int)){
        perror("Erro ao receber dados do servidor");
        close(rtable->sockfd);
        return NULL;
    };

    msgsize = ntohl(msgsizeAux);

    bufAux = malloc(msgsize);

    //recebe mensagem
    if((nbytes = read_all(rtable->sockfd, bufAux, msgsize)) != msgsize){
        perror("Erro ao receber dados do servidor");
        close(rtable->sockfd);
        return NULL;
    };
    
    MessageT *res = message_t__unpack(NULL, msgsize, bufAux);

    free(bufAux);

    //printf("endereço da resposta em network_client %p\n", res);
    printf("fez unpack e retornou mensagem %d\n", res->opcode);
    if(res == NULL){
        printf("error unpacking message\n");
        return NULL;
    }
    else {
        return res;
    }
    
}   

int network_close(struct rtable_t * rtable){
    close(rtable->sockfd); // Fecha o socket
    printf("ligação terminada\n");
    return 0; // Termina
}