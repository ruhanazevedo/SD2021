#include <stdio.h>

#include "headers/data.h"
#include "src/data.c"

#include "headers/entry.h"
#include "src/entry.c"

#include "headers/list.h"
#include "headers/list-private.h"
#include "src/list.c"

int main(void){
    struct list_t *list = list_create();
    list_destroy(list);
    struct entry_t *entry = entry_create(NULL, NULL);
    list_add(list, entry);
    //list_remove(list, NULL);
    //struct entry_t *entry2 = list_get(list, NULL);
    //int i = list_size(list);
    //char **array = list_get_keys(list);
    //list_free_keys(array);
    //list_print(list);
}