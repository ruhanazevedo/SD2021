#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "../include/entry.h"
#include "../proto/sdmessage.pb-c.h"
#include "message-private.h"


struct message_t {

	MessageT *m;
};


/* aloca o espaço necessário a uma mensagem */
struct message_t *message_create();


/* Converte o conteúdo de uma message_t num char *, retornando o tamanho do
 * buffer alocado para a mensagem serializada como um array de bytes, ou -1
 * em caso de erro.
 * A mensagem serializada numa sequência de bytes, deve ter o seguinte
 * formato:
 *
 * OPCODE		C_TYPE		TABLE_NUM
 * [2 bytes]	[2 bytes]	[2 bytes]
 *  
 * a partir daí, o formato difere para cada tipo de conteúdo (c_type):
 * CT_ENTRY:		KEYSIZE(KS)		KEY 		DATASIZE(DS)	DATA
 * 					 [2 bytes]	[KS bytes]		[4 bytes]	 [DS bytes]
 * CT_KEY:		KEYSIZE(KS)		KEY 		
 *				[2 bytes]	[KS bytes]
 * CT_KEYS:		NKEYS			KEYSIZE(KS)		KEY		...
 *			  [4 bytes]			[2 bytes]	[KS bytes]	...
 * CT_VALUE:	DATASIZE(DS)		DATA
 *				 [4 bytes]		 [DS bytes]
 * CT_RESULT:		RESULT
 *				   [4 bytes]
 *
 * Notar que o `\0´ no fim da string e o NULL no fim do array de
 * chaves não são enviados nas mensagens.
 */
int message_to_buffer(struct message_t *msg, char **msg_buf);

/* Transforma uma mensagem no array de bytes, buffer, para
 * uma struct message_t*
 */
struct message_t *buffer_to_message(char *msg_buf, int msg_size);

/* Liberta a memoria alocada na função buffer_to_message
 */
void free_message(struct message_t *msg);

#endif
