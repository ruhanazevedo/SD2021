/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/message.h"
#include "../include/extra/inet.h"
//#include "sdmessage.pb-c.h"
#include <errno.h>

int write_all(int sock, void *buf, int len) {
	
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


int read_all(int sock, void *buf, int len) {
	printf("reading value\n");

	int bufsize = len;
	while (len > 0) {
		int res = read(sock, buf, len);
		printf("sock = %d\n", sock);
		printf("buf = %p\n", buf);
		printf("len = %d\n", len);
		printf("res = %d\n", res);
		if (res <= 0) { //EOF n é erro
			if (errno == EINTR)
				continue;
			perror("read failed");
			return res;
		}
		buf += res;
		len -= res;
	}
	printf("bufsize = %s\n", bufsize);
	return bufsize;
}
