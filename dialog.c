
#include"dialog.h"
#include <string.h>
#include "item.h"
#include "get.h"
#include "table.h"
#include <stdlib.h>
#include <stdio.h>

const char *errmsgs[] = {"OK", "Duplicate key", "Table overflow", "Wrong parent key", "There is no such key"};


int dialog(const char *msgs[], int n) {
    char *errmsg = "";
    int rc;
    int i, p;
    do {
        puts(errmsg);
        errmsg = "You are wrong";
        for (i = 0; i < n; ++i)
            puts(msgs[i]);
        puts("Make your choise: -->");
        p = Get_Int(&rc);
        if (p == 0)
            rc = 0;
    } while (rc < 0 || rc > n);
    rc--;
    return rc;
}
int D_Add(Table *ptab) {
    int rc;
    int k1;
    int par;
    char *k2 = NULL;
    char *info;
    printf("Enter key1: -->");
    Get_Int(&k1);
    printf("Enter parent key: -->");
    Get_Int0(&par);
    printf("Enter key2: -->");
    k2 = Get_Strk2(ptab);
    if (k2 == NULL)
        return 0;
    printf("Enter info:\n");
    info = Get_Str();
    if (info == NULL)
        return 0;
    rc = insert(ptab, k1, par, k2, info);
    printf("%s: %d, %s\n", errmsgs[rc], k1, k2);
    return 1;
}
int D_Find(Table *ptab) {
    Item *x;
    int k1;
    char *k2 = NULL;
    printf("Enter key1: -->");
    Get_Int(&k1);
    printf("Enter key2: -->");
    k2 = Get_Strk2(ptab);
    if (k2 == NULL)
        return 0;
    x= find(ptab,k1,k2);
    if(x==NULL){
        printf("%s: %d, %s\n", errmsgs[4], k1, k2);
    }
    else{
        show_elements(ptab,x);
    }
    return 1;
}

int D_Findk1(Table *ptab) {
    Item *x;
    int k1;
    printf("Enter key1: -->");
    Get_Int(&k1);
    x= findk1(ptab,k1);
    if(x==NULL){
        printf("%s: %d\n", errmsgs[4], k1);
    }
    else{
        show_elements(ptab,x);
    }
    return 1;
}

int D_Delete(Table *ptab) {


    return 1;
}


int D_Show(Table *t) {
    int size;
    fseek(t->fd, t->ind_cur_size1, SEEK_SET);
    fread(&size, sizeof(int), 1, t->fd);
    if (size == 0) {
        printf("Table is empty");
    } else {
        show(t);
    }
    return 1;
}

int D_ParFind(Table *ptab) {
    KeySpace1 *x;
    int k1,kol=0;
    printf("Enter parent: -->");
    Get_Int(&k1);
    x= findkpar(ptab,k1,&kol);
    if(x==NULL){
        printf("%s: %d\n", errmsgs[4], k1);
    }
    else{
        for(int i=0;i<kol;i++){
            show_elements(ptab,x->info);
        }
    }
    return 1;
}

int D_Find_Realises(Table *ptab) {
    KeySpace2 *x;
    KeySpace2 *next;
    int k1,kol=0;
    char*k2;
    printf("Enter key2: -->");
    k2 = Get_Strk2(ptab);
    if (k2 == NULL)
        return 0;
    x= findk2_realises(ptab,k2,&kol);
    if(x==NULL){
        printf("%s: %s\n", errmsgs[4], k2);
    }
    else{
        for(int i=0;i<kol;i++){
            show_element(ptab,x[i].info);
        }
        free(x);
    }
    return 1;
}

int D_Find_Current_Realise(Table *ptab) {

    return 1;
}

int D_Reorg(Table *ptab) {

}

int D_Delete_Key1(Table *ptab) {

}

int D_Delete_Key2(Table *ptab) {

    return 1;
}
