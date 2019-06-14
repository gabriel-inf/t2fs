
/**
*/
#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "../include/data.h"
#include "../include/error.h"
#include "../include/helper.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//Funções que podem ir para um arquivo de suporte:

int initBitMap(unsigned char *bitMap, unsigned int bitMapSize) {
    int i = 0;
    for(i = 0; i < bitMapSize; i++){
        *(bitMap + (i* sizeof(char))) = 0;
    }
    return 0;
}


//END FUNÇÕES DE SUPORTE--------------------------------------------------------------------

/*-----------------------------------------------------------------------------
Função:	Informa a identificação dos desenvolvedores do T2FS.
-----------------------------------------------------------------------------*/
int identify2 (char *name, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Formata logicamente o disco virtual t2fs_disk.dat para o sistema de
		arquivos T2FS definido usando blocos de dados de tamanho 
		corresponde a um múltiplo de setores dados por sectors_per_block.
-----------------------------------------------------------------------------*/
int format2 (int sectors_per_block) {

    BYTE *mbr = (BYTE *) malloc(sizeof(BYTE) * SECTOR_SIZE);
    // lê o primeiro setor do disco que é reservado para o MBE
    if(read_sector((unsigned int) 0, mbr) != SUCCESS_CODE) return FAILED_TO_READ_SECTOR;

    BYTE buffer[SECTOR_SIZE] = {0};
    unsigned int disk_version = (unsigned int)(mbr[0] | ( mbr[1] << 8 ));
    printf("-> Disk version: %x\n", disk_version);
    printf("***About partition 0***\n");

    unsigned int lba_i = (unsigned int)(mbr[8] | mbr[9] << 8 | mbr[10] << 16 | mbr[11] << 24) ;
    unsigned int lba_f = mbr[12] | mbr[13] << 8 | mbr[14] << 16| mbr[15] << 24; //Assuming that it is little endian
    unsigned int number_of_sectors = lba_f - lba_i + 1;

    unsigned int superblock_sector = lba_i;// o superbloco vai ocupar o primeiro setor da partição
    unsigned int remaining_sectors = 0;
    unsigned int number_of_blocks = 0;

    unsigned char *bitmap;

    free(mbr);

    printf("Number of sectors: %u\n", number_of_sectors);
    printf("lba_i: %d, lba_f: %d\n", lba_i, lba_f);

    SuperBloco* superBloco = malloc(sizeof(SuperBloco));
    superBloco->rootDirBegin = (unsigned int) superblock_sector +
                               (unsigned int) 1; //sectors_per_block is leaving a portion of sectors for storing this superBlock.
    superBloco->rootDirEnd = superBloco->rootDirBegin + 16 * sectors_per_block - 1;
    superBloco->bitmap_sector = superBloco->rootDirEnd + 1;

    remaining_sectors = number_of_sectors - superBloco->bitmap_sector;
    printf("***********************\n");

    number_of_blocks = (unsigned int) (remaining_sectors/sectors_per_block);
    superBloco->numberOfBlocks = number_of_blocks;
    superBloco->bitmap_size = (unsigned int) number_of_blocks/8; // Defining the size in bytes.

    superBloco->generalBlocksBegin = superblock_sector + 1;

    printSuperblock(superBloco);
    printf("%s", buffer);
    printf("remaining_sectors: %u\n", remaining_sectors);

    bitmap = malloc(sizeof(char)*SECTOR_SIZE);
    initBitMap(bitmap, superBloco->bitmap_size);
    assert(write_sector(superBloco->bitmap_sector, bitmap) == SUCCESS_CODE);

//    unsigned int number_of_write_sectors = (unsigned int)ceil(sizeof(superBloco)/SECTOR_SIZE);
    printf("\tnumber_of_write_sectors: %d\n", (int) sizeof(SuperBloco));
    superBlockToBuffer(superBloco, buffer);
    printf("%s\n", buffer);

    SuperBloco* superBloco2 = malloc(sizeof(SuperBloco));
    bufferToSuperBlock(buffer, superBloco2);
    printSuperblock(superBloco2);



    // o superblock cabe em apenas 1 setor lógico. Daí precisamos definir qual setor vai ser esse.
    assert(write_sector(superblock_sector, buffer) == SUCCESS_CODE);

    return SUCCESS_CODE;

}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um novo arquivo no disco e abrí-lo,
		sendo, nesse último aspecto, equivalente a função open2.
		No entanto, diferentemente da open2, se filename referenciar um 
		arquivo já existente, o mesmo terá seu conteúdo removido e 
		assumirá um tamanho de zero bytes.
-----------------------------------------------------------------------------*/
FILE2 create2 (char *filename) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para remover (apagar) um arquivo do disco. 
-----------------------------------------------------------------------------*/
int delete2 (char *filename) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função que abre um arquivo existente no disco.
-----------------------------------------------------------------------------*/
FILE2 open2 (char *filename) {

    if (DEBUG) printf("BEGIN OF __PRETTY_FUNCTION__\n");

    const char slash[2] = "/";
    char path_copy[MAX_FILE_NAME_SIZE];
    strcpy(path_copy, pathname);

    // tokenize the path of directories

    char *direct_child_pathname;
    direct_child_pathname = strtok(path_copy, slash);

    // reads from disk first parent, the root director

    unsigned char *root_dir_data = malloc(SECTOR_SIZE);
    int result_root = read_sector(root_dir_sector, root_dir_data);
    if (result_root != SUCCESS_CODE) return result_root;

    Directory *parent_directory = (Directory *) root_dir_data;

    while (direct_child_pathname != NULL) {

        DIRENT2 *entry = malloc(sizeof(DIRENT2));
        if (entry == NULL) return MALLOC_ERROR_EXCEPTION;

        // check if subdir is on parent's hash

        int result = getValue(direct_child_pathname, &entry, parent_directory->hash_table);
        if (result != SUCCESS_CODE) return result;

        // Se é um arquivo e terminou o path, abre o arquivo

        if (entry->fileType == '-' ) {

            //check if path ended

            if ( NULL != strtok(NULL, slash) ) return NOT_A_PATH_EXCEPTION;

            Block *block = malloc(sizeof(Block));

            if (block == NULL) return MALLOC_ERROR_EXCEPTION;

            // get the logical block from the child directory

            int get_dir_result = read_block(&block, entry->firstCluster, sectors_per_block);
            if (get_dir_result != SUCCESS_CODE) return get_dir_result;

            // continues the process for next subdirectories in path

            File *new_file = (File *) block->data;

            if (files_opened_counter >= MAX_FILES_OPENED) return MAX_OPENED_FILES_REACHED;
            files_opened[files_opened_counter] = *new_file;
            files_opened_counter ++;

            //TODO ASK ASC
            return SUCCESS_CODE;

        } else if (entry->fileType == 'd') {

            if (block == NULL) return MALLOC_ERROR_EXCEPTION;

            // get the logical block from the child directory

            int get_dir_result = read_block(&block, entry->firstCluster, sectors_per_block);
            if (get_dir_result != SUCCESS_CODE) return get_dir_result;

            // continues the process for next subdirectories in path

            Directory *new_dir = (Directory *) block->data;
            memcpy(parent_directory, new_dir, sizeof(Directory));

            direct_child_pathname = strtok(NULL, slash);

        } else {

            return ERROR_CODE;
        }

    }

    return SUCCESS_CODE;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para fechar um arquivo.
-----------------------------------------------------------------------------*/
int close2 (FILE2 handle) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para realizar a leitura de uma certa quantidade
		de bytes (size) de um arquivo.
-----------------------------------------------------------------------------*/
int read2 (FILE2 handle, char *buffer, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para realizar a escrita de uma certa quantidade
		de bytes (size) de  um arquivo.
-----------------------------------------------------------------------------*/
int write2 (FILE2 handle, char *buffer, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para truncar um arquivo. Remove do arquivo 
		todos os bytes a partir da posição atual do contador de posição
		(current pointer), inclusive, até o seu final.
-----------------------------------------------------------------------------*/
int truncate2 (FILE2 handle) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Altera o contador de posição (current pointer) do arquivo.
-----------------------------------------------------------------------------*/
int seek2 (FILE2 handle, DWORD offset) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um novo diretório.
-----------------------------------------------------------------------------*/
int mkdir2 (char *pathname) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para remover (apagar) um diretório do disco.
-----------------------------------------------------------------------------*/
int rmdir2 (char *pathname) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para alterar o CP (current path)
-----------------------------------------------------------------------------*/
int chdir2 (char *pathname) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para obter o caminho do diretório corrente.
-----------------------------------------------------------------------------*/
int getcwd2 (char *pathname, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função que abre um diretório existente no disco.
-----------------------------------------------------------------------------*/
DIR2 opendir2 (char *pathname) {

    if (DEBUG) printf("BEGIN OF __PRETTY_FUNCTION__\n");

    const char slash[2] = "/";
    char path_copy[MAX_FILE_NAME_SIZE];
    strcpy(path_copy, pathname);

    // tokenize the path of directories

    char *direct_child_pathname;
    direct_child_pathname = strtok(path_copy, slash);

    // reads from disk first parent, the root director

    unsigned char *root_dir_data = malloc(SECTOR_SIZE);
    int result_root = read_sector(root_dir_sector, root_dir_data);
    if (result_root != SUCCESS_CODE) return result_root;


    Directory *parent_directory = (Directory *) root_dir_data;

    while (direct_child_pathname != NULL) {

        DIRENT2 *entry = malloc(sizeof(DIRENT2));
        if (entry == NULL) return MALLOC_ERROR_EXCEPTION;

        // check if subdir is on parent's hash

        int result = getValue(direct_child_pathname, &entry, parent_directory->hash_table);
        if (result != SUCCESS_CODE) return result;
        if (entry->fileType != 'd') return FILE_NOT_FOUND;

        Block *block = malloc(sizeof(Block));

        if (block == NULL) return MALLOC_ERROR_EXCEPTION;

        // get the logical block from the child directory

        int get_dir_result = read_block(&block, entry->firstCluster, sectors_per_block);
        if (get_dir_result != SUCCESS_CODE) return get_dir_result;

        // continues the process for next subdirectories in path

        Directory *new_dir = (Directory *) block->data;
        memcpy(parent_directory, new_dir, sizeof(Directory));

        direct_child_pathname = strtok(NULL, slash);

    }

    opened_dir = parent_directory;

    //TODO change the return value
    // nao sei pra que serve esse DIR2, alguem??
    return SUCCESS_CODE;

}

/*-----------------------------------------------------------------------------
Função:	Função usada para ler as entradas de um diretório.
-----------------------------------------------------------------------------*/
int readdir2 (DIR2 handle, DIRENT2 *dentry) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para fechar um diretório.
-----------------------------------------------------------------------------*/
int closedir2 (DIR2 handle) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um caminho alternativo (softlink) com
		o nome dado por linkname (relativo ou absoluto) para um 
		arquivo ou diretório fornecido por filename.
-----------------------------------------------------------------------------*/
int ln2 (char *linkname, char *filename) {
	return -1;
}


