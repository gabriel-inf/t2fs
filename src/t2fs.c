
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

void init_global_vars(int sectors_number) {
    sectors_per_block = sectors_number;
    files_opened_counter = 0;
}

int format2 (int sectors_per_block) {

    init_global_vars(sectors_per_block);

    BYTE *mbr = (BYTE *) malloc(SECTOR_SIZE);
    // lê o primeiro setor do disco que é reservado para o MBE
    if(read_sector((unsigned int) 0, mbr) != SUCCESS_CODE) return FAILED_TO_READ_SECTOR;

    BYTE buffer[SECTOR_SIZE] = {0};
    unsigned int disk_version = (unsigned int)(mbr[0] | ( mbr[1] << 8 ));
    if (DEBUG) printf("-> Disk version: %x\n", disk_version);
    if (DEBUG) printf("***About partition 0***\n");

    unsigned int lba_i = (unsigned int)(mbr[8] | mbr[9] << 8 | mbr[10] << 16 | mbr[11] << 24) ;
    unsigned int lba_f = mbr[12] | mbr[13] << 8 | mbr[14] << 16| mbr[15] << 24; //Assuming that it is little endian
    unsigned int number_of_sectors = lba_f - lba_i + 1;

    unsigned int superblock_sector = lba_i; // o superbloco vai ocupar o primeiro setor da partição
    unsigned int remaining_sectors = 0;
    unsigned int number_of_blocks = 0;


    free(mbr);

    if (DEBUG) printf("Number of sectors: %u\n", number_of_sectors);
    if (DEBUG) printf("lba_i: %d, lba_f: %d\n", lba_i, lba_f);

    SuperBloco* superBloco = malloc(SECTOR_SIZE);

    superBloco->bitmap_sector = (unsigned int) superblock_sector + (unsigned int) 1;

    remaining_sectors = number_of_sectors - superBloco->bitmap_sector;

    if (DEBUG) printf("***********************\n");

    number_of_blocks = (unsigned int) (remaining_sectors/sectors_per_block);
    superBloco->numberOfBlocks = number_of_blocks;
    superBloco->bitmap_size = (unsigned int) number_of_blocks/8; // Defining the size in bytes.

    superBloco->generalBlocksBegin = superBloco->bitmap_sector + 1;

    if (DEBUG) printSuperblock(superBloco);
    if (DEBUG) printf("%s", buffer);
    if (DEBUG) printf("remaining_sectors: %u\n", remaining_sectors);

    unsigned char *bitmap;
    bitmap = malloc(SECTOR_SIZE);

    init_bitmap(bitmap, superBloco->bitmap_size);

    printf("format2. Testing if all the bitmap is entire free: \n");
    int k;
    for (k = 0; k<superBloco->bitmap_sector; k++) {

        assert(bitmap[k] == 0);
        printf("b%d - OK; ", k);

    }
    printf("Brooooooooo, tudo ta certo: \n\n");

    printf("Bitmap sector: %d\n", superBloco->bitmap_sector);
    if (write_sector(superBloco->bitmap_sector, bitmap) != SUCCESS_CODE) return ERROR_CODE;

    Directory *root_directory = malloc(SECTOR_SIZE*sectors_per_block);
    initialize_directory(root_directory, FIRST_BLOCK);

    Block *root_dir_block = malloc(SECTOR_SIZE*sectors_per_block);
    if(root_dir_block == NULL) return MALLOC_ERROR_EXCEPTION;

    root_dir_block->data = (unsigned char *) root_directory;

    root_dir_block->address = FIRST_BLOCK;
    root_dir_block->next= NO_NEXT;

    int write_block_result = writeBlock(FIRST_BLOCK, root_dir_block);

    printf("write block\n");

    if(write_block_result != SUCCESS_CODE) return write_block_result;
    int occupy_first_block_result = set_block_as_occupied(FIRST_BLOCK);
    if(occupy_first_block_result != SUCCESS_CODE) return occupy_first_block_result;

//  unsigned int number_of_write_sectors = (unsigned int)ceil(sizeof(superBloco)/SECTOR_SIZE);
    if (DEBUG) printf("\tnumber_of_write_sectors: %d\n", (int) sizeof(SuperBloco));
    superBlockToBuffer(superBloco, buffer);
    if (DEBUG) printf("%s\n", buffer);





    // o superblock cabe em apenas 1 setor lógico. Daí precisamos definir qual setor vai ser esse.
    if (write_sector(superblock_sector, buffer) != SUCCESS_CODE) return ERROR_CODE;

    printf("SAIU DA FORMAT2\n");

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
	return -1;
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
	return -1;
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


