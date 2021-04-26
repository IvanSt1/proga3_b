#include <string.h>
#include "get.h"
#include "table.h"
#include <stdlib.h>
#include <stdio.h>

int Get_Int(int *a) {
    int n = 0;
    while (n == 0) {
        n = scanf("%d", a);
        if (n == 0 || *a <= 0) {

            printf("Error reading positive integer value\n");
            scanf("%*[^\n]");
            n = 0;
        }
    }
    return n < 0 ? 0 : 1;
}

int Get_Int0(int *a) {
    int n = 0;
    while (n == 0) {
        n = scanf("%d", a);
        if (n == 0 || *a < 0) {
            printf("Error reading positive integer value\n");
            scanf("%*[^\n]");
            n = 0;
        }
    }
    return n < 0 ? 0 : 1;
}

char *Get_Str() {
    int len = 0, n;
    char buf[80];
    char *res = malloc(80 * sizeof(char));
    *res = '\0';
    scanf("%*[\n]");
    do {
        n = scanf("%79[^\n]", buf);
        if (n<0){
            free(res);
            return NULL;
        }
        len += strlen(buf);
        if (n > 0) {
            res = realloc(res, len * sizeof(char) + 1);
            strcat(res, buf);
        }
    } while (n != 0 && n != -1);
    if (n == -1) {
        free(res);
        return NULL;
    }
    return res;
}


char *Get_Strk2(Table *t) {
    char *res = NULL;
    res = Get_Str();
    while (strlen(res) > t->strl) {
        printf("wrong length of string\n");
        res = Get_Str();
    }
    return res;
}


int Hesh(Table *t, char *s) {
    int sum = 0;
    for (int i = 0; i < strlen(s); i++) {
        sum += (int) s[i];
    }
    return (sum % t->strl);
}