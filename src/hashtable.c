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

    if ( i < SIZE && hashArray[i].valid == 1 && strcmp(path, hashArray[i].key) == 0) {

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

    char *direct_child_pathname;
    direct_child_pathname = strtok(path_copy, slash);

    //first parent is root directory

    //supposing root dir occupies one sector
    // TODO: - change to right position of root dir
    unsigned char *root_dir_data = malloc(SECTOR_SIZE);
    int result_root = read_sector(10, root_dir_data);
    if (result_root != SUCCESS_CODE) return result_root;

    Directory *parent_directory = (Directory *) root_dir_data;

    while (direct_child_pathname != NULL) {

        DIRENT2 *entry = malloc(sizeof(DIRENT2));
        int result = getValue(direct_child_pathname, &entry, parent_directory->hash_table);
        if (result != SUCCESS_CODE) return result;

        Block *block = malloc(sizeof(Block));

        //TODO: trocar o sectors per block
        int get_dir_result = read_block(&block, entry->firstCluster, 1);
        if (get_dir_result != SUCCESS_CODE) return get_dir_result;

        parent_directory = (Directory *) block->data;

        subdirs = strtok(NULL, slash);
    }

    opened_dir = parent_directory;

    return SUCCESS_CODE;

}

//DIR2 get_dir(char *pathname) {
//
//
//    //TODO Change to proper numbers
//    int NUMBER_OF_FILES = 10;
//    int STRING_LENGTH = 100;
//    const char separator[2] = "/";
//    char *token;
//    char files[NUMBER_OF_FILES][STRING_LENGTH+1];
//    int i = 0;
//
//    /* get the first token */
//    token = strtok(str, separator);
//
//    /* walk through other tokens */
//    while( token != NULL ) {
//
//        strcpy(files[i++], token);
//        token = strtok(NULL, s);
//    }
//
//    // First parent is root directory
//    // TODO: - change to right position of root dir
//
//    Block *root_dir_data = malloc(SECTOR_SIZE);
//    int result_root = read_sector(10, root_dir_data);
//    if (result_root != SUCCESS_CODE) return result_root;
//
//    Directory *parent_directory = (Directory *) root_dir_data;
//    DIRENT2 entry = malloc(sizeof(DIRENT2));
//
//    for (int i =0; i < NUMBER_OF_FILES; i++){
//
//        if (i != 0) {
//            Block *block = malloc(sizeof(Block));
//
//            //TODO: trocar o sectors per block
//            int get_dir_result = read_block(&block, entry->firstCluster, 1);
//            if (get_dir_result != SUCCESS_CODE) return get_dir_result;
//
//            parent_directory = (Directory *) block->data;
//        }
//
//        entry = malloc(sizeof(DIRENT2));
//        int result = getValue(subdirs, &entry, parent_directory->hash_table);
//        if (result != SUCCESS_CODE) return result;
//        if (entry->fileType == '-') return FILE_NOT_FOUND;
//
//
//    }
//
//
//
//
//
//
//}


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