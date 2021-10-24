#include "message.h"
#include "../include/extra/inet.h"
#include "sdmessage.pb-c.h"
#include <errno.h>


/* aloca o espaço necessário a uma mensagem */
struct message_t *message_create() {

	struct message_t *msg = (struct message_t *) malloc(sizeof(struct message_t));

	if (msg == NULL) {
		printf("Error cant create message\n");
		return NULL;
	}
	
	return msg;
}

int message_to_buf(struct message_t *msg, char **msg_buf) {
	//TODO
	return -1;
}

/* Transforma uma mensagem no array de bytes, buffer, para
 * uma struct message_t*
 */
struct message_t *buf_to_message(char *msg_buf, int msg_size) {
	//TODO
	return NULL;
}


int write_all(int sock, char *buf, int len) {
	
	int bufsize = len; 
	while(len > 0) {
		int res = write(sock, buf, len); 
		if(res < 0) {
			if(errno==EINTR) 
				continue;
			perror("write failed");
			return res;
		} 
		buf+= res;
		len-= res;
	}
	return bufsize;
}


int read_all(int sock, char *buf, int len) {

	int bufsize = len;
	while (len > 0) {
		int res = read(sock, buf, len);
		if (res <= 0) {
			if (errno == EINTR)
				continue;
			perror("read failed");
			return res;
		}
		buf += res;
		len -= res;
	}
	return bufsize;
}
