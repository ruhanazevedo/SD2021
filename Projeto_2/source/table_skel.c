#include "../proto/sdmessage.pb-c.h"
#include "table.h"
#include "table_skel.h"
#include "../include/message.h"
#include <stddef.h> //NULLS

struct table_t *table;


int table_skel_init(int n_lists) {

	table = table_create(n_lists);
	if (table == NULL) {
		return -1;
	}
	return 0;
}


void table_skel_destroy() {
	table_destroy(table);
}


int invoke(struct message_t *msg) {

	if (msg == NULL || table == NULL || msg->m->opcode < 10 || msg->m->c_type < 10 || 
		msg->m->opcode >= 99 || msg->m->c_type >= 50) {
		return -1;
	}

	if (msg->m->opcode == MESSAGE_T__OPCODE__OP_SIZE && msg->m->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->m->opcode += 1;
		msg->m->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
		msg->m->result = table_size(table);
		return 0;
	}

	else if (msg->m->opcode == MESSAGE_T__OPCODE__OP_DEL && msg->m->c_type == MESSAGE_T__C_TYPE__CT_KEY) {	
		msg->m->c_type = MESSAGE_T__C_TYPE__CT_NONE;
		if ((table_del(table, msg->m->key)) == 0) {
			msg->m->opcode += 1;
			return 0;
		} else {
			msg->m->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->m->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return -1;
		}

	} 

	else if (msg->m->opcode == MESSAGE_T__OPCODE__OP_GET && msg->m->c_type == MESSAGE_T__C_TYPE__CT_KEY) {

		struct data_t *data = table_get(table, msg->m->key);
		if (data == NULL) {
			msg->m->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->m->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return -1;
		} else {
			msg->m->opcode += 1;
			msg->m->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
			msg->m->data = data->data;
			msg->m->data_size = data->datasize;
			return 0;

		}
	} 

	else if (msg->m->opcode == MESSAGE_T__OPCODE__OP_PUT && msg->m->c_type == MESSAGE_T__C_TYPE__CT_ENTRY) {
		
		if ((table_put(table, msg->m->key, data_create2(msg->m->data_size,msg->m->data))) == 0) {
			msg->m->opcode += 1;
			msg->m->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return 0;
		} else {
			msg->m->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->m->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return -1;
		}

	} 

	else if (msg->m->opcode == MESSAGE_T__OPCODE__OP_GETKEYS && msg->m->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->m->opcode += 1;
		msg->m->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
		msg->m->keys = table_get_keys(table);
		return 0;
	} 

	else if (msg->m->opcode == MESSAGE_T__OPCODE__OP_PRINT && msg->m->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->m->opcode += 1;
		msg->m->c_type = MESSAGE_T__C_TYPE__CT_TABLE;
		table_print(table);
		return 0;
	}

	return -1;
}
