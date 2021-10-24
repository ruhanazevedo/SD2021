#include <stdio.h>
#include "../include/client/network_client.h"
#include "../include/extra/inet.h"
#include <errno.h>
#include "../include/client/client_stub-private.h"

static int sockfd;
static struct sockaddr_in server;

int network_connect(struct rtable_t *rtable){
    

    //int sockfd;
    //struct sockaddr_in server;
    char str[MAX_MSG];
    int count, nbytes;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket TCP");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(rtable->port);
    if (inet_pton(AF_INET, rtable->address, &server.sin_addr) < 1) {
        printf("Erro ao converter IP\n");
        close(sockfd);
        return -1;
    }
    
    if (connect(sockfd,(struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erro ao conectar-se ao servidor");
        close(sockfd);
        return -1;
    }
    
    printf("ligação feita\n");
    return 0;
}

struct MessageT *network_send_receive(struct rtable_t * rtable,
                                       struct MessageT *msg){
    int nbytes;
    char response[MAX_MSG];

    message_t__init(msg);
    int len = message_t__get_packed_size(msg);
    char*buf = malloc(len);
    sdmessage__pack(msg,buf);

    // Envia tamanho da mensagem
    if((nbytes = write(sockfd,len, 4)) != 4){
        perror("Erro ao enviar dados ao servidor");
        close(sockfd);
        return -1;
    }
    // Envia mensagem
    if((nbytes = write(sockfd,buf,len)) != len){
        perror("Erro ao enviar dados ao servidor");
        close(sockfd);
        return NULL;
    }
    
    if((nbytes = read(sockfd,&response,len)) != len){
        perror("Erro ao receber dados do servidor");
        close(sockfd);
        return -1;
    };
    MessageT *ds;
    ds = message_t__unpack(NULL, len, buf);
}   

int network_close(struct rtable_t * rtable){
    close(sockfd); // Fecha o socket
    printf("ligação terminada\n");
    return 0; // Termina
}

int main(int argc, char **argv){
    struct rtable_t *table = malloc(sizeof(struct rtable_t));
    table->address = "127.0.0.7";
    table->port = 5000;
    network_connect(table);
    network_send_receive(table, NULL);
    int test = network_close(table);
    printf("resultado: %d\n" ,test);
}