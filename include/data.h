//
// Created by eduar on 28/05/2019.
//

#ifndef T2FS_DATA_H
#define T2FS_DATA_H

#include "t2fs.h"

#define SUPER_BLOCK_SECTOR 1
#define FIRST_BLOCK 0
#define NO_NEXT 9999
#define SIZE 20
#define MAX_DIRECTORIES_NUMBER 10
#define MAX_FILES_OPENED 10

typedef struct{
    unsigned int generalBlocksBegin;
    unsigned int numberOfBlocks;
    unsigned int bitmap_sector;
    unsigned int bitmap_size; // tamanho em bytes do bitmap

}SuperBloco;

typedef struct {
    unsigned int address;   // endereço do bloco (block number) se é o bloco 0, 1, 2 , 3...
    unsigned int next;      // próximo bloco a ser escrito
    unsigned char *data;    // aqui a gente vai ter que fazer a conta pro malloc do data quando soubermos o nr de setores por bloco
} Block;

typedef struct {
    DIRENT2 value;
    char key[MAX_FILE_NAME_SIZE+1];
    int valid;
}DataItem;

typedef struct {

    // TODO trocar numero max de entradas
    DataItem *hash_table; //ponteiro para o primeiro elemento da hash

    unsigned int current_entry_index; // próxima entrada da hash a ser referenciada
    DIR2 identifier;
    unsigned int opened; //1 se dir esta aberto. 0 caso contrario
    unsigned int block_number; //number of the block it is located

}Directory;

int sectors_per_block;
int files_opened_counter;
Directory opened_directories[MAX_DIRECTORIES_NUMBER];

#endif //T2FS_DATA_H
