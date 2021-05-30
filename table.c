
#include <string.h>
#include "item.h"
#include "get.h"
#include "table.h"
#include <stdlib.h>
#include <stdio.h>

void *Get_New_Table(Table *table) {
    int size1, size2, len, c_size1, k = 0;
    printf("Enter max size of ks1 table\n");
    Get_Int(&size1);
    printf("Enter max size of ks2 table\n");
    Get_Int(&size2);
    printf("Length of key2\n");
    Get_Int(&len);
    c_size1 = 0;
    table->ks1 = (KeySpace1 *) calloc(sizeof(KeySpace1), size1);
    table->ks2 = (KeySpace2 **) calloc(sizeof(KeySpace2 *), size2);
    fseek(table->fd, 0, SEEK_SET);

    table->ind_max_size1 = 0;
    fwrite(&size1, sizeof(int), 1, table->fd);// макс размер первого

    table->ind_cur_size1 = ftell(table->fd);
    fwrite(&c_size1, sizeof(int), 1, table->fd);//реальный размер первого

    table->ind_max_size2 = ftell(table->fd);
    fwrite(&size2, sizeof(int), 1, table->fd);// размер второго

    table->number_of_items = ftell(table->fd); // кол-во всего элементов
    fwrite(&c_size1, sizeof(int), 1, table->fd);

    table->ind_str_l = ftell(table->fd);
    fwrite(&len, sizeof(int), 1, table->fd);// макс длина строки второго
    return 0;
}

void *Get_Old_Table(Table *table) {
    int size1, size2, len, csize, k1, par, realise, len_k2, len_info;
    char *k2, *info;

    return 0;
}

Item *find(Table *t, int k1, char *k2) {
    int h, key1;
    unsigned len;
    char *key2;
    h = Hesh(t, k2);
    KeySpace2 *ptr = t->ks2[h];
    KeySpace2 *x = NULL;
    while (ptr != NULL) {

        fseek(t->fd, ptr->info->ind_key1, SEEK_SET);
        fread(&key1, sizeof(int), 1, t->fd);

        fseek(t->fd, ptr->info->ind_key2, SEEK_SET);
        fread(&len, sizeof(unsigned), 1, t->fd);
        key2 = (char *) calloc(len, sizeof(char));
        fread(key2, sizeof(char), len, t->fd);
        if (key1 == k1 && strcmp(key2, k2) == 0) {
            x = ptr;
        }
        free(key2);
        ptr = ptr->next;
    }
    if (x != NULL) {
        return x->info;
    } else {
        return NULL;
    }
}

Item *findk1(Table *t, int k1) {
    int size, key1, i;
    fseek(t->fd, t->ind_max_size1, SEEK_SET);
    fread(&size, sizeof(int), 1, t->fd);
    for (i = 0; i < size; i++) {
        if (t->ks1[i].ind_key != 0) {
            fseek(t->fd, t->ks1[i].ind_key, SEEK_SET);
            fread(&key1, sizeof(int), 1, t->fd);
            if (key1 == k1) {
                break;
            }
        }
    }
    if (i == size)
        return NULL;
    else
        return t->ks1[i].info;
}


KeySpace1 *findkpar(Table *t, int kpar, int *kol) {
    KeySpace1 *ks1;
    *kol = 0;
    ks1 = (KeySpace1 *) calloc(sizeof(KeySpace1), (*kol + 1));
    int size, key1, i;
    fseek(t->fd, t->ind_max_size1, SEEK_SET);
    fread(&size, sizeof(int), 1, t->fd);
    for (i = 0; i < size; i++) {
        if (t->ks1[i].ind_key != 0) {
            fseek(t->fd, t->ks1[i].ind_par, SEEK_SET);
            fread(&key1, sizeof(int), 1, t->fd);
            if (key1 == kpar) {
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

KeySpace2 *findk2(Table *t, char *k2) {
    KeySpace2 *res = NULL;
    KeySpace2 *ptr;
    char *key2;
    int h, len;
    h = Hesh(t, k2);
    ptr = t->ks2[h];
    while (ptr != NULL) {
        fseek(t->fd, ptr->ind_key, SEEK_SET);
        fread(&len, sizeof(unsigned), 1, t->fd);
        key2 = (char *) calloc(len, sizeof(char));
        fread(key2, sizeof(char), len, t->fd);
        if (strcmp(key2, k2) == 0) {
            res = ptr;
            free(key2);
            break;
        }
        free(key2);
        ptr = ptr->next;
    }
    return res;
}

KeySpace2 *findk2_realises(Table *t, char *k2,int* kol) {
    KeySpace2 *res =(KeySpace2*) calloc(1, sizeof(KeySpace2));
    KeySpace2 *ptr;
    char *key2;
    int h, len;
    h = Hesh(t, k2);
    ptr = t->ks2[h];
    while (ptr != NULL) {
        fseek(t->fd, ptr->ind_key, SEEK_SET);
        fread(&len, sizeof(unsigned), 1, t->fd);
        key2 = (char *) calloc(len, sizeof(char));
        fread(key2, sizeof(char), len, t->fd);
        if (strcmp(key2, k2) == 0) {
            res = realloc(res, sizeof(KeySpace2) * (*kol + 1));
            res[*kol] = *ptr;
            (*kol)++;
            free(key2);
        }
        else {
            free(key2);
        }
        ptr = ptr->next;
    }
    if (*kol == 0) {
        free(res);
        return NULL;
    } else
        return res;
}

int insert(Table *t, int k1, int par, char *k2, char *information) {
    int h, msize1, csize1, rel, key, n;
    unsigned len_k2, len_inf;

    fseek(t->fd, t->ind_max_size1, SEEK_SET);
    fread(&msize1, sizeof(int), 1, t->fd);

    fseek(t->fd, t->ind_cur_size1, SEEK_SET);
    fread(&csize1, sizeof(int), 1, t->fd);

    Item *y = findk1(t, k1);
    Item *x = find(t, k1, k2);
    if (y == NULL) {
        if (msize1 == csize1) return 2; // если нет элемента с составным таким же ключом и вся таблица забита
    }
    if (x == NULL && y != NULL) {
        return 1; // есть элемент с таким же 1 кдючом но не составным
    } else {
        if ((findk1(t, par) == NULL) && (par != 0)) return 3;
        else {
            KeySpace2 *newks2 = calloc(sizeof(KeySpace2), 1);
            Item *item = malloc(sizeof(Item));

            fseek(t->fd, 0, SEEK_END); // нашли конец

            item->ind_key1 = ftell(t->fd);
            fwrite(&k1, sizeof(int), 1, t->fd); // отступ для первого ключа

            len_k2 = strlen(k2);
            item->ind_key2 = ftell(t->fd);
            fwrite(&len_k2, sizeof(unsigned), 1, t->fd); // длина второй ключа
            fwrite(k2, sizeof(char), len_k2, t->fd); // второй ключ

            len_inf = strlen(information);
            item->ind_inf = ftell(t->fd);
            fwrite(&len_inf, sizeof(unsigned), 1, t->fd); // длина информации
            fwrite(information, sizeof(char), len_inf, t->fd); // информация

            h = Hesh(t, k2);
            if (t->ks2[h] == NULL) {
                rel = 0;
                fseek(t->fd, 0, SEEK_END);// нашли конец после хеша

                newks2->ind_realise = ftell(t->fd);
                fwrite(&rel, sizeof(int), 1, t->fd); // релиз во второй пространстве ключей


                newks2->ind_key = item->ind_key2;

                newks2->next = NULL;
                newks2->previous = NULL;
                newks2->info = item;
                item->ks2 = newks2;
                t->ks2[h] = newks2;

            } else {
                newks2->previous = NULL;
                KeySpace2 *z = findk2(t, k2);
                if (z == NULL) {
                    rel = 0;
                } else {
                    fseek(t->fd, z->ind_realise, SEEK_SET);
                    fread(&rel, sizeof(int), 1, t->fd);
                    rel++;
                }

                fseek(t->fd, 0, SEEK_END);
                newks2->ind_realise = ftell(t->fd);
                fwrite(&rel, sizeof(int), 1, t->fd);

                newks2->info = item;
                item->ks2 = newks2;
                newks2->ind_key = item->ind_key2;
                t->ks2[h]->previous = newks2;
                newks2->next = t->ks2[h];
                t->ks2[h] = newks2;


            }
            if (x != NULL) {
                while (x->next != NULL) {
                    x = x->next;
                }
                fseek(t->fd, x->ind_realise, SEEK_SET);
                fread(&rel, sizeof(int), 1, t->fd);
                rel++;
                fseek(t->fd, 0, SEEK_END);
                item->ind_realise = ftell(t->fd);
                fwrite(&rel, sizeof(int), 1, t->fd);
                x->next = item;
            } else {
                for (int i = 0; i < msize1; i++) {

                    if (t->ks1[i].info == NULL) {
                        fseek(t->fd, 0, SEEK_END);
                        rel = 0;
                        item->ind_realise = ftell(t->fd);
                        fwrite(&rel, sizeof(int), 1, t->fd);
                        t->ks1[i].ind_par = ftell(t->fd);
                        fwrite(&par, sizeof(int), 1, t->fd);
                        fwrite(&i, sizeof(int), 1, t->fd);
                        t->ks1[i].info = item;
                        t->ks1[i].ind_key = item->ind_key1;
                        break;
                    }
                }
                csize1++;
                fseek(t->fd, t->ind_cur_size1, SEEK_SET);
                fwrite(&csize1, sizeof(int), 1, t->fd);
            }
            fseek(t->fd, t->number_of_items, SEEK_SET);
            fread(&n, sizeof(int), 1, t->fd);
            n++;
            fseek(t->fd, t->number_of_items, SEEK_SET);
            fwrite(&n, sizeof(int), 1, t->fd);
            return 0;
        }
    }
}

void show(Table *t) {
    int size, i;
    Item *x;
    fseek(t->fd, t->ind_max_size1, SEEK_SET);
    fread(&size, sizeof(int), 1, t->fd);
    for (i = 0; i < size; i++) {
        x = t->ks1[i].info;
        show_elements(t, x);
    }
}

void show_elements(Table *t, Item *x) {
    int key1, rel, rel_ks2, par, k = 0;
    unsigned len_key2, len_info;
    char *inf = NULL, *key2 = NULL;
    while (x != NULL) {
        fseek(t->fd, x->ind_key1, SEEK_SET);
        fread(&key1, sizeof(int), 1, t->fd);
        fread(&len_key2, sizeof(unsigned), 1, t->fd);
        key2 = (char *) calloc(len_key2, sizeof(char));
        fread(key2, sizeof(char), len_key2, t->fd);
        fread(&len_info, sizeof(unsigned), 1, t->fd);
        inf = (char *) calloc(len_info, sizeof(char));
        fread(inf, sizeof(char), len_info, t->fd);
        fread(&rel_ks2, sizeof(int), 1, t->fd);
        fread(&rel, sizeof(int), 1, t->fd);
        if (k == 0) {
            fread(&par, sizeof(int), 1, t->fd);
            printf("Item key1: %d | par: %d | key2: %s | info: %s  | rel: %d | rel in ks2 %d\n", key1, par,
                   key2, inf, rel, rel_ks2);
        } else {
            printf("Item key1: %d | key2: %s | info: %s | rel: %d | rel in ks2 %d\n", key1, key2,
                   inf, rel, rel_ks2);
        }
        x = x->next;
        free(key2);
        free(inf);
        k++;
    }
}

void show_element(Table *t, Item *x) {
    int key1, rel, rel_ks2, par;
    unsigned len_key2, len_info;
    char *inf = NULL, *key2 = NULL;
    fseek(t->fd, x->ind_key1, SEEK_SET);
    fread(&key1, sizeof(int), 1, t->fd);
    fread(&len_key2, sizeof(unsigned), 1, t->fd);
    key2 = (char *) calloc(len_key2, sizeof(char));
    fread(key2, sizeof(char), len_key2, t->fd);
    fread(&len_info, sizeof(unsigned), 1, t->fd);
    inf = (char *) calloc(len_info, sizeof(char));
    fread(inf, sizeof(char), len_info, t->fd);
    fread(&rel_ks2, sizeof(int), 1, t->fd);
    fread(&rel, sizeof(int), 1, t->fd);
    printf("Item key1: %d | key2: %s | info: %s | rel: %d | rel in ks2 %d\n", key1, key2,
           inf, rel, rel_ks2);

    free(key2);
    free(inf);

}

/*
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
 */

void delete_all(Table *table) {
    free(table->ks1);
    free(table->ks2);
    fclose(table->fd);
}