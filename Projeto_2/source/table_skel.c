//#include "../proto/sdmessage.pb-c.h"
#include "../sdmessage.pb-c.h"
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


int invoke(MessageT *msg) {

	if (msg == NULL || table == NULL || msg->opcode < 10 || msg->c_type < 10 || 
		msg->opcode >= 99 || msg->c_type >= 50) {
		return -1;
	}

	if (msg->opcode == MESSAGE_T__OPCODE__OP_SIZE && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
		msg->result = table_size(table);
		return 0;
	}

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_DEL && msg->c_type == MESSAGE_T__C_TYPE__CT_KEY) {	
		msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
		if ((table_del(table, msg->key)) == 0) {
			msg->opcode += 1;
			return 0;
		} else {
			msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return -1;
		}

	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_GET && msg->c_type == MESSAGE_T__C_TYPE__CT_KEY) {

		struct data_t *data = table_get(table, msg->key);
		if (data == NULL) {
			msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return -1;
		} else {
<<<<<<< HEAD
			msg->m->opcode += 1;
			msg->m->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
			//msg->m->data = data->data;
			memcpy(msg->m->data.data, data->data, data->datasize);
			//msg->m->data_size = data->datasize;
			memcpy(msg->m->data.len, data->datasize, sizeof(int));
=======
			msg->opcode += 1;
			msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
			msg->data2 = data->data;
			msg->data_size = data->datasize;
>>>>>>> main
			return 0;

		}
	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_PUT && msg->c_type == MESSAGE_T__C_TYPE__CT_ENTRY) {
		
<<<<<<< HEAD
		if ((table_put(table, msg->m->key, data_create2(msg->m->data_size,msg->m->data.data))) == 0) {
			msg->m->opcode += 1;
			msg->m->c_type = MESSAGE_T__C_TYPE__CT_NONE;
=======
		if ((table_put(table, msg->key, data_create2(msg->data_size, msg->data2))) == 0) {
			msg->opcode += 1;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
>>>>>>> main
			return 0;
		} else {
			msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return -1;
		}

	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_GETKEYS && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
		msg->keys = table_get_keys(table);
		return 0;
	} 

<<<<<<< HEAD
	else if (msg->m->opcode == MESSAGE_T__OPCODE__OP_PRINT && msg->m->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->m->opcode += 1;
		msg->m->c_type = MESSAGE_T__C_TYPE__CT_TABLE;
		table_print(table);
=======
	else if (msg->opcode == MESSAGE_T__OPCODE__OP_PRINT && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;
		msg->entries = table_get_entries(table);
>>>>>>> main
		return 0;
	}

	return -1;
}
