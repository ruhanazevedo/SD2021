#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "../include/network_server.h"
#include "../include/message.h"


int testInput(int argc){
    if (argc != 3){

	printf("Uso: ./server_exemplo <porto _servidor> <size_table> \n");
    	printf("Exemplo de uso: ./server_exemplo 12345 30\n");
        return -1;
    } 
    return 0;
}

int main(int argc, char **argv) {
	
	int sockfd, port;

	if (testInput(argc) < 0) return -1;

	port = atoi(argv[1]);
	sockfd = network_server_init(port);

	table_skel_init(argv[2]);
	int result = network_main_loop(sockfd);

	int network_server_close();

  	return 0;
}
