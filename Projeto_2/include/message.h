/********* Grupo 9 ********
* 44898 - José Alves      *
* 46670 - Tiago Lourenço  *
* 51779 - Ruhan Azevedo   *
***************************/

#ifndef _MESSAGE_H
#define _MESSAGE_H


/* Função que garante a leitura de len bytes armazenados em buf
 * através da connecao com uma socket sock
*/
int write_all(int sock, char *buf, int len);

/* Função que garante a escrita de len bytes armazenados em buf
 * através da connecao com uma socket sock
*/
int read_all(int sock, char *buf, int len);

#endif
