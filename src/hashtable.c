//
// Created by gabriel on 06/06/19.
//

#include "../include/hashtable.h"
#include "../include/data.h"
#include "../include/error.h"
#include "../include/apidisk.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int addEntry(char *path, DIRENT2 *entry, DataItem **hashArray) {

    //verificar nome repetido

    if (path == NULL) return NULL_POINTER_EXCEPTION;

    int i = 0;

    while ( i < SIZE && (*hashArray)[i].valid == 1) {

        i++;
    }

    if (i < SIZE) {

        DataItem *item = malloc(sizeof(DataItem));
        item->valid = 1;
        item->key = malloc(sizeof(char));
        strcpy(item->key, entry->name);
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

    int i = 0;

    if (path == NULL) return NULL_POINTER_EXCEPTION;
    if (*entry == NULL) return NULL_POINTER_EXCEPTION;
    if (hashArray == NULL) return NULL_POINTER_EXCEPTION;

    while (i < SIZE) {

        char * key = hashArray[i].key;

        if ( hashArray[i].valid == 1 && key != NULL && strcmp(path, key ) == 0) {
            break;
        }

        i++;

    }

    if ( hashArray[i].valid == 1 && i < SIZE && strcmp(path, hashArray[i].key) == 0) {

        *entry = &(hashArray[i].value);
        return SUCCESS_CODE;
    }

    return FILE_NOT_FOUND;

}

int get_directory(Directory **directory) {

    Directory *dir = malloc(sizeof(Directory));

    memcpy(dir, directory_array[dir_index], sizeof(Directory));

    if (dir == NULL) return NULL_POINTER_EXCEPTION;

    *directory = dir;

    dir_index += 1;
    return SUCCESS_CODE;
}

DIR2 opendir1 (char *pathname) {


    const char slash[2] = "/";
    char path_copy[MAX_FILE_NAME_SIZE];
    strcpy(path_copy, pathname);

    char * subdirs;
    subdirs = strtok(path_copy, slash);

    Directory *parent_directory = malloc(sizeof(Directory));
    DIRENT2 *entry;

    while( subdirs != NULL ) {

        int get_dir_result = get_directory(&parent_directory);
        if (get_dir_result != SUCCESS_CODE) return get_dir_result;
        if (parent_directory == NULL) return NULL_POINTER_EXCEPTION;


        //precisa desse maloco?
        entry = malloc(sizeof(DIRENT2));

        int result = getValue(subdirs, &entry, parent_directory->hash_table);

        if (result != SUCCESS_CODE) return result;
        if (entry->fileType == '-') return FILE_NOT_FOUND;

        //como pego o proximo diretorio?? tenho o nome mas e o end?

        subdirs = strtok(NULL, slash);
    }

    int get_dir_result = get_directory(&parent_directory);
    if (get_dir_result != SUCCESS_CODE) return get_dir_result;
    opened_dir = parent_directory;

    return SUCCESS_CODE;

}

int readdir1 (DIR2 handle, DIRENT2 *dentry) {

//    //acha o diretório a partir do id só deus sabe como
//    //por enquanto o dir esta mokado
//
//
//    int current_index = direcory_mock->current_entry_index;
//    if (current_index >= SIZE) return INDEX_OUT_OF_RANGE;
//
//
//    DIRENT2 *current_entry = &(direcory_mock->hash_table[current_index].value);
//    if (current_entry == NULL) return NULL_POINTER_EXCEPTION;
//
//
//
//    *dentry = *current_entry;
//
//    direcory_mock->current_entry_index += 1;

    return SUCCESS_CODE;

}