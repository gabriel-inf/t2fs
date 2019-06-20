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

//int initialize_hashTable(DataItem **hashArray) {
//
//    //*hashArray = malloc(sizeof(DataItem) * SIZE);
//
//    int i = 0;
//    for (i=0; i < SIZE; i++) {
//
//        (*hashArray)[i].valid = 0;
//        strncpy((*hashArray)[i].key, "", MAX_FILE_NAME_SIZE);
//    }
//
//    return SUCCESS_CODE;
//
//}

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
        assert(strlen(path) <= MAX_FILE_NAME_SIZE);

        printf("tamanho path = %d\n", strlen(path) );
        strncpy(item->key, path, MAX_FILE_NAME_SIZE+1);
        printf("tamanho key = %d\n", strlen(item->key) );

        assert(strlen(item->key) <= MAX_FILE_NAME_SIZE);
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

    if (i < SIZE && (*hashArray)[i].key != NULL && strcmp(path, (*hashArray)[i].key) == 0) {

        (*hashArray[i]).valid = 0;
        return SUCCESS_CODE;
    }

    return FILE_NOT_FOUND;

}

int getValue(char *path, DIRENT2 **entry, DataItem *hashArray) {

    if (DEBUG) printf("BEGIN OF GET VALUE\n\n");

    int i = 0;

    if (path == NULL) return NULL_POINTER_EXCEPTION;
    if (*entry == NULL) return NULL_POINTER_EXCEPTION;
    if (hashArray == NULL) return NULL_POINTER_EXCEPTION;

    while (i < SIZE) {


        char * key = hashArray[i].key;

        printf("size da key = %d\n", strlen(key));
        printf("key = \n");
        puts(key);

        if (hashArray[i].valid != 1) {
            printf("VALIDATION\n");
        }

        if (strcmp(path, key) != 0) {
            printf("COMPARE\n");
            puts(path);
            puts(key);
        }

        if ( hashArray[i].valid == 1 && key != NULL && strcmp(path, key) == 0) {
            break;
        }

        i++;

    }

//    if (i >= SIZE) {
//        printf("SIZE\n");
//    }


    if ( i < SIZE && hashArray[i].valid == 1 && strcmp(path, hashArray[i].key) == 0) {
        *entry = &(hashArray[i].value);
        if (DEBUG) printf("END OF GET VALUE SUCCESS\n\n");
        return SUCCESS_CODE;
    }

    if (DEBUG) printf("END OF GET VALUE NO PATH\n\n");
    return FILE_NOT_FOUND;

}