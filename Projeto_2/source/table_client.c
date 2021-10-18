#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Projeto_1/source/data.c"
#include "../../Projeto_1/source/entry.c"

#define BUFFERSIZE 50

int main(int argc, char** argv) { 
    char *text = calloc(1,1), buffer[BUFFERSIZE];
    printf("Enter a message: \n");
    while(fgets(buffer, BUFFERSIZE , stdin)){
        text = realloc( text, strlen(text)+1+strlen(buffer) );
        if( !text ){
            printf("fail");
        }
        strcat( text, buffer ); 
        //printf("%s\n", buffer);   
        char *split = strtok(text, "\n");
        if(strcmp(split, "quit") == 0){
            break;
        }
        else if(strcmp(strtok(split, " "), "put") == 0){
            char *argKey = strtok(NULL, " ");
            char *argData = strtok(NULL, " ");

            int datasize = strlen(argData);
            struct data_t *data = data_create2(datasize, argData);
            struct entry_t *entry = entry_create(argKey, data);
    
        }

        else if(strcmp(strtok(split, " "), "get") == 0){
            char *argKey = strtok(NULL, " ");
        }

        else if(strcmp(strtok(split, " "), "del") == 0){
            char *argKey = strtok(NULL, " ");
        }

        else if(strcmp(split, "table_print") == 0);
        else if(strcmp(split, "getkeys") == 0);
        else if(strcmp(split, "size") == 0);
    }
    //printf("\n%d\n",argc);
    //printf("%s ", argv[0]);
    /*for(int i=0 ; i<argc ; i++){
        printf("%s ", argv[i]);
    }*/
    printf("\n");
    return 0;
 }