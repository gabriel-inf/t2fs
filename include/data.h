//
// Created by eduar on 28/05/2019.
//

#ifndef T2FS_DATA_H
#define T2FS_DATA_H

#include "t2fs.h"
#define MAX_FILES_OPENED 10
#define MAX_DIRECTORIES_NUMBER 10

#define SUPER_BLOCK_SECTOR 1

typedef struct{
    unsigned int rootDirBegin;
    unsigned int rootDirEnd;
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
    char *key;
    int valid;
}DataItem;

typedef struct {

    DataItem *hash_table; //ponteiro para o primeiro elemento da hash
    int current_entry_index; // próxima entrada da hash a ser referenciada
    DIR2 identifier;
    int opened; //1 se dir esta aberto. 0 caso contrario
    int block_number; //number of the block it is located

}Directory;

/*
 *  Estrutura mantida em memória para cada arquivo aberto
 */
typedef struct {
    unsigned int read_write_pointer; // ponteiro de leitura e escrita ver isso: nao salvo no disco
    char    name[MAX_FILE_NAME_SIZE+1]; /* Nome do arquivo cuja entrada foi lida do disco      */
    DWORD   fileSize;                   /* Numero de bytes do arquivo                          */
    DWORD   first_block;
    int valid;
}File;

//TODO inicializar as duas vars inteiras na format
//TODO perguntar carissimi quantos dir abertos podem existir

int sectors_per_block;
Directory *root_dir;
Directory opened_directories[MAX_DIRECTORIES_NUMBER];
unsigned int root_dir_sector;
int files_opened_counter;
File files_opened[MAX_FILES_OPENED];
int next_valid_blockk;
int block_data_util;


#endif //T2FS_DATA_H
