
/**
*/
#include "apidisk.h"
#include "t2fs.h"
#include <math.h>


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

    BYTE mbr[SECTOR_SIZE];

    if(read_sector(0, &mbr) == 0){

        int iterator = 0;

        unsigned int lba_i = mbr[8] | mbr[9] << 8 | mbr[10] << 16 | mbr[11] << 24 ;

        unsigned int lba_f = mbr[12] | mbr[13] << 8 | mbr[14] << 16| mbr[15] << 24; //Assuming that it is little endian

        unsigned int number_of_sectors = lba_f - lba_i;

        unsigned int remaining_sectors = 0;

        unsigned int remaining_bytes = 0;

        unsigned int number_of_blocks = 0;


        unsigned int bytes_per_block = sectors_per_block*SECTOR_SIZE;

        char* bitmap;

        SuperBloco* superBloco = malloc(sizeof(SuperBloco));

        superBloco->rootDirBegin = sectors_per_block + 1; //sectors_per_block is leaving a portion of sectors for storing this superBlock.
        superBloco->rootDirEnd = superBloco->rootDirBegin + 128; //Todo: Define the size of the root dir. |32KB| 32KB/256B = 128 sectors.

        remaining_sectors = number_of_sectors - superBloco->rootDirEnd;

        remaining_bytes = SECTOR_SIZE*remaining_sectors;

        number_of_blocks = floor((remaining_bytes*8)/(8*bytes_per_block + 1));

        superBloco->bitmap_size = number_of_blocks/8; // Defining the size in bytes.

        bitmap = malloc(sizeof(superBloco->bitmap_size));
        superBloco->bitmap = bitmap;

        initBitMap(&superBloco->bitmap, superBloco->bitmap_size);


        superBloco->generalBlocksBegin = ;//Todo: Define the size of the blocks area based on the number of sectors and

        superBloco->numberOfBlocks = number_of_blocks;

        unsigned int number_of_write_sectors = ceil(sizeof(superBloco)/SECTOR_SIZE);

        if(number_of_write_sectors <= sectors_per_block){
            for(iterator = 1; iterator <= number_of_write_sectors; iterator++){
                if(write_sector(iterator, /*data portion*/) != 0){ //Todo: Find a way for divide the super block into sectors so we can write this sectors
                    return -1;
                }
            }

            return 0;
        }else {
            return -1;
        }




    }




	return -1;
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

int initBitMap(char* bitMap, unsigned int bitMapSize){
    int i = 0;
    for(i = 0; i < bitMapSize; i++){
        *(bitMap + i) = 0;
    }
    return 0;
}


