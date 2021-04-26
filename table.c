
#include <string.h>
#include "item.h"
#include "get.h"
#include "table.h"
#include <stdlib.h>

Item *find(Table *t, int k1, char *k2) {
    int i;
    for (i = 0; i < t->msize1; i++) {
        if (t->ks1[i].key != 0) {
            if ((t->ks1[i].key == k1) && (strcmp(t->ks1[i].info->key2, k2) == 0)) {
                break;
            }
        }
    }

    if (i == t->msize1)
        return NULL;
    else
        return t->ks1[i].info;
}

Item *findk1(Table *t, int k1) {
    int i = 0;
    while ((i < t->msize1) && (t->ks1[i].key != k1)) {
        i++;
    }
    if (i == t->msize1)
        return NULL;
    else
        return t->ks1[i].info;
}

KeySpace1 *findkpar(Table *t, int kpar, int *kol) {
    KeySpace1 *ks1;
    *kol = 0;
    ks1 = (KeySpace1 *) calloc(sizeof(KeySpace1), (*kol + 1));
    for (int i = 0; i < t->msize1; i++) {
        if (t->ks1[i].key != 0) {
            if (t->ks1[i].par == kpar) {
                ks1 = realloc(ks1, sizeof(KeySpace1) * (*kol + 1));
                ks1[*kol] = t->ks1[i];
                (*kol)++;
            }
        }
    }
    if (*kol == 0) {
        free(ks1);
        return NULL;
    } else
        return ks1;
}


int insert(Table *t, int k1, int par, char *k2, char *information) {
    int h;
    if (findk1(t, k1) != NULL) return 1;
    else {
        if (t->msize1 == t->csize1) return 2;
        else {
            if ((findk1(t, par) == NULL) && (par != 0)) return 3;
            else {
                KeySpace2 *newks2 = calloc(sizeof(KeySpace2), 1);
                Item *item = malloc(sizeof(Item));
                item->key1 = k1;
                item->key2 = k2;
                item->inf = information;
                h = Hesh(t, k2);
                if (t->ks2[h] == NULL) {

                    item->realise = 0;
                    newks2->realise = 0;
                    newks2->key = k2;
                    newks2->next = NULL;
                    newks2->previous = NULL;
                    newks2->info = item;
                    item->ks2 = newks2;
                    t->ks2[h] = newks2;
                } else {
                    newks2->key = k2;
                    newks2->previous = NULL;
                    newks2->realise = t->ks2[h]->realise + 1;
                    item->realise = t->ks2[h]->realise + 1;
                    newks2->info = item;
                    t->ks2[h]->previous = newks2;
                    newks2->next = t->ks2[h];
                    t->ks2[h] = newks2;
                    item->ks2 = newks2;
                }
                for (int i = 0; i < t->msize1; i++) {
                    if (t->ks1[i].key == 0) {
                        t->ks1[i].key = k1;
                        t->ks1[i].par = par;
                        t->ks1[i].info = item;
                        break;
                    }
                }
                t->csize1++;
                return 0;
            }
        }
    }
}

int delete(Table *t, int k1, char *k2) {
    int i = 0;
    // нахождение по первому пространству элемент который нужно удалить
    for (; i < t->msize1; ++i) {
        if (t->ks1[i].key != 0) {
            if ((strcmp(t->ks1[i].info->key2, k2) == 0) && (t->ks1[i].key == k1)) {
                break;
            }
        }
    }
    if (i == t->msize1) {
        return 0;
    }
    Item *item;
    item = t->ks1[i].info;
    KeySpace2 *del_ks2;
    del_ks2 = item->ks2;
    // удаление из второго пространства ключей
    if (del_ks2->next==NULL){
        if(del_ks2->previous==NULL){ //единственный элемент
            int h = Hesh(t, k2);
            t->ks2[h]=NULL;
        }
        else{// последний элемент
            del_ks2->previous->next=NULL;
        }
    }
    else
    {
        if (del_ks2->previous==NULL){ //первый элемент
            int h = Hesh(t, k2);
            del_ks2->next->previous=NULL;
            t->ks2[h]=del_ks2->next;
        }
        else{ // не крайний элемент
            del_ks2->previous->next = del_ks2->next;
            del_ks2->next->previous=del_ks2->previous;

        }
    }
    //удаление из первого
    t->ks1[i].key = 0;
    t->ks1[i].info=NULL;
    t->ks1[i].par=0;
    t->csize1 = t->csize1 - 1;
    // замена родительских ключей на нули
    for (int i = 0; i < t->msize1; i++) {
        if (t->ks1[i].key != 0) {
            if (t->ks1[i].par == k1) {
                t->ks1[i].par = 0;
            }
        }

    }

    //очищение памяти
    free(item->key2);
    free(item->inf);
    free(item);
    free(del_ks2);
    return 1;

}