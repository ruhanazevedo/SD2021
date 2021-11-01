/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/table_skel.h"
#include "network_server.h"
#include "extra/inet.h"
//#include "../include/message.h"
#include "message.c"


int network_server_init(short port) {

	int sockfd;
	struct sockaddr_in server;

	// Cria socket TCP
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error creating socket!\n");
		return -1;
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
	if (listen(sockfd, 0) < 0) {
		perror("Erroe executing listen\n");
		close(sockfd);
		return -1;
	}
	
	return sockfd;
}


int network_main_loop(int listening_socket) {
	
	struct sockaddr_in *client;
    socklen_t client_size;
    int connsockfd;

    // Bloqueia a espera de pedidos de conexão

    while ((connsockfd = accept(listening_socket, &client, &client_size)) != -1) {

        MessageT *msg = network_receive(connsockfd);
		if(msg == NULL){
			printf("msg null network_server\n");
		}
   		if (invoke(msg) == -1) {
   			printf("Error in network_main_loop 1\n");
			free(msg);
   			return -1;
   		}
   		if (network_send(connsockfd, msg) == -1) {
   			printf("Error in network_main_loop 2\n");
			free(msg);
   			return -1;
   		}
        // Fecha socket referente a esta conexão
		close(connsockfd);
		//message_t__free_unpacked(msg, NULL);
    }
	printf("morreu\n");

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
		perror("Error in read_all\n");
		close(client_socket);
		return NULL;
	}
	uint32_t msgsize = ntohl(msgsizeAux);
	//msg = (MessageT *) malloc(msgsize);
	buf = malloc(msgsize);
	
	// Recebe a mensagem
	if ((result = read_all(client_socket, buf, msgsize)) != msgsize) {
		perror("Error in read_all\n");
		close(client_socket);
		free(buf);
		return NULL;
	}
	msg = message_t__unpack(NULL, msgsize, buf);

	// Verificar se a deserializacao teve sucesso 
	if (msg == NULL) {
		fprintf(stdout, "error unpacking message\n");
		free(buf);
		return NULL;
	}
	free(buf);
	//message_t__free_unpacked(msg, NULL);
	return msg;
}


int network_send(int client_socket, struct MessageT *msg) {
	
	int msgsize, result, msgsizeAux;
	char *buf;

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
	printf("RH2\n");
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
	return 0;
}

