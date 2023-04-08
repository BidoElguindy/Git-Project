#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ex6.h"
#define COMMIT_SIZE 100


kvp* createKeyVal(char* key, char* val) {
    kvp* k = malloc(sizeof(kvp));
    k->key = strdup(key);
    k->value = strdup(val);
    return k;
}

void freeKeyVal(kvp* kv) {
    free(kv->key);
    free(kv->value);
    free(kv);
}

char* kvts(kvp* k) {
    char* buff = malloc(sizeof(char) * 100);
    sprintf(buff, "%s:%s", k->key, k->value);
    return buff;
}


kvp* stkv(char* str) {
    char* key = strtok(str, ":");
    char* value = strtok(NULL, ":");
    return createKeyVal(key,value);
}



Commit* initCommit() {
    Commit* c = malloc(sizeof(Commit));
    c->n = 0;
    c->size = COMMIT_SIZE;
    c->T = malloc(sizeof(kvp*) * COMMIT_SIZE);
    for (int i = 0; i < COMMIT_SIZE; i++) {
        c->T[i] = NULL;
    }
    return c;
}
