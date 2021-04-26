#include"dialog.h"
#include <string.h>
#include "item.h"
#include "get.h"
#include "table.h"
#include <stdlib.h>
#include <stdio.h>

const char *msgs[] = {"1. Quite", "2. Add", "3. Find", "4. Par Find", "5. Delete", "6. Show", "7. Find all realise",
                      "8. Find curent realise", "9. Find key 1", "10. Reorganization", "11. Delete key 1","12. Delete key2"};
const int NMgsgs = sizeof(msgs) / sizeof(msgs[0]);

int
(*f[])(Table *) ={NULL, D_Add, D_Find, D_ParFind, D_Delete, D_Show, D_Find_Realises, D_Find_Current_Realise, D_Findk1, D_Reorg,D_Delete_Key1,D_Delete_Key2};

int main() {
    int size1, size2, len;
    printf("Enter max size of ks1 table\n");
    Get_Int(&size1);

    printf("Enter max size of ks2 table\n");
    Get_Int(&size2);

    printf("Length of key2\n");
    Get_Int(&len);
    Table table;
    table.ks1 = calloc(sizeof(KeySpace1), size1);
    table.ks2 = (KeySpace2 **) calloc(sizeof(KeySpace2 *), size2);
    table.msize1 = size1;
    table.csize1 = 0;
    table.msize2 = size2;
    table.strl = len;
    for (int i = 0; i < size1; i++) {
        table.ks1[i].key = 0;
    }

    int rc;
    rc = dialog(msgs, NMgsgs);
    while (rc) {
        if (!f[rc](&table)) {
            break;
        }
        rc = dialog(msgs, NMgsgs);
    }
    for(int i=0;i<table.msize1;i++){
        if(table.ks1[i].key!=0){
            delete(&table,table.ks1[i].key,table.ks1[i].info->key2);
        }
    }
    free(table.ks1);
    free(table.ks2);
    return 0;
}
