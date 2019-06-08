//
// Created by Laura Corssac on 2019-06-06.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../include/data.h"
#include "../include/hashtable.h"
#include "../include/error.h"
#include "../include/t2fs.h"

int main()
{

    char * gabriel = "gabriel";
    char * guerra = "guerra";

    DIRENT2 entry1;
    DIRENT2 *entry2 = malloc(sizeof(DIRENT2));
    DIRENT2 *entry3;

    entry1.fileSize = 100;
    entry1.fileType = '-';
    strcpy(entry1.name, gabriel);

    entry2->fileSize = 20;
    entry2->fileType = 'd';
    strcpy(entry2->name, guerra);

    entry3 = malloc(sizeof(DIRENT2));

    DataItem *hashArray[SIZE] = {NULL};

    assert(SUCCESS_CODE == addEntry(gabriel, &entry1, hashArray));
    assert(hashArray[0]->value.fileSize == entry1.fileSize);
    assert(hashArray[0]->value.fileType == entry1.fileType);
    assert(0 == strcmp(hashArray[0]->value.name, entry1.name));

    assert(SUCCESS_CODE == getValue(gabriel, &entry3, hashArray));
    assert(entry3->fileType == '-');
    assert(entry3->fileSize == 100);
    assert(0 == strcmp(entry3->name, gabriel));

    assert(FILE_NOT_FOUND == removeEntry(guerra, hashArray));
    assert(FILE_NOT_FOUND == getValue(guerra, &entry3, hashArray));

    assert(SUCCESS_CODE == removeEntry(gabriel, hashArray));

    assert(hashArray[0] == NULL);
    assert(FILE_NOT_FOUND == getValue(gabriel, &entry3, hashArray));

    assert(SUCCESS_CODE == addEntry(guerra, entry2, hashArray));
    assert(hashArray[0]->value.fileSize == entry2->fileSize);
    assert(hashArray[0]->value.fileType == entry2->fileType);
    assert( 0 == strcmp(hashArray[0]->value.name, entry2->name));

    assert(SUCCESS_CODE == addEntry(gabriel, &entry1, hashArray));

    direcory_mock = malloc(sizeof(Directory));
    direcory_mock->current_entry_index = 0;
    direcory_mock->hash_table = hashArray[0];

    DIRENT2 dirent2;

    assert(SUCCESS_CODE == readdir1(1, &dirent2));
    assert(direcory_mock->current_entry_index == 1);
    assert(dirent2.fileType == entry2->fileType);
    assert(dirent2.fileSize == entry2->fileSize);
    assert(0 == strcmp(dirent2.name, entry2->name));

    assert(SUCCESS_CODE == readdir1(1, &dirent2));
    assert(direcory_mock->current_entry_index == 2);
    assert(dirent2.fileType == entry1.fileType);
    assert(dirent2.fileSize == entry1.fileSize);
    assert(0 == strcmp(dirent2.name, entry1.name));

    assert(NULL_POINTER_EXCEPTION == readdir1(1, &dirent2));

    return 0;
}