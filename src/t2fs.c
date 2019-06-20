
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

    SuperBloco* superBloco = malloc(sizeof(SuperBloco));
    superBloco->rootDirBegin = (unsigned int) superblock_sector +
                               (unsigned int) 1; //sectors_per_block is leaving a portion of sectors for storing this superBlock.
    superBloco->rootDirEnd = superBloco->rootDirBegin + 16 * sectors_per_block - 1;
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
        // TODO liberar os blocos do arquivo gabe on it

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
// validar o nome do arquivo - OK
// pegar o path do dir pai - OK
// abrir o opendir2 pai - OK
// opened_dir - OK
// deletar a entrada na tabela - OK
// salva de novo o dir pai - ok
// percorrer o encadeamento dos blocos, marcando cada bit de endereço como 0 no bitmap

int delete2 (char *filename) {

    char *dir_name = malloc(sizeof(char));
    char *file_name = malloc(sizeof(char));
    if (dir_name == NULL || file_name == NULL) return MALLOC_ERROR_EXCEPTION;

    int file_name_result = getPathAndFileName(filename, dir_name, file_name);
    if (file_name_result != SUCCESS_CODE) return file_name_result;

    printf(" [DELETE] DIR NAME \n");
    puts(dir_name);
    printf(" [DELETE] FILE NAME \n");
    puts(file_name);

    int open_dir_id = opendir2(dir_name);
    if (open_dir_id < 0) return ERROR_CODE;

    Directory opened_dir = opened_directories[open_dir_id];
    DIRENT2 *entry = malloc(sizeof(DIRENT2));
    if (getValue(filename, &entry, opened_dir.hash_table) != SUCCESS_CODE) return ERROR_CODE;
    if (removeEntry(file_name, &(opened_dir.hash_table)) != SUCCESS_CODE) return ERROR_CODE;

    free_file_blocks(get_file_handler(filename));

    // TODO: write_dir(opened_dir);

	return SUCCESS_CODE;
}

/*-----------------------------------------------------------------------------
Função:	Função que abre um arquivo existente no disco.
-----------------------------------------------------------------------------*/
FILE2 open2 (char *filename) {

    if (DEBUG) printf("BEGIN OF OPEN2\n");
    if (filename == NULL) return NULL_POINTER_EXCEPTION;

    const char slash[2] = "/";
    char path_copy[MAX_FILE_NAME_SIZE];
    strcpy(path_copy, filename);

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

            int get_dir_result = read_block(&block, entry->first_block, sectors_per_block);
            if (get_dir_result != SUCCESS_CODE) return get_dir_result;

            // continues the process for next subdirectories in path

            File *new_file = (File *) block->data;

            if (files_opened_counter >= MAX_FILES_OPENED && files_opened_counter >= 0) return INDEX_OUT_OF_RANGE;
            files_opened[files_opened_counter] = *new_file;
            files_opened_counter ++;

            // retorna o index do arquivo

            if (DEBUG) printf("END OF OPENDIR2\n");
            return files_opened_counter - 1;

        } else if (entry->fileType == 'd') {

			direct_child_pathname = strtok(NULL, slash);
			if (direct_child_pathname == NULL) return NOT_A_PATH_EXCEPTION;

            Block *block = malloc(sizeof(Block));

            if (block == NULL) return MALLOC_ERROR_EXCEPTION;

            // get the logical block from the child directory

            int get_dir_result = read_block(&block, entry->first_block, sectors_per_block);
            if (get_dir_result != SUCCESS_CODE) return get_dir_result;

            // continues the process for next subdirectories in path

            Directory *new_dir = (Directory *) block->data;
            memcpy(parent_directory, new_dir, sizeof(Directory));


        } else {

            if (DEBUG) printf("END OF OPENDIR2\n");
            return ERROR_CODE;
        }

    }

    if (DEBUG) printf("END OF OPENDIR2\n");
    return NOT_A_PATH_EXCEPTION;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para fechar um arquivo.
-----------------------------------------------------------------------------*/

// ve se esta na lista dos abertos
// se nao -> erro ok
// se sim -> remove da lista atualizando os contadores
// opened_files[handle] = NULL; ok
// salvar o primeiro bloco no disco com todas as modificacoes do current dir(não precisa pois a única coisa que temos
//  na memória é o current pointer dentro da file). O resto tudo já tá no disco, nenhum dado se perde
int close2 (FILE2 handle) {

    if (files_opened[handle] == NULL) return FILE_NOT_FOUND;
    File file = files_opened[handle];
    opened_files[handle] = NULL;
    files_opened_counter--;

	return SUCCESS_CODE;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para realizar a leitura de uma certa quantidade
		de bytes (size) de um arquivo.
-----------------------------------------------------------------------------*/
int read2 (FILE2 handle, char *buffer, int size) {

    // ve se ta aberto ok
    // se nao -> erro ok
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
// ve se ta aberto ok
// se nao -> erro ok
// se sim
// achar o bloco qur o current index esta
// faz conta de quanto desse size tu conseguiu escrever do bloco ok
// se conseguiu tudo -> show ok
// vai alocando e escrevendo e salvdo e dando update no param "next" dos blocos ok
// atualiza o current index ok
int write2 (FILE2 handle, char *buffer, int size) {

    File file;
    unsigned int blocks_to_write, current_block, last_block_size, block_address, i, old_address, write_pointer_block, write_pointer_offset;
    int size_to_write, size_to_write_first;
    block_data_util = SECTOR_SIZE * sectors_per_block - sizeof(unsigned int) * 2;

    // pega o arquivo aberto
    int getfile_result = get_file_by_handler(handle, &file);
    if (getfile_result != SUCCESS_CODE) return getfile_result;

    // pegando o bloco que ta reader pointer
    int result_getblock_index = get_block_and_position_by_index(file.read_write_pointer, sectors_per_block, &write_pointer_block, &write_pointer_offset);
    if (result_getblock_index != SUCCESS_CODE) return result_getblock_index;
    // sei que se trata do enezimo bloco do arquivo. agora tenho que iterar na lista até ele

    Block *first_block = malloc(SECTOR_SIZE * sectors_per_block);
    file.first_block

    if (is_block_free(block_address)) return BLOCK_IS_FREE;
    Block *first_block = malloc(SECTOR_SIZE * sectors_per_block);
    int result_read_block = read_block(&first_block, write_pointer_block, sectors_per_block);
    if (result_read_block != SUCCESS_CODE) return result_getblock_index;

    size_to_write_first = (block_data_util - write_pointer_offset);
    memcpy(first_block->data + write_pointer_offset, buffer, size_to_write_first);
    size_to_write = size - size_to_write_first;

    blocks_to_write = (int) size_to_write / block_data_util;
    last_block_size = size_to_write % block_data_util;

    char *block_buffer = malloc(block_data_util);

    for (current_block = 0; current_block < blocks_to_write; current_block++) {

        Block *block = malloc(sectors_per_block * SECTOR_SIZE);
        memcpy(block_buffer, buffer + (current_block * block_data_util), block_data_util);
        block_address = get_free_block();

        if (block_address == FULL_BLOCKS) return ERROR_CODE;

        if (current_block != 0) {
            Block *last_block = malloc(sectors_per_block * SECTOR_SIZE);
            read_block(&last_block, old_address, sectors_per_block);
            last_block->next = block_address;
            writeBlock(last_block->address, sectors_per_block, last_block);
            free(last_block);
        }

        block->address = block_address;
        block->data = block_buffer;

        writeBlock(block_address, sectors_per_block, block);
        file.read_write_pointer += block_data_util;
        old_address = block_address;
        free(block);
    }

    if (last_block_size != 0) {
        for (i = 0; i < block_data_util; i++) {
            block_buffer[i] = 0;
        }
        memcpy(block_buffer, buffer+(current_block*block_data_util), last_block_size);
        writeBlock(block_address, sectors_per_block, block_buffer);
        file.read_write_pointer += block_data_util;
    }

    files_opened[handle] = file;

	return SUCCESS_CODE;
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

    if (DEBUG) printf("\n\nBEGIN OF MKDIR 2 FOS %s\n", pathname);

    char *parent_name = malloc(sizeof(char));
    char *dir_name = malloc(sizeof(char));

    if (parent_name == NULL || dir_name == NULL) return MALLOC_ERROR_EXCEPTION;
    if (SUCCESS_CODE != getPathAndFileName(pathname, parent_name, dir_name)) return NOT_A_PATH_EXCEPTION;

    if (DEBUG) printf("validou o nome\n");

    Directory *parent_directory = malloc(sizeof(Directory));
    if (parent_directory == NULL) return MALLOC_ERROR_EXCEPTION;

    // se eh sem pai, insere no root

    if (strcmp("", parent_name) == 0) {

        if (DEBUG) printf("caiu no root como parent\n");
        //TODO funcao que retorne o root dir
        parent_directory = root_dir;

    } else {

        if (DEBUG) printf("nao caiu no root como parent\n");

        int get_dir_result = get_dir_from_path(parent_name, &parent_directory);
        if (get_dir_result != SUCCESS_CODE) return get_dir_result;

    }

    //TODO pegar do bitmap
    int next_valid_block = next_valid_blockk;

    Directory *new_directory = malloc(sizeof(Directory));
    int init_dir_result = initialize_directory(&new_directory, next_valid_block);

    Block *new_block = malloc(sizeof(Block));
    if (new_block == NULL) return MALLOC_ERROR_EXCEPTION;
    new_block->data = (unsigned char *) new_directory;
    new_block->address = (unsigned int) next_valid_block;
    new_block->next = 0;

    DIRENT2 entry;
    entry.fileType = 'd';
    entry.first_block = (unsigned int) next_valid_block;
    strcpy(entry.name, dir_name);

    printf("ateh aqui foi\n");

    int add_entry_result = addEntry(dir_name, &entry, &parent_directory->hash_table);
    if (add_entry_result != SUCCESS_CODE) return add_entry_result;

    printf("adicionou a entrada\n");

    printf("child dir number = %8u\n", new_directory->block_number);
    printf("new block add = %8u\n", new_block->address);

    int write_child_result = writeBlock((unsigned int) new_block->address, sectors_per_block, new_block);
    if (write_child_result != SUCCESS_CODE) return write_child_result;

    printf("escreveu filho\n");

    Block *parent_block = malloc(sizeof(Block));
    parent_block->data = (unsigned char *) parent_directory;
    parent_block->address = parent_directory->block_number;
    //TODO alguem me diga: o diretorio nao tem next certo???
    parent_block->next = 0;

    printf("parent block number = %8u\n", parent_directory->block_number);
    printf("parent block add = %8u\n", parent_block->address);
    int write_parent_result = writeBlock((unsigned int) parent_directory->block_number, sectors_per_block, parent_block);
    if (write_parent_result != SUCCESS_CODE) return write_parent_result;


    printf("newxt valid block %d\n", next_valid_blockk);
    Block *b = malloc(sizeof(Block));
    assert(b != NULL);
    assert(SUCCESS_CODE == read_block(&b, new_block->address, sectors_per_block));

    Directory *d = malloc(sizeof(Directory));
    assert(d != NULL);
    assert(b->data != NULL);
    assert(b->address == next_valid_blockk);
    d = (Directory *) b->data;
    assert(d != NULL);
    assert(d->block_number == next_valid_blockk);

    printf("escreveu pai\n");

    if (DEBUG) printf("END OF MKDIR 2\n\n");

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

    if (DEBUG) printf("\n\nBEGIN OF RMDIR 2 FOS %s\n", pathname);

    char *parent_name = malloc(sizeof(char));
    char *dir_name = malloc(sizeof(char));

    if (parent_name == NULL || dir_name == NULL) return MALLOC_ERROR_EXCEPTION;
    if (SUCCESS_CODE != getPathAndFileName(pathname, parent_name, dir_name)) return NOT_A_PATH_EXCEPTION;

    Directory *parent_dir = malloc(sizeof(Directory));
    int get_parent_result = get_dir_from_path(parent_name, &parent_dir);
    if (get_parent_result != SUCCESS_CODE) return get_parent_result;

    int removal_result = removeEntry(dir_name, &(parent_dir->hash_table));
    if (removal_result != SUCCESS_CODE) return  removal_result;

    Directory *dir = malloc(sizeof(Directory));
    int get_dir_result = get_dir_from_path(pathname, &dir);
    if (get_dir_result != SUCCESS_CODE) return get_dir_result;

    if (verifyIfDirIsOpened(dir->identifier)) {
        int close_result = close2(dir->identifier);
        if (close_result != SUCCESS_CODE) return ERROR_CODE;
    }

    //TODO atualizar o bitmap



	return SUCCESS_CODE;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para alterar o CP (current path)
 NÃO PRECISA
-----------------------------------------------------------------------------*/
int chdir2 (char *pathname) {

    //openDir()

	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para obter o caminho do diretório corrente.
 Essa funçao nao sera implementada pois fazemos caminho absoluto
 NÃO PRECISA
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
    char *parent_name = malloc(sizeof(char));
    char *dir_name = malloc(sizeof(char));
    int get_name_result = getPathAndFileName(pathname, parent_name, dir_name);
    if (get_name_result != SUCCESS_CODE) return get_name_result;

    Directory *parent_directory = malloc(sizeof(Directory));
    int get_dir_result = get_dir_from_path(pathname, &parent_directory);
    if (get_dir_result != SUCCESS_CODE) return get_dir_result;

    DIR2 handle = 0;
    while (handle < MAX_DIRECTORIES_NUMBER && opened_directories[handle].opened == 1 ) {
        handle ++;
    }
    if (SUCCESS_CODE != validate_file_handle(handle)) return MAX_OPENED_FILES_REACHED;

    (*parent_directory).opened = 1;
    opened_directories[handle] = *parent_directory;

    if (DEBUG) printf("END OF OPENDIR2\n");

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
N PRECISA
-----------------------------------------------------------------------------*/
int ln2 (char *linkname, char *filename) {
	return -1;
}


