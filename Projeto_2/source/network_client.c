#include <stdio.h>
#include "../include/client/network_client.h"
//#include "../include/inet.h"
#include "../include/extra/inet.h"
#include <errno.h>
#include "../include/client/client_stub-private.h"


int network_connect(struct rtable_t *rtable){
    

    int sockfd;
    struct sockaddr_in server;
    char str[MAX_MSG];
    int count, nbytes;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket TCP");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &server.sin_addr) < 1) {
        printf("Erro ao converter IP\n");
        close(sockfd);
        return -1;
    }
}

struct message_t *network_send_receive(struct rtable_t * rtable,
                                       struct message_t *msg);

int network_close(struct rtable_t * rtable);                                       