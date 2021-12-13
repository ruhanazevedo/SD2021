/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#include "../include/sdmessage.pb-c.h"
#include "../include/table_skel.h"
#include "../include/table-private.h"
#include "../include/message.h"
#include "../include/list-private.h"
#include "../include/stats-private.h"
#include "../include/table_skel-private.h"
#include <stddef.h> //NULLS
#include <stdio.h>
#include <pthread.h>
#include <zookeeper/zookeeper.h>


struct table_t *table;
pthread_mutex_t m_table = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_stats = PTHREAD_MUTEX_INITIALIZER;
struct statistics *stats;
static zhandle_t *zh;
char *zk_server_endpoint;

void insert_on_backup(const char *buffer, int buflen){
	zoo_set(zh, "/kvstore/backup", buffer, buflen, 0);
}

void watcher_function(zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx){
	printf("\n\nteste watcher func\n\n");
	
	if (ZOK != zoo_wget_children(zh, "/kvstore/primary", watcher_function, watcherCtx, NULL)) {
 		fprintf(stderr, "Error setting watch at %s!\n", "/kvstore/primary");
	}

	int primary = zoo_exists(zh, "/kvstore/primary", 0, NULL);
	int backup = zoo_exists(zh, "/kvstore/backup", 0, NULL);
	if(primary == ZNONODE && backup == ZOK){
		printf("\n\entrou watcher func\n\n");
		char *server_endpoint[64];
		int bufferlen1 = sizeof(server_endpoint);

		int flag1 = zoo_get (zh, "/kvstore/backup", 0,
						server_endpoint, & bufferlen1, NULL);
		zoo_delete(zh, "/kvstore/backup", -1);
		zoo_create(zh, "/kvstore/primary", server_endpoint, strlen(server_endpoint)+1, & ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
	}
}

int table_skel_mapping(int n_lists, char *zk_endpoint, char *server_port){
	zh = zookeeper_init(zk_endpoint, watcher_function, 2000, 0, NULL, 0);
	if(zh == NULL){
		fprintf(stderr, "[ERROR] Error on connection attempt to zookeeper server\n");
		exit(EXIT_FAILURE);
	}
	printf("\n\nRH3\n\n");
	if (zoo_exists(zh, "/kvstore", 0, NULL) == ZNONODE) { //primario
		printf("\n\nexiste?\n\n");
		zoo_create(zh, "/kvstore", NULL, -1, & ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0);
	}
	printf("\n passou if kvstore \n");
	int primary = zoo_exists(zh, "/kvstore/primary", 0, NULL);
	int backup = zoo_exists(zh, "/kvstore/backup", 0, NULL);
	if(primary == ZNONODE){ 
		printf("\n entrou no if primary \n");
		if(backup == ZOK){
			printf("\n entrou no if backup \n");

			printf("\n oh boy! este caso é mito fixe :^) \n");
			return; //CONTINUE
		}
		else{
			printf("\n passou no if backup \n");

			printf("criar node com port = %s \n", server_port);
			char *local_host = calloc(1, sizeof(char));
			strcat(local_host, "127.0.0.1:");
			printf("local_host1 = %s\n\n", local_host);
			strcat(local_host,server_port);
			printf("local_host2 = %s\n\n", local_host);
			zoo_create(zh, "/kvstore/primary", local_host, strlen(local_host)+1, & ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
			zk_server_endpoint = local_host;
			printf("\n\nzk_server_endpoint = %s\n\n", zk_server_endpoint);
		}	
		printf("\n primary criado \n");
	}
	else if(backup == ZNONODE){
		char *local_host = calloc(1, sizeof(char));
		strcat(local_host, "127.0.0.1:");
		printf("local_host1 = %s\n\n", local_host);
		strcat(local_host,server_port);
		printf("local_host2 = %s\n\n", local_host);
		zoo_create(zh, "/kvstore/backup", local_host, strlen(local_host)+1, & ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
		zk_server_endpoint = local_host;
	}
	else {
		printf("já há primary e backup, job done\n");
		return -2;
	}
		

	printf("\nparte de zookeeper feita, vou passar para o table_skel_init \n");

	return table_skel_init(n_lists);
}

int table_skel_init(int n_lists) {

	table = table_create(n_lists);

	if (table == NULL) {
		return -1;
	}

	stats = malloc(sizeof(struct statistics));
	stats->avg_time = 0;
	stats->n_del = 0;
	stats->n_get = 0;
	stats->n_getkeys = 0;
	stats->n_put = 0;
	stats->n_size = 0;
	stats->n_table_print = 0;

	return 0;
}

void table_skel_destroy() {

	table_destroy(table);
	zookeeper_close(zh);

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

		pthread_mutex_lock(&m_stats);

		stats->n_size +=1;

		pthread_mutex_unlock(&m_stats);

		return 0;
	}
	else if (msg->opcode == MESSAGE_T__OPCODE__OP_DEL && msg->c_type == MESSAGE_T__C_TYPE__CT_KEY) {
		pthread_mutex_lock(&m_table);
		msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
		if ((table_del(table, msg->keys[0])) == 0) {
			msg->opcode += 1;
			pthread_mutex_unlock(&m_table);
			
			pthread_mutex_lock(&m_stats);
			stats->n_del +=1;
			pthread_mutex_unlock(&m_stats);
			return 0;
		} else {
			msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			pthread_mutex_unlock(&m_table);
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
			pthread_mutex_lock(&m_stats);
			stats->n_get +=1;
			pthread_mutex_unlock(&m_stats);
			return 0;
		} else {
			msg->opcode += 1;
			msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
			msg->data.data = data->data;
			msg->data.len = data->datasize;
			pthread_mutex_lock(&m_stats);
			stats->n_get +=1;
			pthread_mutex_unlock(&m_stats);
			return 0;

		}
	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_PUT && msg->c_type == MESSAGE_T__C_TYPE__CT_ENTRY) {
		pthread_mutex_lock(&m_table);

		if ((table_put(table, msg->entries[0]->key, data_create2(msg->entries[0]->data.len, msg->entries[0]->data.data))) == 0) {
			msg->opcode += 1;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			pthread_mutex_unlock(&m_table);
			pthread_mutex_lock(&m_stats);
			stats->n_put +=1;
			pthread_mutex_unlock(&m_stats);
			return 0;
		} else {
			msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
			msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
			pthread_mutex_unlock(&m_table);
			return 0;
		}
		
	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_GETKEYS && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
		msg->keys = table_get_keys(table);
		int k = 0;
        for(int i=0 ; i<table->nListas ; i++){
            if(table->list[i] != NULL){
                int nEntries = list_size(table->list[i]);
				
                struct entry_t **list_entries = list_get_entrys(table->list[i]); 
				if(nEntries > 0){
					for(int j=0 ; j<nEntries ; j++){
						if(list_entries[j] != NULL){
							msg->keys[k] = list_entries[j]->key;
							k+=1;
						}
                	}
				}
            }
        }
		
		msg->n_keys = k;
		pthread_mutex_lock(&m_stats);
		stats->n_getkeys +=1;
		pthread_mutex_unlock(&m_stats);
		return 0;
	} 

	else if (msg->opcode == MESSAGE_T__OPCODE__OP_PRINT && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;

		MessageT__Entry **msg_entries;
		msg_entries = malloc(table->nListas * sizeof(MessageT__Entry));
		int k = 0;
        for(int i=0 ; i<table->nListas ; i++){
            if(table->list[i] != NULL){
                int nEntries = list_size(table->list[i]);
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
		pthread_mutex_lock(&m_stats);
		stats->n_table_print +=1;
		pthread_mutex_unlock(&m_stats);
		return 0;
	}
	else if (msg->opcode == MESSAGE_T__OPCODE__OP_STATS && msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
		msg->opcode += 1;
		msg->c_type = MESSAGE_T__C_TYPE__CT_STATS;
		msg->n_stats = 6;
		msg->stats = malloc(6*sizeof(int));
		msg->stats[0] = stats->n_put;
		msg->stats[1] = stats->n_get;
		msg->stats[2] = stats->n_del;
		msg->stats[3] = stats->n_size;
		msg->stats[4] = stats->n_getkeys;
		msg->stats[5] = stats->n_table_print;
		msg->avg_time = stats->avg_time;
		return 0;
	}

	return -1;
}

void setStatsAVGTime(clock_t time){
	float seconds = ((float)clock()-time)/ CLOCKS_PER_SEC;
	int n_total = stats->n_del + stats->n_get + stats->n_getkeys + stats->n_put + stats->n_size + stats->n_table_print;
	float current_avg_time = stats->avg_time;
	pthread_mutex_lock(&m_stats);
	stats->avg_time = (current_avg_time * ((float)(n_total-1)) + seconds) / n_total;
	pthread_mutex_unlock(&m_stats);
}