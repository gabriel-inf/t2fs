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

void test_hashtable() {

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

    DataItem *hashArray = malloc(sizeof(DataItem) * SIZE);

    assert(SUCCESS_CODE == addEntry(gabriel, &entry1, &hashArray ));

    assert(hashArray[0].value.fileSize == entry1.fileSize);
    assert(hashArray[0].value.fileType == entry1.fileType);
    assert(0 == strcmp(hashArray[0].value.name, entry1.name));

    assert(SUCCESS_CODE == getValue(gabriel, &entry3, hashArray));
    assert(entry3->fileType == '-');
    assert(entry3->fileSize == 100);
    assert(0 == strcmp(entry3->name, gabriel));

    assert(FILE_NOT_FOUND == removeEntry(guerra, &hashArray));
    assert(FILE_NOT_FOUND == getValue(guerra, &entry3, hashArray));

    assert(SUCCESS_CODE == removeEntry(gabriel, &hashArray));
    assert(hashArray[0].valid == 0);
    assert(FILE_NOT_FOUND == getValue(gabriel, &entry3, hashArray));

    assert(SUCCESS_CODE == addEntry(guerra, entry2, &hashArray));
    assert(hashArray[0].value.fileSize == entry2->fileSize);
    assert(hashArray[0].value.fileType == entry2->fileType);
    assert( 0 == strcmp(hashArray[0].value.name, entry2->name));

    assert(SUCCESS_CODE == addEntry(gabriel, &entry1, &hashArray));

//    direcory_mock = malloc(sizeof(Directory));
//    direcory_mock->current_entry_index = 0;
//    direcory_mock->hash_table = hashArray[0];

//    DIRENT2 dirent2;
//
//    assert(SUCCESS_CODE == readdir1(1, &dirent2));
//    assert(direcory_mock->current_entry_index == 1);
//    assert(dirent2.fileType == entry2->fileType);
//    assert(dirent2.fileSize == entry2->fileSize);
//    assert(0 == strcmp(dirent2.name, entry2->name));
//
//    printf("size %8u\n", dirent2.fileSize);
//    printf("type %c\n", (dirent2.fileType==0x02?'d':'-'));
//
//    assert(SUCCESS_CODE == readdir1(1, &dirent2));
//    assert(direcory_mock->current_entry_index == 2);
//
//    printf("size %8u\n", dirent2.fileSize);
//    printf("type %c\n", (dirent2.fileType==0x02?'d':'-'));
//
//    assert(dirent2.fileSize == entry1.fileSize);
//    assert(dirent2.fileType == entry1.fileType);
//    assert(0 == strcmp(dirent2.name, entry1.name));

    printf("TODOS TESTES DE HASHTABLE PASSARAM\n");
}

void test_open_dir() {

    //root -> cookie_dir -> cafe_dir
    //          |-> file


    DIRENT2 root_dir_entry;
    root_dir_entry.fileType = 'd';
    strcpy(root_dir_entry.name, "root");

    DIRENT2 cookie_dir_entry;
    cookie_dir_entry.fileType = 'd';
    strcpy(cookie_dir_entry.name, "cookie");

    DIRENT2 cafe_dir_entry;
    cafe_dir_entry.fileType = 'd';
    strcpy(cafe_dir_entry.name, "cafe");

    DIRENT2 file;
    file.fileType = '-';

    root_dir = malloc(sizeof(Directory));
    root_dir->identifier = 10;

    Directory *cookie_dir = malloc(sizeof(Directory));
    cookie_dir->identifier = 1;

    Directory *cafe_dir = malloc(sizeof(Directory));
    cafe_dir->identifier = 2;

    DataItem *hashArray_root = malloc(sizeof(DataItem) * SIZE);
    DataItem *hashArray_cookie = malloc(sizeof(DataItem) * SIZE);

    assert(SUCCESS_CODE == addEntry("cookie", &cookie_dir_entry, &hashArray_root));
    assert(SUCCESS_CODE == addEntry("cafe", &cafe_dir_entry, &hashArray_cookie));
    assert(SUCCESS_CODE == addEntry("file", &file, &hashArray_cookie));

    root_dir->hash_table = hashArray_root;
    root_dir->current_entry_index = 0;

    cookie_dir->hash_table = hashArray_cookie;
    cookie_dir->current_entry_index = 0;

    directory_array = malloc(sizeof(Directory *) * 5);
    directory_array[0] = root_dir;
    directory_array[1] = cookie_dir;
    directory_array[2] = cafe_dir;

    assert(0 == strcmp((directory_array)[1]->hash_table[0].key, "cafe"));
    assert((directory_array)[1]->hash_table[0].valid == 1);
    assert(0 == strcmp((directory_array[1])->hash_table[0].value.name, "cafe"));

    assert(0 == strcmp(hashArray_cookie[0].key, "cafe"));
    assert(hashArray_cookie[0].valid == 1);
    assert(0 == strcmp(hashArray_cookie[0].value.name, "cafe"));

    assert(0 == strcmp(cookie_dir->hash_table[0].key, "cafe"));
    assert(cookie_dir->hash_table[0].valid == 1);
    assert(0 == strcmp(cookie_dir->hash_table[0].value.name, "cafe"));

    assert(SUCCESS_CODE == opendir1("/cookie"));

    assert(opened_dir->identifier == cookie_dir->identifier);

    dir_index = 0;
    assert(SUCCESS_CODE == opendir1("/cookie/cafe"));
    assert(opened_dir->identifier == cafe_dir->identifier);

    dir_index = 0;
    assert( FILE_NOT_FOUND == opendir1("/invalidDir"));

    dir_index = 0;
    assert( FILE_NOT_FOUND == opendir1("/cookie/invalidDir"));

    dir_index = 0;
    assert( FILE_NOT_FOUND == opendir1("/cookie/file"));

    printf("TODOS TESTES DE OPENDIR PASSARAM\n");

}


int main()
{


    //assert(NULL_POINTER_EXCEPTION == readdir1(1, &dirent2));

    //test_open_dir();

    test_hashtable();

    test_open_dir();

    return 0;
}