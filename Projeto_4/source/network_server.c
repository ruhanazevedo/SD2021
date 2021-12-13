/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/table_skel.h"
#include "../include/table_skel-private.h"
#include "network_server.h"
#include "extra/inet.h"
#include "message.c"
#include <pthread.h>
#include "time.h"


int network_server_init(short port) {

	int sockfd;
	struct sockaddr_in server;

	// Cria socket TCP
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error creating socket!\n");
		return -1;
	}

	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
    	perror("setsockopt(SO_REUSEADDR) failed");
	}
	
	// Preenche estrutura server com endereço(s) para associar (bind) à socket
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	// Faz bind
	if (bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("Error making bind!\n");
		close(sockfd);
		return -1;
	}
	
    // Esta chamada diz ao SO que esta é uma socket para receber pedidos
	if (listen(sockfd, 5) < 0) {
		perror("Erroe executing listen\n");
		close(sockfd);
		return -1;
	}
	
	return sockfd;
}


void *thread_job(void *params){
	int *aux = (int*) params;
	int connsockfd = *aux;
	MessageT *msg;

	while((msg = network_receive(connsockfd)) != NULL){
		clock_t t;
		t = clock();

		if(msg == NULL){
			printf("message received is null on network_server\n");
			//return -1;
		}
		
		if (invoke(msg) == -1) {
			printf("Error in network server invoke\n");
			free(msg);
			//return -1;
		}
			
		if (network_send(connsockfd, msg) == -1) {
			printf("Error in network server send\n");
			free(msg);
			//return -1;
		}
		if((msg->opcode != MESSAGE_T__OPCODE__OP_STATS+1) || (msg->opcode == MESSAGE_T__OPCODE__OP_ERROR)){
			setStatsAVGTime(t);
		}
		
	}	
	return NULL;
}


int network_main_loop(int listening_socket) {
	
	struct sockaddr_in client;
    socklen_t client_size;
    int connsockfd;

    // Bloqueia a espera de pedidos de conexão
	printf("Servidor na escuta\n");
    while ((connsockfd = accept(listening_socket, (struct sockaddr *) &client, &client_size)) != -1) {
		
		pthread_t nova;

		if (pthread_create(&nova, NULL, &thread_job, (void *) &connsockfd) != 0){
			printf("\nFalha na criação da Thread\n");
			exit(EXIT_FAILURE);
		}
		if (pthread_detach(nova) != 0){
			printf("\nErro na chamada do thread_detach\n");
		}
        // Fecha socket referente a esta conexão
		//close(connsockfd);
    }
	perror("O servidor encerrou de maneira inesperada, tente novamente\n");

	return 0;
}


MessageT *network_receive(int client_socket) {
	MessageT *msg = NULL;
	int result, msgsizeAux;
   	void *buf;

	if (client_socket < 0) {
		return NULL;
	}

	// Recebe o tamanho da mensagem
	result = read_all(client_socket, &msgsizeAux, sizeof(int));
	if (result != sizeof(msgsizeAux)) {
		printf("Error in read_all message size result = %d\n", result);
		close(client_socket);
		return NULL;
	}
	
	uint32_t msgsize = ntohl(msgsizeAux);
	buf = malloc(msgsize);
	
	// Recebe a mensagem
	if ((result = read_all(client_socket, buf, msgsize)) != msgsize) {
		printf("Perdeu conexão com o cliente %d\n", result);
		close(client_socket);
		free(buf);
		return NULL;
	}
	printf("Recebendo mensagem\n");
	msg = message_t__unpack(NULL, msgsize, buf);

	// Verificar se a deserializacao teve sucesso 
	if (msg == NULL) {
		printf("error unpacking message\n");
		free(buf);
		return NULL;
	}
	free(buf);
	//message_t__free_unpacked(msg, NULL);
	return msg;
}


int network_send(int client_socket, MessageT *msg) {
	
	int msgsize, result, msgsizeAux;
	unsigned char *buf;

	if (client_socket < 0 || msg == NULL) {
		return -1;
	}
	
	msgsizeAux = message_t__get_packed_size(msg);
    buf = malloc(msgsizeAux);
    if (buf == NULL) {
        fprintf(stdout, "malloc error\n");
        return 1;
    }

    message_t__pack(msg, buf);

	//Verifica se a serializacao teve sucesso 
	if (msgsizeAux < 0) {
		free(buf);
		return -1;
	}
	
	msgsize = htonl(msgsizeAux);
	//envia tamanho da mensagem 
	if ((result = write_all(client_socket, &msgsize, sizeof(int))) != sizeof(int)) {
		perror("Error in write_all\n");
		close(client_socket);
		free(buf);
		return -1;
	}

	
	//envia mensagem
	if ((result = write_all(client_socket, buf, msgsizeAux)) != msgsizeAux) {
		perror("Error in write_all\n");
		close(client_socket);
		free(buf);
		return -1;
	}

	
	free(buf);
	free(msg);
	return 0;
}


int network_server_close() {

	table_skel_destroy();
	zookeeper_close();
	return 0;
}


