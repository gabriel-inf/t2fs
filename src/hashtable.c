//
// Created by gabriel on 06/06/19.
//

#include "../include/hashtable.h"
#include "../include/data.h"
#include "../include/error.h"
#include "../include/apidisk.h"
#include "../include/helper.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int initialize_hashTable(DataItem **hashArray) {

    *hashArray = malloc(sizeof(DataItem) * SIZE);

    int i = 0;
    for (i=0; i < SIZE; i++) {

        (*hashArray)[i].valid = 0;
        (*hashArray)[i].key = malloc(sizeof(MAX_FILE_NAME_SIZE));
    }

    return SUCCESS_CODE;

}

int addEntry(char *path, DIRENT2 *entry, DataItem **hashArray) {

    if (path == NULL) return NULL_POINTER_EXCEPTION;

    int i = 0;

    DIRENT2 *entry_copy = malloc(sizeof(DIRENT2));
    memcpy(entry_copy, entry, sizeof(DIRENT2));

    //verifies if name is present in hashtable
    if ((getValue(path, &entry_copy, *hashArray)) == SUCCESS_CODE) return KEY_ALREADY_PRESENT;

    while ( i < SIZE && (*hashArray)[i].valid == 1) {

        i++;
    }

    if (i < SIZE) {

        DataItem *item = malloc(sizeof(DataItem));
        item->valid = 1;
        item->key = malloc(sizeof(char));
        strcpy(item->key, path);
        item->value = *entry;

        (*hashArray)[i] = *item;

        return SUCCESS_CODE;

    }

    return HASH_TABLE_FULL;
}
int removeEntry(char *path, DataItem **hashArray) {

    int i = 0;

    if (path == NULL) return NULL_POINTER_EXCEPTION;

    while (i < SIZE) {

        char * key = (*hashArray)[i].key;
        if ( key != NULL && strcmp(key, path) == 0  ){
            break;
        }

        i ++;
    }

    if (i < SIZE && strcmp(path, (*hashArray)[i].key) == 0) {

        (*hashArray[i]).valid = 0;
        return SUCCESS_CODE;
    }

    return FILE_NOT_FOUND;

}

int getValue(char *path, DIRENT2 **entry, DataItem *hashArray) {

    if (DEBUG) printf("BEGIN OF GET VALUE\n");

    int i = 0;

    if (path == NULL) return NULL_POINTER_EXCEPTION;
    if (DEBUG) printf("path foi\n");
    if (*entry == NULL) return NULL_POINTER_EXCEPTION;
    if (DEBUG) printf("entry foi\n");
    if (hashArray == NULL) return NULL_POINTER_EXCEPTION;
    if (DEBUG) printf("hasharray foi\n");

    while (i < SIZE) {

        char * key = hashArray[i].key;

        if ( hashArray[i].valid == 1 && key != NULL && strcmp(path, key ) == 0) {
            break;
        }

        i++;

    }

    if ( i < SIZE && hashArray[i].valid == 1 && strcmp(path, hashArray[i].key) == 0) {

        *entry = &(hashArray[i].value);
        if (DEBUG) printf("END OF GET VALUE\n");
        return SUCCESS_CODE;
    }

    if (DEBUG) printf("END OF GET VALUE\n");
    return FILE_NOT_FOUND;

}