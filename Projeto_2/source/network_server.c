#include "table_skel.h"
#include "network_server.h"
#include "inet.h"


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
	
	struct sockaddr_in client;
    	socklen_t client_size;
    	int connsockfd;

    	// Bloqueia a espera de pedidos de conexão
    	while ((connsockfd = accept(listening_socket,(struct sockaddr *) &client, &client_size)) != -1) {

        	struct message_t * msg = network_receive(connsockfd);

   		if (invoke(msg) == -1) {
   			printf("Error in network_main_loop\n");
			free(msg);
   			return -1;
   		}
   		if (network_send(connsockfd, msg) == -1) {
   			printf("Error in network_main_loop\n");
			free(msg);
   			return -1;
   		}
        	// Fecha socket referente a esta conexão
		close(connsockfd);
    	}
    	return 0;
}


struct message_t *network_receive(int client_socket) {
	
   	char *buf;
	int res, msgsize, msgsizeAux;
	struct message_t *msg;

	// recebe o tamanho da mensagem
	if ((res = read_all(client_socket, (char *)&msgsizeAux, sizeof(int))) < 0) {
		perror("Error in read_all\n");
		close(client_socket);
		return NULL;

	}

	msgsize = ntohl(msgsizeAux);
	msg = (struct message_t *) malloc(msgsize);
	buf = (char *) malloc(sizeof(char) *msgsize);

	// recebe a mensagem e coloca no buf
	if ((res = read_all(client_socket, buf, msgsize)) != msgsize) {
		perror("Erro in read_all\n");
		close(client_socket);
		free(buf);
		free(msg);
		return NULL;
	}

	// De-serializar a mensagem do pedido
	msg = buf_to_message(buf, msgsize);

	// Verificar se a de-serializacao teve sucesso 
	if (msg == NULL) {
		free(buf);
		free(msg);
		return NULL;
	}
	
	free(buf);
	return msg;
}


int network_send(int client_socket, struct message_t *msg) {

	char *buf;
	int msgsize, result, msgsizeAux;

    	//Serializa a mensagem 
	msgsizeAux = message_to_buf(msg, &buf);

	//Verifica se a serializacao teve sucesso 
	if (msgsizeAux < 0) {
		free(buf);
		free(msg);
		return -1;
	}

	//Verifica se o envio teve sucesso
	msgsize = htonl(msgsizeAux);
	if (((result = write_all(client_socket, (char *)&msgsize, sizeof(int))) < 0) || ((result = write_all(client_socket, buf, msgsizeAux)) != msgsizeAux)) {
		perror("Error in write_all\n");
		close(client_socket);
		free(buf);
		free(msg);
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

