#include <stdio.h>

#include "headers/data.h"
#include "src/data.c"

#include "headers/entry.h"
#include "src/entry.c"

#include "headers/list.h"
#include "headers/list-private.h"
#include "src/list.c"

#include "headers/table.h"
#include "src/table.c"

int main(int argc, char **argv){
    struct table_t *table = table_create(2);
    struct data_t *data = data_create(3);
    struct data_t *data2 = data_create2(3, "abc");
    table_put(table, "123", data2);
    /*
    struct list_t *list = list_create();
    //list_destroy(list); //WARNING
    struct entry_t *entry = entry_create(NULL, NULL);
    list_add(list, entry);*/
    //list_remove(list, NULL);
    //struct entry_t *entry2 = list_get(list, NULL);
    //int i = list_size(list);
    //char **array = list_get_keys(list);
    //list_free_keys(array);
    //list_print(list);
}
