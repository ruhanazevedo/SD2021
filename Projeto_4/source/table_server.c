/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "network_server.h"
#include "message.h"


int testInput(int argc){
    if (argc != 3){

		printf("Uso: ./table_server <porto _servidor> <size_table> \n");
    	printf("Exemplo de uso: ./table_server 12345 30\n");
        return -1;
    } 
    return 0;
}

int main(int argc, char **argv) {
	
	int sockfd, port;

	if (testInput(argc) < 0) return -1;

	port = atoi(argv[1]);
	sockfd = network_server_init(port);
	
	if (sockfd == -1) {
		printf("Error creating server\n");
		return -1;
	}	

	int n_lists = atoi(argv[2]);

	//table_skel_init(n_lists);
	int filler = table_skel_mapping(n_lists, "127.0.0.1", port);
	if (filler == 0){
		int result = network_main_loop(sockfd);

		if (result == -1) {
			printf("Error in main loop\n");
			return -1;
		}
	}
	if (filler == -2){
		printf("já tavam feito *shrugs*\n");
	}

	int network_server_close();

  	return 0;
}
