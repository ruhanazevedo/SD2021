/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/


#ifndef _TABLE_SKEL_PRIVATE_H
#define _TABLE_SKEL_PRIVATE_H

#include "time.h"

/*
 * Altera o average time contido no stats
 */
void setStatsAVGTime(clock_t time);

/*
 * Função para mapear o init do table_skel, isto é, fazer o init necessario do table_skel_init e também o init do zookeeper
 */
int table_skel_mapping(int n_lists, char *zk_endpoint, char *server_port);

/*
 * Função auxiliar de watch do zookeeper
 */
void watcher_function();

#endif
