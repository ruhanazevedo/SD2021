#ifndef _MESSAGE_PRIVATE_H
#define _MESSAGE_PRIVATE_H

#include "../include/entry.h"
#include "../proto/sdmessage.pb-c.h"


/* Função que garante a leitura de len bytes armazenados em buf
 * através da connecao com uma socket sock
*/
int write_all(int sock, char *buf, int len);

/* Função que garante a escrita de len bytes armazenados em buf
 * através da connecao com uma socket sock
*/
int read_all(int sock, char *buf, int len);

#endif
