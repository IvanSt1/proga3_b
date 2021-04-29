
#include <string.h>
#include "item.h"
#include "get.h"
#include "table.h"
#include <stdlib.h>
#include <stdio.h>

void *Get_New_Table(Table *table) {
    int size1, size2, len;
    printf("Enter max size of ks1 table\n");
    Get_Int(&size1);
    printf("Enter max size of ks2 table\n");
    Get_Int(&size2);
    printf("Length of key2\n");
    Get_Int(&len);
    table->ks1 = calloc(sizeof(KeySpace1), size1);
    table->ks2 = (KeySpace2 **) calloc(sizeof(KeySpace2 *), size2);
    table->msize1 = size1;
    table->csize1 = 0;
    table->msize2 = size2;
    table->strl = len;
    for (int i = 0; i < size1; i++) {
        table->ks1[i].key = 0;
    }
    return 0;
}

void *Get_Old_Table(Table *table) {
    int size1, size2, len,csize,k1,par,realise,len_k2,len_info;
    long int key2_offset,info_offset, tmp_offset;
    char *k2, *info;
    fseek(table->fd, 0, SEEK_SET);
    fread(&size1,sizeof(int),1,table->fd);
    fread(&csize,sizeof(int),1,table->fd);
    fread(&size2,sizeof(int),1,table->fd);
    fread(&len,sizeof(int),1,table->fd);
    table->ks1 = calloc(sizeof(KeySpace1), size1);
    table->ks2 = (KeySpace2 **) calloc(sizeof(KeySpace2 *), size2);
    table->msize1 = size1;
    table->csize1 = 0;
    table->msize2 = size2;
    table->strl = len;
    for(int i=0; i<csize;i++){
        fread(&k1,sizeof(int),1,table->fd);
        fread(&par,sizeof(int),1,table->fd);
        fread(&realise,sizeof(int),1,table->fd);
        fread(&key2_offset,sizeof(long int),1,table->fd);
        fread(&len_k2,sizeof(int),1,table->fd);
        tmp_offset=ftell(table->fd);
        fseek(table->fd, key2_offset, SEEK_SET);
        fread(&k2,sizeof(char),len_k2,table->fd);
        fseek(table->fd, tmp_offset, SEEK_SET);

        fread(&info_offset,sizeof(long int),1,table->fd);
        fread(&len_info,sizeof(int),1,table->fd);
        tmp_offset=ftell(table->fd);
        fseek(table->fd, info_offset, SEEK_SET);
        fread(&info,sizeof(char),len_info,table->fd);
        fseek(table->fd, tmp_offset, SEEK_SET);
        insert(table,k1,par,k2,info);
    }
    return 0;
}
void *load(Table *ptab){
    long int key2_offset,info_offset, tmp_offset;
    int key2_len,info_len;
    fseek(ptab->fd, 0, SEEK_SET);
    fwrite(&ptab->msize1,sizeof (int),1,ptab->fd); // макс размер первого
    fwrite(&ptab->csize1,sizeof (int),1,ptab->fd); // реальный размер первого
    fwrite(&ptab->msize2,sizeof (int),1,ptab->fd); // размер второго
    fwrite(&ptab->strl,sizeof (int),1,ptab->fd);// макс длина строки второго
    int n=ptab->csize1;
    tmp_offset = ftell(ptab->fd);
    int k=0;
    for (int i=0; i<n*7;i++){
        fwrite(&k,sizeof(int),1,ptab->fd);
    }
    for(int i=0;i<ptab->msize1;i++){
        if(ptab->ks1[i].key!=0) {
            fseek(ptab->fd, tmp_offset, SEEK_SET);
            fwrite(&ptab->ks1[i].key, sizeof(int), 1, ptab->fd);// первый ключ
            fwrite(&ptab->ks1[i].par, sizeof(int), 1, ptab->fd);// родительский ключ
            fwrite(&ptab->ks1[i].info->realise, sizeof(int), 1, ptab->fd);// версию элемента
            tmp_offset = ftell(ptab->fd);
            fseek(ptab->fd, 0, SEEK_END);
            key2_offset = ftell(ptab->fd);
            key2_len=strlen(ptab->ks1[i].info->key2)+1;
            fwrite(&ptab->ks1[i].info->key2, sizeof(char), key2_len, ptab->fd); //второй ключ
            fseek(ptab->fd, tmp_offset, SEEK_SET);
            fwrite(&key2_offset, sizeof(long int), 1, ptab->fd);// смещение в файле ключа 2
            fwrite(&key2_len, sizeof(int), 1, ptab->fd); // длина второго ключа
            tmp_offset = ftell(ptab->fd);
            fseek(ptab->fd, 0, SEEK_END);
            info_offset = ftell(ptab->fd);
            info_len=strlen(ptab->ks1[i].info->key2)+1;
            fwrite(&ptab->ks1[i].info->inf, sizeof(char), info_len, ptab->fd); //второй ключ
            fseek(ptab->fd, tmp_offset, SEEK_SET);
            fwrite(&info_offset, sizeof(long int), 1, ptab->fd);// смещение в файле ключа 2
            fwrite(&info_len, sizeof(int), 1, ptab->fd);
            tmp_offset = ftell(ptab->fd);
        }
    }
    return 0;
}
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
    if (del_ks2->next == NULL) {
        if (del_ks2->previous == NULL) { //единственный элемент
            int h = Hesh(t, k2);
            t->ks2[h] = NULL;
        } else {// последний элемент
            del_ks2->previous->next = NULL;
        }
    } else {
        if (del_ks2->previous == NULL) { //первый элемент
            int h = Hesh(t, k2);
            del_ks2->next->previous = NULL;
            t->ks2[h] = del_ks2->next;
        } else { // не крайний элемент
            del_ks2->previous->next = del_ks2->next;
            del_ks2->next->previous = del_ks2->previous;

        }
    }
    //удаление из первого
    t->ks1[i].key = 0;
    t->ks1[i].info = NULL;
    t->ks1[i].par = 0;
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