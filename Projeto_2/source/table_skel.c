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
		msg->opcode > 99 || msg->c_type > 70) {
		if(msg == NULL){
			printf("RH2\n");
		}else if(table == NULL){
			printf("RH3\n");
		}else if(msg->opcode < 10){
			printf("RH4\n");
		}else if(msg->c_type < 10){
			printf("RH5\n");
		}else if(msg->opcode > 99){
			printf("RH6\n");
		}else if(msg->c_type > 70){
			printf("RH7\n");
		}
		return -1;
	}
	printf("RH1\n");
	if (msg->opcode == MESSAGE_T__OPCODE__OP_SIZE && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
		msg->result = table_size(table);
		printf("RH2\n");
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
			msg->opcode += 1;
			msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
			msg->data2 = data->data;
			msg->data_size = data->datasize;
			return 0;

		}
	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_PUT && msg->c_type == MESSAGE_T__C_TYPE__CT_ENTRY) {
		if ((table_put(table, msg->key, data_create2(msg->data_size, msg->data2))) == 0) {
			msg->opcode += 1;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
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

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_PRINT && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;
		struct entry_t **entries = malloc(sizeof(struct entry_t)*table->size + sizeof(NULL)); 
        int k = 0;
        for(int i=0 ; i<table->nListas ; i++){
            if(table->list[i] != NULL){
                int listSize = list_size(table->list[i]); 
                char **list_entries = list_get_entrys(table->list[i]); 
                for(int j=0 ; j<listSize ; j++){
                    if(list_entries[j] != NULL){
                        entries[k] =  list_entries[j];
                        ++k;
                    }
                }
            }
        }
        entries[k] = NULL; //talvez possamos tirar isso
		msg->n_entries = k;
        msg->entries = entries;
		return 0;
	}

	/*else if (msg->opcode == MESSAGE_T__OPCODE__OP_PRINT && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;
		msg->entries = table_get_entries(table);
		return 0;
	}*/

	return -1;
}

