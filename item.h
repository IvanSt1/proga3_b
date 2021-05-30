//
// Created by ivan on 26.04.2021.
//

#ifndef PROGA3_B_ITEM_H
#define PROGA3_B_ITEM_H
typedef struct Item {
    long int ind_inf; // сначала длина потом строка
    long int ind_key1;
    long int  ind_key2; // сначала длина потом строка
    long int ind_realise;
    struct Item* next;
    struct KeySpace2 *ks2;
} Item;
typedef struct KeySpace1 {
    long int ind_key;
    long int ind_par;
    Item *info;
} KeySpace1;
typedef struct KeySpace2 {
    long int ind_key;
    Item *info;
    long int ind_realise;
    struct KeySpace2 *next;
    struct KeySpace2 *previous;
} KeySpace2;
#endif //PROGA3_B_ITEM_H
