
#include"dialog.h"
#include <string.h>
#include "item.h"
#include "get.h"
#include "table.h"
#include <stdlib.h>
#include <stdio.h>

const char *errmsgs[] = {"OK", "Duplicate key", "Table overflow", "Wrong parent key"};

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

int D_Find(Table *ptab) {
    int k1;
    char *k2 = NULL;
    Item *rc = NULL;
    printf("Enter key1: -->");
    Get_Int(&k1);
    printf("Enter key2: -->");
    k2 = Get_Strk2(ptab);

    if (k2 == NULL)
        return 0;
    if (ptab->csize1 != 0) {
        rc = find(ptab, k1, k2);
        if (rc == NULL) {
            free(k2);
            printf("There is not such key.");
            return 1;
        } else {
            printf("key1: %d | key2: %s | info: %s | realise: %d\n", rc->key1, rc->key2, rc->inf, rc->realise);
            free(k2);
            return 1;
        }
    } else
        printf("Empty table\n");
    return 1;
}

int D_Findk1(Table *ptab) {
    int k1;
    Item *rc = NULL;
    printf("Enter key1: -->");
    Get_Int(&k1);
    if (ptab->csize1 != 0) {
        rc = findk1(ptab, k1);
        if (rc == NULL) {
            printf("There is not such key.");
            return 1;
        } else {
            printf("key1: %d | key2: %s | info: %s | realise: %d\n", rc->key1, rc->key2, rc->inf, rc->realise);
            return 1;
        }
    } else
        printf("Empty table\n");
    return 1;
}

int D_Delete(Table *ptab) {

    char *k2 = NULL;
    int rc, k1;
    printf("Enter key1: -->");
    Get_Int(&k1);
    printf("Enter key2: -->");
    k2 = Get_Strk2(ptab);
    if (k2 == NULL)
        return 0;
    rc = delete(ptab, k1, k2);
    if (rc == 0) printf("There is not such key.\n");
    if (rc == 1) printf("Successful deletion.\n");
    return 1;
}


int D_Show(Table *ptab) {
    for (int i = 0; i < ptab->msize1; i++) {
        if (ptab->ks1[i].key != 0) {
            printf("key1: %d | parkey:%d |key2: %s | info: %s | realise: %d\n", ptab->ks1[i].key, ptab->ks1[i].par,
                   ptab->ks1[i].info->key2, ptab->ks1[i].info->inf, ptab->ks1[i].info->realise);
        }
    }
    return 1;
}

int D_ParFind(Table *ptab) {
    int l = 0;
    int park;
    printf("Enter parkey: -->");
    Get_Int(&park);
    KeySpace1 *t;
    t = findkpar(ptab, park, &l);
    if (t == NULL) {
        printf("There is not such key.");
        return 1;
    } else {
        for (int i = 0; i < l; i++) {
            printf("key1: %d | parkey:%d |key2: %s | info: %s | realise: %d\n", t[i].key, t[i].par, t[i].info->key2,
                   t[i].info->inf, t[i].info->realise);
        }
    }
    free(t);
    return 1;
}

int D_Find_Realises(Table *ptab) {
    char *k2 = NULL;
    KeySpace2 *cks2;
    int kol = 0;
    printf("Enter key2: -->");
    k2 = Get_Strk2(ptab);
    if (k2 == NULL) {
        return 0;
    }
    int h = Hesh(ptab, k2);
    cks2 = ptab->ks2[h];
    if (cks2 == NULL) {
        printf("There is not such key");
    } else {
        while (cks2->next != NULL) {
            if (strcmp(cks2->key, k2) == 0) {
                printf("key1: %d| key2: %s | info: %s| realise: %d\n", cks2->info->key1, cks2->key, cks2->info->inf,
                       cks2->realise);
                kol++;
            }
            cks2 = cks2->next;
        }
        if (strcmp(cks2->key, k2) == 0) {
            printf("key1: %d| key2: %s | info: %s| realise: %d\n", cks2->info->key1, cks2->key, cks2->info->inf,
                   cks2->realise);
            kol++;
        }
        if (kol == 0) {
            printf("There is not such key");
        }
    }
    return 1;
}

int D_Find_Current_Realise(Table *ptab) {
    char *k2 = NULL;
    int kol = 0;
    int r;
    printf("Enter key2: -->");
    k2 = Get_Strk2(ptab);
    if (k2 == NULL) {
        return 0;
    }
    printf("Enter realise: -->");
    Get_Int0(&r);
    KeySpace2 *ks2;
    int h = Hesh(ptab, k2);
    ks2 = ptab->ks2[h];
    if (ks2 == NULL) {
        printf("There is not such key\n");
    } else {
        while (ks2->next != NULL) {
            if ((strcmp(ks2->key, k2) == 0) && (ks2->realise == r)) {
                printf("key1: %d| key2: %s | info: %s| realise: %d\n", ks2->info->key1, ks2->key, ks2->info->inf,
                       ks2->realise);
                kol++;
            }
            (ks2) = (ks2->next);
        }
        if ((strcmp(ks2->key, k2) == 0) && (ks2->realise == r)) {
            printf("key1: %d| key2: %s | info: %s| realise: %d\n", ks2->info->key1, ks2->key, ks2->info->inf,
                   ks2->realise);
            kol++;
        }
        if (kol == 0) {
            printf("There is not such key and realise\n");
        }
    }

    return 1;
}
int D_Reorg(Table *ptab){
    char *k2 = NULL;
    printf("Enter key2: -->");
    k2 = Get_Strk2(ptab);
    if (k2 == NULL) {
        return 0;
    }
    int h= Hesh(ptab,k2);
    if (ptab->ks2[h]==NULL){
        printf("There is not such key.\n");
        return 1;
    }
    while (ptab->ks2[h]->next !=NULL){
        delete(ptab,ptab->ks2[h]->next->info->key1,ptab->ks2[h]->next->key);
    }
    printf("Successful deletion.\n");
    return 1;
}

int D_Delete_Key1(Table *ptab){
    int k1;
    Item *rc = NULL;
    printf("Enter key1: -->");
    Get_Int(&k1);
    if (ptab->csize1 != 0) {
        rc = findk1(ptab, k1);
        if (rc == NULL) {
            printf("There is not such key.");
            return 1;
        } else {
            delete(ptab,rc->key1,rc->key2);
            printf("Successful deletion.\n");
            return 1;
        }
    } else
        printf("Empty table\n");
    return 1;
}
int D_Delete_Key2(Table *ptab){
    char *k2 = NULL;
    printf("Enter key2: -->");
    k2 = Get_Strk2(ptab);
    if (k2 == NULL) {
        return 0;
    }
    int h= Hesh(ptab,k2);
    if (ptab->ks2[h]==NULL){
        printf("There is not such key.\n");
        return 1;
    }
    KeySpace2 *k=NULL;
    while (ptab->ks2[h]->next !=NULL){
        if(strcmp(ptab->ks2[h]->key,k2)==0) {
            delete(ptab, ptab->ks2[h]->info->key1, ptab->ks2[h]->key);
        }
        else{
            if (k==NULL){
                k=ptab->ks2[h];
                ptab->ks2[h]=ptab->ks2[h]->next;
            }
            else{
                ptab->ks2[h]=ptab->ks2[h]->next;
            }
        }
    }
    if(strcmp(ptab->ks2[h]->key,k2)==0) {
        delete(ptab, ptab->ks2[h]->info->key1, ptab->ks2[h]->key);
    }
    if (k!=NULL)
        ptab->ks2[h]=k;
    printf("Successful deletion.\n");
    return 1;
}
