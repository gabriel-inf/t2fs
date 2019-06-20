//
// Created by Laura Corssac on 2019-06-06.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
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

	files_opened_counter = 0;

	// Entries definition

    DIRENT2 *cookie_dir_entry = malloc(sizeof(DIRENT2));
    cookie_dir_entry->fileType = 'd';
    cookie_dir_entry->first_block = (unsigned int) 20;
    strcpy(cookie_dir_entry->name, "cookie");

    DIRENT2 *cafe_dir_entry = malloc(sizeof(DIRENT2));
    cafe_dir_entry->fileType = 'd';
    cafe_dir_entry->first_block = (unsigned int) 30;
    strcpy(cafe_dir_entry->name, "cafe");

    DIRENT2 *file_entry = malloc(sizeof(DIRENT2));
    file_entry->fileType = '-';
    file_entry->first_block = (unsigned int) 14;
    strcpy(file_entry->name, "file");

    // Directories and Files Definition
    
    File *file = malloc(sizeof(File));
    file->read_write_pointer = (unsigned int) 10;



    Directory *cookie_dir = malloc(SECTOR_SIZE * sectors_per_block);

    initialize_directory(cookie_dir, (unsigned int) 20);
    cookie_dir->identifier = 100;

    printf("cookie number %d\n", cookie_dir->block_number);

    Directory *cafe_dir = malloc(SECTOR_SIZE * sectors_per_block);
    initialize_directory(cafe_dir, 30);
    cafe_dir->identifier = 21;

    Directory *root_dir = malloc(SECTOR_SIZE * sectors_per_block);
    initialize_directory(root_dir, 0);
    assert(SUCCESS_CODE == get_root_directory(root_dir));

    // Hashes definition and attribution

    assert(SUCCESS_CODE == addEntry("cookie", cookie_dir_entry, &root_dir->hash_table));
    assert(SUCCESS_CODE == addEntry("cafe", cafe_dir_entry, &cookie_dir->hash_table));
    assert(SUCCESS_CODE == addEntry("file", file_entry, &cookie_dir->hash_table));

    //root_dir->hash_table = hashArray_root;
    root_dir->current_entry_index = 0;
    assert(root_dir->hash_table[0].key != NULL);

    printf("begin of hash print 1\n");
    int sos =0;
    for (sos =0; sos < SIZE; sos ++) {
        puts(root_dir->hash_table[sos].key);
    }

    Block *root_block = malloc(SECTOR_SIZE * sectors_per_block);
    root_block->data = (unsigned char *) root_dir;
    root_block->address = root_dir->block_number;
    root_block->next = UINT_MAX;

    assert(SUCCESS_CODE == writeBlock(root_dir->block_number, root_block));

    //cookie_dir.hash_table = hashArray_cookie;
    //cookie_dir.current_entry_index = 0;

    Directory *root2 = malloc(SECTOR_SIZE * sectors_per_block);
    initialize_directory(root2, 0);
    assert(root2 != NULL);
    get_root_directory(root2);

    int sos2 =0;
    for (sos2 =0; sos2 < SIZE; sos2 ++) {
        puts(root2->hash_table[sos2].key);
    }

    printBits(SECTOR_SIZE * sectors_per_block, root2);

    printf("COMECANDO 1\n\n\n");
    printf("cookie numbereeee = %u\n", cookie_dir->block_number);

    // Definitions of blocks

    Block *cookieBlock = malloc(SECTOR_SIZE * sectors_per_block);

    cookieBlock->data = (unsigned char *) cookie_dir;
    printf("COMECANDO 2\n\n\n");
    printf("cookie numbereeee = %u\n", cookie_dir->block_number);

    unsigned int copy = cookie_dir->block_number;
    cookieBlock->address = copy; // cookie_dir->block_number, sizeof(int));
    cookieBlock->next = UINT_MAX;

    printf("COMECANDO 3\n\n\n");

    Block *cofeeBlock = malloc(SECTOR_SIZE * sectors_per_block);
    cofeeBlock->data = (unsigned char *) cafe_dir;
    unsigned int copy2 = cookie_dir->block_number;
    cofeeBlock->address = copy2; // cafe_dir->block_number;
    cofeeBlock->next = UINT_MAX;

    Block *fileBlock = malloc(SECTOR_SIZE * sectors_per_block);
    fileBlock->data = (unsigned char *) file;
    fileBlock->address = (unsigned int) 14;
    fileBlock->next = (unsigned int) 10;

    // Writing blocks on disk

    unsigned int fileBlockAdd = fileBlock->address;
    unsigned int cookieBlockAdd = cookieBlock->address;
    unsigned int coffeeBlockAdd = cofeeBlock->address;

    assert(SUCCESS_CODE == writeBlock(fileBlockAdd, fileBlock));
    assert(SUCCESS_CODE == writeBlock(cookieBlockAdd, cookieBlock));

    printf("COMECANDO 4\n\n\n");
    Block *teste = malloc(sectors_per_block *SECTOR_SIZE);
    assert(SUCCESS_CODE == read_block(teste, cookieBlockAdd));
    assert(teste->data != NULL);
    Directory *di_teste = (Directory *) teste->data;
    printf("COOKIE DIR ID = %d\n", di_teste->identifier);

    assert(SUCCESS_CODE == writeBlock(coffeeBlockAdd, cofeeBlock));

    // Assertions for opendir2

//    assert(0 == strcmp(hashArray_cookie[0].key, "cafe"));
//    assert(hashArray_cookie[0].valid == 1);
//    assert(0 == strcmp(hashArray_cookie[0].value.name, "cafe"));


//    assert(0 == strcmp(cookie_dir->hash_table[0].key, "cafe"));
//    assert(cookie_dir->hash_table[0].valid == 1);
//    assert(0 == strcmp(cookie_dir->hash_table[0].value.name, "cafe"));
//

    unsigned int cookie_dir_id = opendir2("/cookie");
    unsigned int zero = 0;

    printf("COMECANDO 5\n\n\n");
    printf("alo = %u\n", cookie_dir_id);
    assert(zero == cookie_dir_id);

    printf("%d\n", opened_directories[0].block_number);
    printf("%d\n", opened_directories[0].identifier);
    printf("%d\n", opened_directories[0].opened);

    int cookiDirId = opened_directories[cookie_dir_id].identifier;
    int aa = cookie_dir->identifier;

    printf("%d %d\n", cookiDirId, aa);
    //assert(cookiDirId == aa);

    printf("\n\n\nCOMECOU\n\n");


    Directory *root3 = malloc(SECTOR_SIZE * sectors_per_block);
    initialize_directory(root3, 0);
    assert(root3 != NULL);
    get_root_directory(root3);

    for (sos2 =0; sos2 < SIZE; sos2 ++) {
        puts(root3->hash_table[sos2].key);
    }

    int open_dir_result = opendir2("/cookie");
    printf("open dir result = %d\n", open_dir_result);

//    assert(1 == open_dir_result );
//    printf("opened dir identifier = %d\n", opened_directories[open_dir_result].identifier);
//    assert(opened_directories[1].identifier == cafe_dir->identifier);
//
//    printf("ate AQUIIIIIIIIIIIIIIIIII foi\n\n\n\n");
//
//    return;
//
//    //assert( FILE_NOT_FOUND == opendir2("/invalidDir"));
//
//    //assert( FILE_NOT_FOUND == opendir2("/cookie/invalidDir"));
//
//    assert( FILE_NOT_FOUND == opendir2("/cookie/file"));
//    assert( SUCCESS_CODE != opendir2(""));
//    assert( SUCCESS_CODE != opendir2("/"));
//
//
//    int open_dir_result2 = opendir2("/cookie/");
//
//    assert(2 == open_dir_result2);
//
////    DIRENT2 dentry;
////    while ( readdir2(2, &dentry) == 0 ) {
////        printf ("%c %8u %s\n", (dentry.fileType==0x02?'d':'-'), dentry.fileSize, dentry.name);
////    }
//
//	//opened_dir->current_entry_index = 0;
//    //opened_dir->hash_table[0].valid = 0;
//    //opened_dir->hash_table[1].valid = 1;
//
//    DIRENT2 dentry2;
//    int index = 0;
//    while ( readdir2(1, &dentry2) == 0 ) {
//        printf("READ DIR CALLED %d\n", index);
//        printf("\n\n\n\n");
//        printf ("%c %8u %s\n", (dentry2.fileType==0x02?'d':'-'), dentry2.fileSize, dentry2.name);
//        index ++;
//    }
//
//    printf("ASSERTIONS FOR OPEN DIR PASSED\n");
//
//
//
//    // Assertions for open2
//
//	//opened_dir->hash_table[0].valid = 1;
//    //opened_dir->hash_table[1].valid = 1;
//	int result_open_file = open2("/cookie/file");
//	printf("ERSULT OPEN FILE %d\n", result_open_file);
//    assert( 0 == result_open_file);
//    assert( files_opened[0].read_write_pointer == file->read_write_pointer);
//    assert( files_opened_counter == 1);
//
//
//
//    int result_open_file2 = open2("/cookie/file");
//    printf("ERSULT OPEN FILE %d\n", result_open_file2);
//    assert( 1 == result_open_file2);
//
//
//
//    assert( FILE_NOT_FOUND == open2("/cookie/invalidDir"));
//
//    assert( NOT_A_PATH_EXCEPTION == open2("/cookie/cafe"));
//
//    assert( NOT_A_PATH_EXCEPTION == open2(""));
//    assert( NOT_A_PATH_EXCEPTION == open2("/"));
//
//    assert(open2(NULL) == NULL_POINTER_EXCEPTION);
//    assert(opendir2(NULL) == NULL_POINTER_EXCEPTION);
//
//    printf("TODOS TESTES DE OPEN DIR E OPEN FILE PASSARAM\n");
//    return;

}

//int test_read_dir() {
//
//    // Entries definition
//
//    DIRENT2 sleep_dir_entry;
//    sleep_dir_entry.fileType = 'd';
//    sleep_dir_entry.fileSize = (unsigned int) 130;
//    strcpy(sleep_dir_entry.name, "sleep");
//
//    DIRENT2 carissimi_dir_entry;
//    carissimi_dir_entry.fileType = 'd';
//    carissimi_dir_entry.fileSize = (unsigned int) 100;
//    carissimi_dir_entry.first_block = (unsigned int) 20;
//    strcpy(carissimi_dir_entry.name, "carissimi");
//
//    DIRENT2 file_entry;
//    file_entry.fileType = '-';
//    file_entry.first_block = (unsigned int) 30;
//    strcpy(file_entry.name, "file");
//
//    DataItem *hashArray = malloc(sizeof(DataItem) * SIZE);
//
//    assert(SUCCESS_CODE == addEntry("diretorio1", &sleep_dir_entry, &hashArray));
//    assert(SUCCESS_CODE == addEntry("diretorio2", &carissimi_dir_entry, &hashArray));
//    assert(SUCCESS_CODE == addEntry("arquivo1", &file_entry, &hashArray));
//
//    Directory *opened_dir = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
//    opened_dir->current_entry_index = 0;
//    opened_dir->hash_table = hashArray;
//
//    opened_directories[0] = *opened_dir;
//
//    DIRENT2 dirent2;
//
//    assert(SUCCESS_CODE == readdir2(0, &dirent2));
//
//    assert(opened_directories[0].current_entry_index == 1);
//
//    assert(dirent2.fileType == sleep_dir_entry.fileType);
//    assert(dirent2.fileSize == sleep_dir_entry.fileSize);
//    assert(0 == strcmp(dirent2.name, sleep_dir_entry.name));
//
//    assert(dirent2.fileSize != NULL);
//    printf("size %8u\n", dirent2.fileSize);
//    printf("type %c\n", dirent2.fileType);
//
//    assert(SUCCESS_CODE == readdir2(1, &dirent2));
//    assert(opened_directories[0].current_entry_index == 2);
//    assert(dirent2.fileType == carissimi_dir_entry.fileType);
//    assert(dirent2.fileSize == carissimi_dir_entry.fileSize);
//    assert(0 == strcmp(dirent2.name, carissimi_dir_entry.name));
//
//    printf("size %8u\n", dirent2.fileSize);
//    printf("type %c\n", (dirent2.fileType));
//
//    assert(SUCCESS_CODE == readdir2(1, &dirent2));
//    assert(opened_directories[0].current_entry_index == 3);
//    assert(dirent2.fileType == file_entry.fileType);
//    assert(dirent2.fileSize == file_entry.fileSize);
//    assert(0 == strcmp(dirent2.name, file_entry.name));
//
//    printf("size %8u\n", dirent2.fileSize);
//    printf("type %c\n", (dirent2.fileType));
//
//    assert(SUCCESS_CODE != readdir2(1, &dirent2));
//
//    // se a primeira entrada eh invalida o ponteiro, retorna os dados do segundo e o ponteiro vai pro terceiro
//
//    hashArray[0].valid = 0;
//    opened_directories[0].current_entry_index = 0;
//
//    assert(SUCCESS_CODE == readdir2(1, &dirent2));
//    assert(opened_directories[0].current_entry_index == 2);
//    assert(dirent2.fileType == carissimi_dir_entry.fileType);
//    assert(dirent2.fileSize == carissimi_dir_entry.fileSize);
//    assert(0 == strcmp(dirent2.name, carissimi_dir_entry.name));
//
//    hashArray[0].valid = 0;
//    hashArray[1].valid = 0;
//    hashArray[2].valid = 0;
//    opened_directories[0].current_entry_index = 0;
//
//    assert(SUCCESS_CODE != readdir2(1, &dirent2));
//
//	printf("TODOS OS TESTES READ DIR PASSARAM\n");
//
//}



int main()
{
    
    //test_read_dir();

    //test_hashtable();

    int result = format2(4);
    printf("format result = %d\n", result);
    assert(SUCCESS_CODE == result);
//
    //test_open_dir();


    int mkdirresult = mkdir2("/cookie");
    printf("MKDIR result = %d\n", mkdirresult);
    assert( mkdirresult == SUCCESS_CODE);

    return 0;
}
