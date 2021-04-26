#include"dialog.h"
#include <string.h>
#include "item.h"
#include "get.h"
#include "table.h"
#include <stdlib.h>
#include <stdio.h>

const char *msgs[] = {"1. Quite", "2. Add", "3. Find", "4. Par Find", "5. Delete", "6. Show", "7. Find all realise",
                      "8. Find curent realise", "9. Find key 1", "10. Reorganization", "11. Delete key 1",
                      "12. Delete key2"};
const int NMgsgs = sizeof(msgs) / sizeof(msgs[0]);

int
(*f[])(Table *) ={NULL, D_Add, D_Find, D_ParFind, D_Delete, D_Show, D_Find_Realises, D_Find_Current_Realise, D_Findk1,
                  D_Reorg, D_Delete_Key1, D_Delete_Key2};

int main() {
    Table table;
    int size1, size2, len;
    printf("Enter file name\n");
    char *s = Get_Str();
    table.fd = fopen(s, "r+b");
    if (table.fd == NULL) { //нет файла с таблицей
        Get_New_Table(&table);
    } else {// есть файл с таблицей
        Get_Old_Table(&table);
    }
    fclose( table.fd);
    int rc;
    rc = dialog(msgs, NMgsgs);
    while (rc) {
        if (!f[rc](&table)) {
            break;
        }
        rc = dialog(msgs, NMgsgs);
    }
    for (int i = 0; i < table.msize1; i++) {
        if (table.ks1[i].key != 0) {
            delete(&table, table.ks1[i].key, table.ks1[i].info->key2);
        }
    }
    table.fd = fopen(s, "w+b");
    load(&table);// обновление файла
    free(table.ks1);
    free(table.ks2);
    fclose(table.fd);
    return 0;
}
