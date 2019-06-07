//
// Created by gabriel on 06/06/19.
//

#include "../include/hashtable.h"
#include "../include/data.h"
#include "../include/error.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int addEntry(char *path, Entry *entry, DataItem **hashArray) {


    if (path == NULL) return NULL_POINTER_EXCEPTION;

    int i = 0;

    while (hashArray[i] != NULL && i < SIZE) {

        i++;
    }

    if (i < SIZE) {


        DataItem *item = malloc(sizeof(DataItem));
        item->key = malloc(sizeof(char));
        strcpy(item->key, path);
        item->value = *entry;

        hashArray[i] = item;

        return SUCCESS_CODE;

    }

    return HASH_TABLE_FULL;
}
int removeEntry(char *path, DataItem **hashArray) {

    int i = 0;

    if (path == NULL) return NULL_POINTER_EXCEPTION;

    while (i < SIZE) {

        if (hashArray[i] != NULL) {
            if (strcmp(path, hashArray[i]->key) != 0) {
                i ++;
                continue;
            } else {
                break;
            }
        }
        i++;
    }

    if (i < SIZE && strcmp(path, hashArray[i]->key) == 0) {

        hashArray[i] = NULL;
        return SUCCESS_CODE;
    }

    return FILE_NOT_FOUND;

}

int getValue(char *path, Entry **entry, DataItem **hashArray) {

    int i = 0;

    if (path == NULL) return NULL_POINTER_EXCEPTION;
    if (*entry == NULL) return NULL_POINTER_EXCEPTION;

    while (i < SIZE) {

        if (hashArray[i] != NULL ) {
            if (strcmp(path, hashArray[i]->key) != 0) {
                i++;
                continue;
            } else {
                break;
            }
        }
        i++;
    }

    if (i < SIZE && strcmp(path, hashArray[i]->key) == 0) {

        *entry = &(hashArray[i]->value);
        return SUCCESS_CODE;
    }

    return FILE_NOT_FOUND;

}