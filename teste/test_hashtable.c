//
// Created by Laura Corssac on 2019-06-06.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define SIZE 20

// EXCEPTIONS ARE NEGATIVE NUMBERS
#define EXCEPTION -1
#define EMPTY_LINE_EXCEPTION -2
#define NULL_POINTER_EXCEPTION -3
#define MALLOC_ERROR_EXCEPTION -7
#define NOT_A_PATH_EXCEPTION -8
#define RELATIVE_PATH_NOT_SUPPORTED_EXCEPTION -9
#define FAILED_TO_READ_SECTOR -10
#define FAILED_TO_WRITE_SECTOR -11
#define INVALID_SIZE_FOR_FILE_NAME -12
#define HASH_TABLE_FULL -13

#define FILE_NOT_FOUND -404

// -------------------------------
#define SUCCESS_CODE 0
#define ERROR_CODE -100
// -------------------------------
#define DEBUG 0

typedef struct{
    unsigned int rootDirBegin;
    unsigned int rootDirEnd;
    unsigned int generalBlocksBegin;
    unsigned int numberOfBlocks;
    unsigned int bitmap_sector;
    unsigned int bitmap_size; // tamanho em bytes do bitmap

}SuperBloco;

typedef struct {

    int gabi;
}HashTable;

typedef struct {
    unsigned int address;
    unsigned int next;
    char* data;
}Block;

typedef struct {
    unsigned int block_address; // address for the first block
    char identifier; // f for file, d for directory
}Entry;

typedef struct {
    Entry directoryEntry;
    HashTable *entries; // entries for all the directories and files
}Directory;

typedef struct {
    Entry value;
    char *key;
}DataItem;


int addEntry(char *path, Entry *entry, DataItem **hashArray) {

    if (path == NULL) return NULL_POINTER_EXCEPTION;

    int i = 0;
    while (hashArray[i] != NULL && i < SIZE) {

        printf("passou aqui");
        i++;
    }

    printf("passou while");

    if (i < SIZE) {

        printf("entrou aqui");
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

    while (i < SIZE && strcmp(path, hashArray[i]->key) != 0) {
        i ++;
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

    while (i < SIZE && strcmp(path, hashArray[i]->key) != 0) {
        i ++;
    }
    if (i < SIZE && strcmp(path, hashArray[i]->key) == 0) {

        *entry = &(hashArray[i]->value);
        return SUCCESS_CODE;
    }

    return FILE_NOT_FOUND;

}

int main()
{

    char * gabriel = "gabriel";
    char * guerra = "guerra";

    Entry entry1;
    Entry entry2;
    Entry *entry3;

    entry1.block_address = 10;
    entry1.identifier = 'f';
    entry2.block_address = 20;
    entry2.identifier = 'd';

    entry3 = malloc(sizeof(Entry));

    printf("inicializou structs\n");


    DataItem *hashArray[SIZE] = {NULL};

    assert(SUCCESS_CODE == addEntry(gabriel, &entry1, hashArray));
    printf("adicionou entrada\n");
    assert(hashArray[0]->value.block_address == entry1.block_address);
    assert(SUCCESS_CODE == getValue(gabriel, &entry3, hashArray));
    printf("get value\n");
    assert(entry3->block_address == 10);
    assert(entry3->identifier == 'f');

    return 0;
}