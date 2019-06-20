
/**
*/
#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "../include/data.h"
#include "../include/error.h"
#include "../include/helper.h"
#include "../include/hashtable.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
//TODO tirar
#include <assert.h>

/*-----------------------------------------------------------------------------
Função:	Informa a identificação dos desenvolvedores do T,2FS.
-----------------------------------------------------------------------------*/
int identify2 (char *name, int size) {
	return -1;
}

void initSectord(int sectors_number) {
    sectors_per_block = sectors_number;
}

/*-----------------------------------------------------------------------------
Função:	Formata logicamente o disco virtual t2fs_disk.dat para o sistema de
		arquivos T2FS definido usando blocos de dados de tamanho 
		corresponde a um múltiplo de setores dados por sectors_per_block.
-----------------------------------------------------------------------------*/
int format2 (int sectors_per_block) {

    initSectord(sectors_per_block);
    files_opened_counter = 0;

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

    unsigned char *bitmap;

    free(mbr);

    if (DEBUG) printf("Number of sectors: %u\n", number_of_sectors);
    if (DEBUG) printf("lba_i: %d, lba_f: %d\n", lba_i, lba_f);

    SuperBloco* superBloco = malloc(SECTOR_SIZE);

    superBloco->rootDirBegin = (unsigned int) superblock_sector + (unsigned int) 1; //sectors_per_block is leaving a portion of sectors for storing this superBlock.


    Directory *root_directory = malloc(SECTOR_SIZE*sectors_per_block);
    int init_result = initialize_directory(root_directory, superBloco->rootDirBegin);


    if (init_result != SUCCESS_CODE) return init_result;

    printf("FORMAT 2 ROOT BLOCK NUMBER %d\n", root_directory->block_number);

    int root_write_result = write_dir(root_directory);
    if (root_write_result != SUCCESS_CODE) return root_write_result;

    assert(superBloco->rootDirBegin != NULL);

    superBloco->rootDirEnd = superBloco->rootDirBegin + sectors_per_block - 1;

    superBloco->bitmap_sector = superBloco->rootDirEnd + 1;

    remaining_sectors = number_of_sectors - superBloco->bitmap_sector;
    if (DEBUG) printf("***********************\n");

    number_of_blocks = (unsigned int) (remaining_sectors/sectors_per_block);
    superBloco->numberOfBlocks = number_of_blocks;
    superBloco->bitmap_size = (unsigned int) number_of_blocks/8; // Defining the size in bytes.

    superBloco->generalBlocksBegin = superblock_sector + 1;

    if (DEBUG) printSuperblock(superBloco);
    if (DEBUG) printf("%s", buffer);
    if (DEBUG) printf("remaining_sectors: %u\n", remaining_sectors);

    bitmap = malloc(SECTOR_SIZE);
    init_bitmap(bitmap, superBloco->bitmap_size);
    // printf("Bitmap sector: %d\n", superBloco->bitmap_sector);
    if (write_sector(superBloco->bitmap_sector, bitmap) != SUCCESS_CODE) return ERROR_CODE;

//    unsigned int number_of_write_sectors = (unsigned int)ceil(sizeof(superBloco)/SECTOR_SIZE);
    if (DEBUG) printf("\tnumber_of_write_sectors: %d\n", (int) sizeof(SuperBloco));
    superBlockToBuffer(superBloco, buffer);
    if (DEBUG) printf("%s\n", buffer);

    SuperBloco* superBloco2 = malloc(sizeof(SuperBloco));
    bufferToSuperBlock(buffer, superBloco2);
    if (DEBUG) printSuperblock(superBloco2);



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

    if (DEBUG) printf("BEGIN OF CREATE2\n");
    // validar o nome do arquivo
    // dá open
    // se sucesso - arquivo exite
        // da update na entrada que ja exite

    // se erro - arquivo nao existe
        // criar nova entrada

    // pegar diretorio que ele ta
    // adicionar uma entrada na hash com o tamanho = 0

    // DENTRY
    // filetype = '-'
    // fileSize = 0
    // first_block = get_free_block
    // name = fileName

    // salva o dir de novo de novo

    char *dir_name = malloc(sizeof(char));
    char *file_name = malloc(sizeof(char));
    if (dir_name == NULL || file_name == NULL) return MALLOC_ERROR_EXCEPTION;

    int file_name_result = getPathAndFileName(filename, dir_name, file_name);
    if (file_name_result != SUCCESS_CODE) return file_name_result;

    printf("DIR NAME \n");
    puts(dir_name);
    printf("FILE NAME \n");
    puts(file_name);

    int open_dir_id = opendir2(dir_name);
    if (open_dir_id < 0) return ERROR_CODE;

    Directory opened_dir = opened_directories[open_dir_id];

    DIRENT2 *file_entry = malloc(sizeof(DIRENT2));
    int get_value_result = getValue(file_name, &file_entry, opened_dir.hash_table);
    if (get_value_result != FILE_NOT_FOUND && get_value_result != SUCCESS_CODE) return get_value_result;

    if (get_value_result == SUCCESS_CODE) {

        int remove_entry_result = removeEntry(file_name, &opened_dir.hash_table);
        if (remove_entry_result != SUCCESS_CODE) return remove_entry_result;
        // TODO liberar os blocos do arquivo

    }

    file_entry->fileSize = 0;
    file_entry->fileType = '-';
    strcpy(file_entry->name, file_name);
    // TODO trocar pra func de bitmap
    file_entry->first_block = 10;
    int add_entry_result = addEntry(file_name, file_entry, &opened_dir.hash_table);
    if (add_entry_result != SUCCESS_CODE) return add_entry_result;

    //TODO salvar dir de novo no disco


	return SUCCESS_CODE;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para remover (apagar) um arquivo do disco. 
-----------------------------------------------------------------------------*/
int delete2 (char *filename) {

    // validar o nome do arquivo
    // pegar o path do dir pai

    // abrir o opendir2 pai
    // opened_dir
    // deletar a entrada na tabela

    // salva de novo o dir pai


	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função que abre um arquivo existente no disco.
-----------------------------------------------------------------------------*/
FILE2 open2 (char *filename) {

//    if (DEBUG) printf("BEGIN OF OPEN2\n");
//    if (filename == NULL) return NULL_POINTER_EXCEPTION;
//
//    const char slash[2] = "/";
//    char path_copy[MAX_FILE_NAME_SIZE];
//    strcpy(path_copy, filename);
//
//    // tokenize the path of directories
//
//    char *direct_child_pathname;
//    direct_child_pathname = strtok(path_copy, slash);
//
//    // reads from disk first parent, the root director
//
////    unsigned char *root_dir_data = malloc(SECTOR_SIZE);
////    int result_root = read_sector(root_dir_sector, root_dir_data);
////    if (result_root != SUCCESS_CODE) return result_root;
//
//    Directory *parent_directory = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
//    int root_result = get_root_directory(parent_directory);
//    if (root_result != SUCCESS_CODE) return root_result;
//
//     // = (Directory *) root_dir_data;
//
//    while (direct_child_pathname != NULL) {
//
//        DIRENT2 *entry = malloc(sizeof(DIRENT2));
//        if (entry == NULL) return MALLOC_ERROR_EXCEPTION;
//
//        // check if subdir is on parent's hash
//
//        int result = getValue(direct_child_pathname, &entry, parent_directory->hash_table);
//        if (result != SUCCESS_CODE) return result;
//
//        // Se é um arquivo e terminou o path, abre o arquivo
//
//        if (entry->fileType == '-' ) {
//
//            //check if path ended
//
//            if ( NULL != strtok(NULL, slash) ) return NOT_A_PATH_EXCEPTION;
//
//            Block *block = malloc(sizeof(Block));
//
//            if (block == NULL) return MALLOC_ERROR_EXCEPTION;
//
//            // get the logical block from the child directory
//
//            int get_dir_result = read_block(&block, entry->first_block);
//            if (get_dir_result != SUCCESS_CODE) return get_dir_result;
//
//            // continues the process for next subdirectories in path
//
//            File *new_file = (File *) block->data;
//
//            if (files_opened_counter >= MAX_FILES_OPENED && files_opened_counter >= 0) return INDEX_OUT_OF_RANGE;
//            files_opened[files_opened_counter] = *new_file;
//            files_opened_counter ++;
//
//            // retorna o index do arquivo
//
//            if (DEBUG) printf("END OF OPENDIR2\n");
//            return files_opened_counter - 1;
//
//        } else if (entry->fileType == 'd') {
//
//			direct_child_pathname = strtok(NULL, slash);
//			if (direct_child_pathname == NULL) return NOT_A_PATH_EXCEPTION;
//
//            Block *block = malloc(sizeof(Block));
//
//            if (block == NULL) return MALLOC_ERROR_EXCEPTION;
//
//            // get the logical block from the child directory
//
//            int get_dir_result = read_block(&block, entry->first_block);
//            if (get_dir_result != SUCCESS_CODE) return get_dir_result;
//
//            // continues the process for next subdirectories in path
//
//            Directory *new_dir = (Directory *) block->data;
//            memcpy(parent_directory, new_dir, sizeof(SECTOR_SIZE * sectors_per_block));
//
//
//        } else {
//
//            if (DEBUG) printf("END OF OPENDIR2\n");
//            return ERROR_CODE;
//        }
//
//    }
//
//    if (DEBUG) printf("END OF OPENDIR2\n");
    return NOT_A_PATH_EXCEPTION;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para fechar um arquivo.
-----------------------------------------------------------------------------*/
int close2 (FILE2 handle) {

    // ve se esta na lista dos abertos
    // se nao -> erro
    // se sim -> remove da lista atualizando os contadores
    // opened_files[handle] = NULL;
    // salvar o primeiro bloco no disco com todas as modificacoes do current

	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para realizar a leitura de uma certa quantidade
		de bytes (size) de um arquivo.
-----------------------------------------------------------------------------*/
int read2 (FILE2 handle, char *buffer, int size) {

    // ve se ta aberto
    // se nao -> erro
    // se sim
    // achar o bloco qur o current index esta
    // aloca um buffer de tamanho size
    // abre o bloco
    // faz conta de quanto desse size tu conseguiu ler do bloco
    // se conseguiu tudo -> show
    // se nao vai lendo dos outros blocos
    // atualizar o currentIndex do arquivo somando size


	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para realizar a escrita de uma certa quantidade
		de bytes (size) de  um arquivo.
-----------------------------------------------------------------------------*/
int write2 (FILE2 handle, char *buffer, int size) {

    // ve se ta aberto
    // se nao -> erro
    // se sim
    // achar o bloco qur o current index esta
    // duvida: perguntar se sobreescreve
    // digamos que sim
    // faz conta de quanto desse size tu conseguiu escrever do bloco
    // se conseguiu tudo -> show
    // vai alocando e escrevendo e salvdo e dando update no param "next" dos blocos
    // atualiza o current index


	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para truncar um arquivo. Remove do arquivo 
		todos os bytes a partir da posição atual do contador de posição
		(current pointer), inclusive, até o seu final.
-----------------------------------------------------------------------------*/
int truncate2 (FILE2 handle) {

    // ve se ta aberto
    // se nao -> erro
    // se sim
    // achar o bloco qur o current index esta
    // seta pra zero o que ainda esta no bloco a partir do current index
    // o resto da lista recebe 0 no bitmap
    // bloco->next = null


	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Altera o contador de posição (current pointer) do arquivo.
-----------------------------------------------------------------------------*/
int seek2 (FILE2 handle, DWORD offset) {

    // ve se ta aberto
    // se nao -> erro
    // se sim
    // arquivo->currentPointer = offset


	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um novo diretório.
 // valida pathname e obtem o nome do dir pai
    // inicializa estrutura de diretorio
    // hashtable zera -> funcao para inicializar a hash
    // ve se consegue inserir nova entrada na hashtable do pai
    // salva o pai
    // salva o filho no disco
    // atualizar o bitmap
    // nao abre o diretorio
-----------------------------------------------------------------------------*/
int mkdir2 (char *pathname) {

//    if (DEBUG) printf("\n\nBEGIN OF MKDIR 2 FOS %s\n", pathname);
//
//    char *parent_name = malloc(sizeof(char));
//    char *dir_name = malloc(sizeof(char));
//
//    if (parent_name == NULL || dir_name == NULL) return MALLOC_ERROR_EXCEPTION;
//    if (SUCCESS_CODE != getPathAndFileName(pathname, parent_name, dir_name)) return NOT_A_PATH_EXCEPTION;
//
//    if (DEBUG) printf("validou o nome\n");
//
//    Directory *parent_directory = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
//    if (parent_directory == NULL) return MALLOC_ERROR_EXCEPTION;
//
//    // se eh sem pai, insere no root
//
//    if (strcmp("", parent_name) == 0) {
//
//        if (DEBUG) printf("caiu no root como parent\n");
//
//        int root_result = get_root_directory(parent_directory);
//        if (root_result != SUCCESS_CODE) return root_result;
//
//    } else {
//
//        if (DEBUG) printf("nao caiu no root como parent\n");
//
//        int get_dir_result = get_dir_from_path(parent_name, parent_directory);
//        if (get_dir_result != SUCCESS_CODE) return get_dir_result;
//
//    }
//
//    int next_valid_block = get_free_block();
//    if (next_valid_block < 0) return FULL_BLOCKS;
//
//    Directory new_directory;
//    int init_dir_result = initialize_directory(&new_directory, next_valid_block);
//    if (init_dir_result != SUCCESS_CODE) return init_dir_result;
//
//    Block *new_block = malloc(sizeof(Block));
//    if (new_block == NULL) return MALLOC_ERROR_EXCEPTION;
//    new_block->data = (unsigned char *) &new_directory;
//    new_block->address = (unsigned int) next_valid_block;
//    new_block->next = 0;
//
//    DIRENT2 entry;
//    entry.fileType = 'd';
//    entry.first_block = (unsigned int) next_valid_block;
//    strcpy(entry.name, dir_name);
//
//    printf("ateh aqui foi\n");
//
//    int add_entry_result = addEntry(dir_name, &entry, &parent_directory->hash_table);
//    if (add_entry_result != SUCCESS_CODE) return add_entry_result;
//
//    printf("adicionou a entrada\n");
//
//    printf("child dir number = %8u\n", new_directory.block_number);
//    printf("new block add = %8u\n", new_block->address);
//
//    int write_child_result = writeBlock((unsigned int) new_block->address, new_block);
//    if (write_child_result != SUCCESS_CODE) return write_child_result;
//
//    printf("escreveu filho\n");
//
//    Block *parent_block = malloc(sizeof(Block));
//    parent_block->data = (unsigned char *) parent_directory;
//    parent_block->address = parent_directory->block_number;
//    parent_block->next = UINT_MAX;
//
//    printf("parent block number = %8u\n", parent_directory->block_number);
//    printf("parent block add = %8u\n", parent_block->address);
//    int write_parent_result = writeBlock((unsigned int) parent_directory->block_number, parent_block);
//    if (write_parent_result != SUCCESS_CODE) return write_parent_result;
//
//    Block *b = malloc(sizeof(Block));
//    assert(b != NULL);
//    assert(SUCCESS_CODE == read_block(&b, new_block->address));
//
//    Directory *d = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
//    assert(d != NULL);
//    assert(b->data != NULL);
//    assert(b->address == next_valid_block);
//    d = (Directory *) b->data;
//    assert(d != NULL);
//    assert(d->block_number == next_valid_block);
//
//    printf("escreveu pai\n");
//
//    int set_block_as_occupied(next_valid_block);
//
//    if (DEBUG) printf("END OF MKDIR 2\n\n");

	return SUCCESS_CODE;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para remover (apagar) um diretório do disco.
     // valida pathname e obtem o nome do dir pai
    // abre o pai
    // remove da hash
    // salva o pai
    // atualiza o bitmap com zero do end do filho
-----------------------------------------------------------------------------*/
int rmdir2 (char *pathname) {

//    if (DEBUG) printf("\n\nBEGIN OF RMDIR 2 FOS %s\n", pathname);
//
//    char *parent_name = malloc(sizeof(char));
//    char *dir_name = malloc(sizeof(char));
//
//    if (parent_name == NULL || dir_name == NULL) return MALLOC_ERROR_EXCEPTION;
//    if (SUCCESS_CODE != getPathAndFileName(pathname, parent_name, dir_name)) return NOT_A_PATH_EXCEPTION;
//
//    Directory *parent_dir = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
//    int get_parent_result = get_dir_from_path(parent_name, parent_dir);
//    if (get_parent_result != SUCCESS_CODE) return get_parent_result;
//
//    int removal_result = removeEntry(dir_name, &(parent_dir->hash_table));
//    if (removal_result != SUCCESS_CODE) return  removal_result;
//
//    Directory *dir = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
//    int get_dir_result = get_dir_from_path(pathname, dir);
//    if (get_dir_result != SUCCESS_CODE) return get_dir_result;
//
//    if (verifyIfDirIsOpened(dir->identifier)) {
//        int close_result = close2(dir->identifier);
//        if (close_result != SUCCESS_CODE) return ERROR_CODE;
//    }
//
//    //TODO atualizar o bitmap



	return SUCCESS_CODE;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para alterar o CP (current path)
-----------------------------------------------------------------------------*/
int chdir2 (char *pathname) {

    //openDir()

	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para obter o caminho do diretório corrente.
 Essa funçao nao sera implementada pois fazemos caminho absoluto
-----------------------------------------------------------------------------*/
int getcwd2 (char *pathname, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função que abre um diretório existente no disco.
-----------------------------------------------------------------------------*/
DIR2 opendir2 (char *pathname) {

    if (DEBUG) printf("BEGIN OF OPENDIR2\n");

    if (pathname == NULL) return NULL_POINTER_EXCEPTION;
    char *parent_name = malloc(MAX_FILE_NAME_SIZE);
    char *dir_name = malloc(MAX_FILE_NAME_SIZE);

    int get_name_result = getPathAndFileName(pathname, parent_name, dir_name);
    if (DEBUG) printf("MEIO OF OPENDIR2\n");
    if (get_name_result != SUCCESS_CODE) return get_name_result;

    Directory *directory = malloc(SECTOR_SIZE * sectors_per_block);
    int get_dir_result = get_dir_from_path(pathname, directory);
    if (get_dir_result != SUCCESS_CODE) return get_dir_result;

    unsigned int handle = 0;
    while (handle < MAX_DIRECTORIES_NUMBER && opened_directories[handle].opened == 1 ) {
        handle ++;
    }
    if (SUCCESS_CODE != validate_file_handle(handle)) return MAX_OPENED_FILES_REACHED;

    directory->opened = 1;
    memcpy(&opened_directories[handle], directory, sizeof(SECTOR_SIZE * sectors_per_block));

    free(parent_name);
    free(dir_name);
    free(directory);

    if (DEBUG) printf("END OF OPENDIR2 with handle = %u\n", handle);

    return handle;

}

/*-----------------------------------------------------------------------------
Função:	Função usada para ler as entradas de um diretório.
-----------------------------------------------------------------------------*/
int readdir2 (DIR2 handle, DIRENT2 *dentry) {

	if (DEBUG) printf("BEGIN OF READDIR\n");

	if (validate_dir_handle(handle) != SUCCESS_CODE) return DIRECTORY_NOT_OPENED;
	if (dentry == NULL) return NULL_POINTER_EXCEPTION;
	if (opened_directories[handle].opened == 0) return DIRECTORY_NOT_OPENED;

	if (DEBUG) printf("deu certo validacoes readdir\n");

    int current_index = opened_directories[handle].current_entry_index;
    if (current_index >= SIZE || current_index < 0) return INDEX_OUT_OF_RANGE;

    if (DEBUG) printf("deu certo validacoes INDEX\n");

    printf("handle = %d\n", handle);
    puts(dentry->name);
    if (opened_directories[handle].hash_table == NULL) return NULL_POINTER_EXCEPTION;
    DataItem item = opened_directories[handle].hash_table[current_index];

    if (DEBUG) printf("deu certo item\n");

    // sets the pointer to the next valid entry

    while (current_index < SIZE && item.valid == 0) {
        current_index ++;
        item = opened_directories[handle].hash_table[current_index];
    }

    if (current_index >= SIZE || item.valid != 1) return FILE_NOT_FOUND;

    DIRENT2 *current_entry = &(item.value);
    if (current_entry == NULL) return NULL_POINTER_EXCEPTION;

    *dentry = *current_entry;

    opened_directories[handle].current_entry_index = current_index + 1;

	if (DEBUG) printf("END OF READDIR2\n");

    return SUCCESS_CODE;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para fechar um diretório.
-----------------------------------------------------------------------------*/
int closedir2 (DIR2 handle) {

    // ve se ta entre os dir abertos
    // se nao -> erro
    // se sim -> fecha
    // opened_dir[handle] = NULL

    int validation = validate_dir_handle(handle);
    if (validation != SUCCESS_CODE) return validation;

    opened_directories[handle].opened = 0;

	return SUCCESS_CODE;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um caminho alternativo (softlink) com
		o nome dado por linkname (relativo ou absoluto) para um 
		arquivo ou diretório fornecido por filename.
-----------------------------------------------------------------------------*/
int ln2 (char *linkname, char *filename) {
	return -1;
}


