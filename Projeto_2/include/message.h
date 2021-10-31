#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "../include/entry.h"
#include "../proto/sdmessage.pb-c.h"
#include "message-private.h"


struct message_t {

	MessageT *m;
};


/* aloca o espaço necessário a uma mensagem */ 	//TODO se nao precisarem podemos apagar
struct message_t *message_create();

/* Liberta a memoria alocada na função buffer_to_message */ 	//TODO se nao precisarem podemos apagar
void free_message(struct message_t *msg);

#endif
