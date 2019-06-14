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
#include "../include/helper.h"

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

    assert(SUCCESS_CODE != addEntry(gabriel, &entry1, &hashArray));
    assert(SUCCESS_CODE != addEntry(guerra, &entry1, &hashArray));

    printf("TODOS TESTES DE HASHTABLE PASSARAM\n");
}



void test_open_dir() {

    //root -> cookie_dir -> cafe_dir
    //          |-> file

	sectors_per_block = 4;
	root_dir_sector = 10;

	// Entries definition

    DIRENT2 root_dir_entry;
    root_dir_entry.fileType = 'd';
    strcpy(root_dir_entry.name, "root");

    DIRENT2 cookie_dir_entry;
    cookie_dir_entry.fileType = 'd';
    cookie_dir_entry.firstCluster = (unsigned int) 20;
    strcpy(cookie_dir_entry.name, "cookie");

    DIRENT2 cafe_dir_entry;
    cafe_dir_entry.fileType = 'd';
    cafe_dir_entry.firstCluster = (unsigned int) 30;
    strcpy(cafe_dir_entry.name, "cafe");

    DIRENT2 file_entry;
    file_entry.fileType = '-';
    file_entry.firstCluster = (unsigned int) 14;
    strcpy(file_entry.name, "file");

    // Directories and Files Definition
    
    File *file = malloc(sizeof(File));
    file->read_write_pointer = (unsigned int) 10;

    root_dir = malloc(sizeof(Directory));
    root_dir->identifier = 15;

    Directory *cookie_dir = malloc(sizeof(Directory));
    cookie_dir->identifier = 100;

    Directory *cafe_dir = malloc(sizeof(Directory));
    cafe_dir->identifier = 21;

    // Hashes definition and attribution

    DataItem *hashArray_root = malloc(sizeof(DataItem) * SIZE);
    DataItem *hashArray_cookie = malloc(sizeof(DataItem) * SIZE);

    assert(SUCCESS_CODE == addEntry("cookie", &cookie_dir_entry, &hashArray_root));
    assert(SUCCESS_CODE == addEntry("cafe", &cafe_dir_entry, &hashArray_cookie));
    assert(SUCCESS_CODE == addEntry("file", &file_entry, &hashArray_cookie));

    root_dir->hash_table = hashArray_root;
    root_dir->current_entry_index = 0;

    cookie_dir->hash_table = hashArray_cookie;
    cookie_dir->current_entry_index = 0;
    
    printf("ate agora foi\n");

    // Write Root Directory

    write_sector(root_dir_sector, (unsigned char *) root_dir);
    
    printf("deu bom write do root\n");

    // Definitions of blocks

    Block *cookieBlock = malloc(sizeof(Block));
    cookieBlock->data = (unsigned char *) cookie_dir;
    cookieBlock->address = 20;
    cookieBlock->next = 2;
    
    Block *cofeeBlock = malloc(sizeof(Block));
    cofeeBlock->data = (unsigned char *) cafe_dir;
    cofeeBlock->address = 30;
    cofeeBlock->next = 2;

    Block *fileBlock = malloc(sizeof(Block));
    fileBlock->data = (unsigned char *) file;
    fileBlock->address = 14;
    fileBlock->next = 10;

    // Writing blocks on disk

    assert(SUCCESS_CODE == writeBlock(14, sectors_per_block, fileBlock));
    assert(SUCCESS_CODE == writeBlock(20, sectors_per_block, cookieBlock));
    assert(SUCCESS_CODE == writeBlock(30, sectors_per_block, cofeeBlock));
    
    printf("deu bom write do bloco\n");

    // Assertions for opendir2

    assert(0 == strcmp(hashArray_cookie[0].key, "cafe"));
    assert(hashArray_cookie[0].valid == 1);
    assert(0 == strcmp(hashArray_cookie[0].value.name, "cafe"));

    assert(0 == strcmp(cookie_dir->hash_table[0].key, "cafe"));
    assert(cookie_dir->hash_table[0].valid == 1);
    assert(0 == strcmp(cookie_dir->hash_table[0].value.name, "cafe"));

	printf("ate aqui foi\n");
	
    assert(SUCCESS_CODE == opendir2("/cookie"));

    assert(opened_dir->identifier == cookie_dir->identifier);

    int open_dir_result = opendir2("/cookie/cafe");
    printf("open dir result = %d\n", open_dir_result);
    
    assert(SUCCESS_CODE == open_dir_result );
    printf("opened dir identifier = %d\n", opened_dir->identifier);
    assert(opened_dir->identifier == cafe_dir->identifier);
    
    assert( FILE_NOT_FOUND == opendir2("/invalidDir"));

    assert( FILE_NOT_FOUND == opendir2("/cookie/invalidDir"));

    assert( FILE_NOT_FOUND == opendir2("/cookie/file"));
    assert( NOT_A_PATH_EXCEPTION == opendir2(""));
    assert( NOT_A_PATH_EXCEPTION == opendir2("/"));
    
    printf("ASSERTIONS FOR OPEN DIR PASSED\n");
    
    // Assertions for open2

	int result_open_file = open2("/cookie/file");
	printf("ERSULT OPEN FILE %d\n", result_open_file);
    assert( SUCCESS_CODE == result_open_file);
    assert( files_opened[0].read_write_pointer == file->read_write_pointer);
    assert( files_opened_counter == 1);
    assert( FILE_NOT_FOUND == open2("/cookie/invalidDir"));
    assert( NOT_A_PATH_EXCEPTION == open2("/cookie/cafe"));
    assert( NOT_A_PATH_EXCEPTION == open2(""));
    assert( NOT_A_PATH_EXCEPTION == open2("/"));


    printf("TODOS TESTES DE OPEN DIR E OPEN FILE PASSARAM\n");

}

int test_read_dir() {

    sectors_per_block = 4;
    root_dir_sector = 10;

    // Entries definition

    DIRENT2 sleep_dir_entry;
    sleep_dir_entry.fileType = 'd';
    strcpy(sleep_dir_entry.name, "sleep");

    DIRENT2 carissimi_dir_entry;
    carissimi_dir_entry.fileType = 'd';
    carissimi_dir_entry.firstCluster = (unsigned int) 20;
    strcpy(carissimi_dir_entry.name, "carissimi");

    DIRENT2 file_entry;
    file_entry.fileType = '-';
    file_entry.firstCluster = (unsigned int) 30;
    strcpy(cafe_dir_entry.name, "cafe");

    DataItem *hashArray = malloc(sizeof(DataItem) * SIZE);

    assert(SUCCESS_CODE == addEntry("diretorio1", &sleep_dir_entry, &hashArray));
    assert(SUCCESS_CODE == addEntry("diretorio2", &carissimi_dir_entry, &hashArray));
    assert(SUCCESS_CODE == addEntry("arquivo1", &file_entry, &hashArray));

    opened_dir = malloc(sizeof(Directory));
    opened_dir->current_entry_index = 0;
    opened_dir->hash_table = hashArray[0];

    DIRENT2 dirent2;

    assert(SUCCESS_CODE == readdir1(1, &dirent2));
    assert(direcory_mock->current_entry_index == 1);
    assert(dirent2.fileType == entry2->fileType);
    assert(dirent2.fileSize == entry2->fileSize);
    assert(0 == strcmp(dirent2.name, entry2->name));

    printf("size %8u\n", dirent2.fileSize);
    printf("type %c\n", (dirent2.fileType==0x02?'d':'-'));

    assert(SUCCESS_CODE == readdir1(1, &dirent2));
    assert(direcory_mock->current_entry_index == 2);

    printf("size %8u\n", dirent2.fileSize);
    printf("type %c\n", (dirent2.fileType==0x02?'d':'-'));

    assert(dirent2.fileSize == entry1.fileSize);
    assert(dirent2.fileType == entry1.fileType);
    assert(0 == strcmp(dirent2.name, entry1.name));
}


int main()
{


    //assert(NULL_POINTER_EXCEPTION == readdir1(1, &dirent2));

    test_read_dir();

    //test_hashtable();

    //test_open_dir();

    return 0;
}
