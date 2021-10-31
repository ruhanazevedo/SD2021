#include "../include/message.h"
#include "../include/extra/inet.h"
#include "../proto/sdmessage.pb-c.h"
#include <errno.h>



struct message_t *message_create() {

	//TODO se nao precisarem podemos apagar

	return;
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
