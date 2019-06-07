//
// Created by gabriel on 06/06/19.
//

#include "../include/hashtable.h"
#include "../include/data.h"
#include "../include/error.h"
#include <string.h>

struct DataItem {
    Entry value;
    char *key;
};

struct DataItem* hashArray[SIZE];

int addEntry(char *path, Entry *entry) {

    if (path == NULL) return NULL_POINTER_EXCEPTION;

    int i = 0;
    while (hashArray[i] != NULL && i < SIZE) {
        i++;
    }

    if (i < SIZE) {

        struct DataItem item;

        hashArray[i] = *entry; // certo?
        return SUCCESS_CODE;

    }

    return HASH_TABLE_FULL;
}
int removeEntry(char *path) {

    int i = 0;

    if (*entry->path == NULL) return NULL_POINTER_EXCEPTION;

    while (i < SIZE && strcmp(path, hashArray[i]->key) != 0) {
        i ++;
    }
    if (i < SIZE && strcmp(path, hashArray[i]->key) == 0) {

        hashArray[i] = NULL;
        return SUCCESS_CODE;
    }

    return FILE_NOT_FOUND;

}

int getValue(char *path, Entry **entry) {

    int i = 0;

    if (*entry->path == NULL) return NULL_POINTER_EXCEPTION;

    while (i < SIZE && strcmp(path, hashArray[i]->key) != 0) {
        i ++;
    }
    if (i < SIZE && strcmp(path, hashArray[i]->key) == 0) {

        *entry = &(hashArray[i]->value);
        return SUCCESS_CODE;
    }

    return FILE_NOT_FOUND;

}