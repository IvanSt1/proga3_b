//
// Created by ivan on 26.04.2021.
//

#ifndef PROGA3_B_TABLE_H
#define PROGA3_B_TABLE_H
#include "item.h"

typedef struct Table {
    KeySpace1 *ks1;
    KeySpace2 **ks2;
    int msize1;
    int csize1;
    int strl;
    int msize2;
} Table;

int insert(Table *ptab, int k1, int par, char *k2, char *info);

Item *find(Table *ptab, int k1, char *k2);

Item *findk1(Table *t, int k1);

KeySpace1 *findkpar(Table *t, int kpar, int *kol);

int delete(Table *ptab, int k1, char *k2);
#endif //PROGA3_B_TABLE_H
