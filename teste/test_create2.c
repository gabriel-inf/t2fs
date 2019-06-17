//
// Created by Laura Corssac on 2019-06-16.
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

int main() {

    sectors_per_block = 4;

    char *dir_name = malloc(sizeof(char));
    char *file_name = malloc(sizeof(char));
    if (dir_name == NULL || file_name == NULL) return MALLOC_ERROR_EXCEPTION;

    int file_name_result = getPathAndFileName("/cookie", dir_name, file_name);
    if (file_name_result != SUCCESS_CODE) return file_name_result;

    printf("DIR NAME \n");

    puts(dir_name);
    printf("FILE NAME \n");
    puts(file_name);

    root_dir_sector = 10;

    Block *root_dir_block = malloc(sizeof(Block));
    root_dir_block->data = (unsigned char *) root_dir;
    root_dir_block->address = root_dir_sector;
    root_dir_block->next = 0;

    root_dir = malloc(sizeof(Directory));
    root_dir->identifier = 15;
    root_dir->block_number = root_dir_sector;
    root_dir->current_entry_index = 0;
    root_dir->opened = 1;

    DataItem *hashArray_root = malloc(sizeof(DataItem) * SIZE);

    int i = 0;
    for (i = 0; i < SIZE; i++) {

        hashArray_root[i].key = malloc(sizeof(MAX_FILE_NAME_SIZE));
        hashArray_root[i].valid = 0;
    }

    root_dir->hash_table = hashArray_root; //, sizeof(DataItem) * SIZE);
    assert(root_dir->hash_table != NULL);

    assert(SUCCESS_CODE == writeBlock(root_dir_sector, sectors_per_block, root_dir_block));

    assert(SUCCESS_CODE == mkdir2("/laura"));

    Block *bloco_teste = malloc(sizeof(Block));
    read_block(&bloco_teste, 17, 4);
    printf("%d\n",bloco_teste->address);
    assert(bloco_teste->address == 17);

    Directory *dir_teste = malloc(sizeof(Directory));
    dir_teste = (Directory *) bloco_teste->data;
    printf("%d\n",dir_teste->block_number);
    assert(dir_teste->block_number == 17);

    Directory *directory = malloc(sizeof(Directory));
    assert(SUCCESS_CODE == get_dir_from_path("/laura", &directory));
    assert(directory->opened == 0);
    assert(directory->current_entry_index == 0);
    printf("%d\n", directory->block_number);
    assert(directory->block_number == 17);
    assert(directory->hash_table != NULL);
    assert(directory->identifier == 10);

    Block *root_dir_block_read = malloc(sizeof(Block));
    assert(SUCCESS_CODE == read_block(&root_dir_block_read, root_dir_sector, sectors_per_block));
    Directory *root_dir_teste = malloc(sizeof(Directory));
    root_dir_teste = (Directory *) root_dir_block_read->data;
    //assert(root_dir_teste->hash_table[0].valid == 1);

    puts(root_dir->hash_table[0].key);

    assert( strcmp( root_dir_teste->hash_table[0].key, "laura") == 0);

    printf("deu bom no read block root \n");

    int result_rodrigo = mkdir2("/laura/rodrigo");
    printf("resultado do make rogrigo = %d\n", result_rodrigo);
    assert(SUCCESS_CODE == result_rodrigo);



    printf("TODOS OS TESTES PASSARAM\n");

    return 0;
}