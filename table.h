//
// Created by ivan on 26.04.2021.
//

#ifndef PROGA3_B_TABLE_H
#define PROGA3_B_TABLE_H

#include <bits/types/FILE.h>
#include "item.h"

typedef struct Table {
    KeySpace1 *ks1;
    KeySpace2 **ks2;
    long int ind_max_size1;
    long int ind_cur_size1;
    long int number_of_items;
    long int ind_str_l;
    long int ind_max_size2;
    FILE *fd;
} Table;

int insert(Table *ptab, int k1, int par, char *k2, char *info);

void show(Table *t);
void show_elements(Table*t ,Item*x);
void show_element(Table*t ,Item*x);
Item *find(Table *ptab, int k1, char *k2);

Item *findk1(Table *t, int k1);

KeySpace1 *findkpar(Table *t, int kpar, int *kol);

KeySpace2 *findk2(Table *t, char *k2);
KeySpace2 *findk2_realises(Table *t, char *k2,int* size);
int delete(Table *ptab, int k1, char *k2);

void *Get_New_Table(Table *ptab);

void *Get_Old_Table(Table *table);

void delete_all(Table *table);

#endif //PROGA3_B_TABLE_H
