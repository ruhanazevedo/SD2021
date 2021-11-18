/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/sdmessage.pb-c.h"
//#include "table.c"
#include "../include/table_skel.h"
#include "../include/table-private.h"
#include "../include/message.h"
#include "../include/list-private.h"
#include <stddef.h> //NULLS
#include <stdio.h>

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
		msg->opcode > 99 || msg->c_type > 70) {
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
		printf("key: %s\n", msg->keys[0]);
		printf("vou entrar no table_del\n");
		if ((table_del(table, msg->keys[0])) == 0) {
			msg->opcode += 1;
			return 0;
		} else {
			msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return 0;
		}

	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_GET && msg->c_type == MESSAGE_T__C_TYPE__CT_KEY) {
		struct data_t *data = table_get(table, msg->keys[0]);
		if (data == NULL) {
			msg->opcode += 1;
			msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
			msg->data.data = NULL;
			msg->data.len = 0;
			return 0;
		} else {
			msg->opcode += 1;
			msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
			msg->data.data = data->data;
			msg->data.len = data->datasize;
			return 0;

		}
	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_PUT && msg->c_type == MESSAGE_T__C_TYPE__CT_ENTRY) {
		printf("menssagem recebida\n");
		printf("key %s\n", msg->entries[0]->key);
		printf("datasize %ld\n", msg->entries[0]->data.len);
		printf("data %s\n", msg->entries[0]->data.data);
		if ((table_put(table, msg->entries[0]->key, data_create2(msg->entries[0]->data.len, msg->entries[0]->data.data))) == 0) {
			table_print(table);
			msg->opcode += 1;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return 0;
		} else {
			msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			return 0;
		}

	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_GETKEYS && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
		msg->keys = table_get_keys(table);
		msg->n_keys = sizeof(msg->keys);
		return 0;
	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_PRINT && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;

		MessageT__Entry **msg_entries;
		msg_entries = malloc(table->nListas * sizeof(MessageT__Entry));
		int numEntries = 0;
		int k = 0;
        for(int i=0 ; i<table->nListas ; i++){
            if(table->list[i] != NULL){
                int nEntries = list_size(table->list[i]);
				numEntries = nEntries;
                struct entry_t **list_entries = list_get_entrys(table->list[i]); 
				if(nEntries > 0){
					for(int j=0 ; j<nEntries ; j++){
						if(list_entries[j] != NULL){
							msg_entries[k] = malloc(sizeof(MessageT__Entry));
							message_t__entry__init(msg_entries[k]);
							msg_entries[k]->key = list_entries[j]->key;
							msg_entries[k]->data.data = list_entries[j]->value->data;
							msg_entries[k]->data.len = list_entries[j]->value->datasize;
							k+=1;
						}
					
                	}
				}
            }
        }
		msg->n_entries = k;
		msg->entries = msg_entries;
		return 0;
	}

	return -1;
}

