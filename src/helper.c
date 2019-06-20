//
// Created by gabriel on 25/05/19.
//

#include "../include/error.h"
#include "../include/data.h"
#include "../include/apidisk.h"
#include "../include/hashtable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

unsigned int my_awesome_pow(unsigned int base, unsigned int exp) {
    unsigned int res = 1;
    int i;
    for (i=0; i<exp; i++) res *= base;
    return res;
}

int verifyIfDirIsOpened(DIR2 dir_id) {

    int index = 0;

    while (index < MAX_DIRECTORIES_NUMBER) {
        if (opened_directories[index].identifier == dir_id && opened_directories[index].opened == 1) {
            return 1;
        }
        index ++;
    }
    return 0;
}

int initialize_directory(Directory* directory, unsigned int next_valid_block) {

    Directory new_dir; // = malloc(SECTOR_SIZE * sectors_per_block);
    //if (new_dir == NULL) return NULL_POINTER_EXCEPTION;


    //int hash_init_result = initialize_hashTable( &(new_dir->hash_table) );
    //if (hash_init_result != SUCCESS_CODE) return hash_init_result;

    new_dir.hash_table = malloc(sizeof(DataItem ) * SIZE);
    int i = 0, j=0;
    for (i=0; i < SIZE; i++) {
        new_dir.hash_table[i].valid = 0;
        for (j =0; j < MAX_FILE_NAME_SIZE; j++) {
            new_dir.hash_table[i].key[j] = 'a';
        }
        //strncpy(directory->hash_table[i].key, "", MAX_FILE_NAME_SIZE);
    }

    new_dir.opened = 0;
    new_dir.current_entry_index = 0;
    new_dir.identifier = 0;
    new_dir.block_number = next_valid_block;
    memcpy(directory, &new_dir, (SECTOR_SIZE * sectors_per_block));
    //free(new_dir);

    return SUCCESS_CODE;

}



int get_dir_from_path(char *pathname, Directory *directory) {

    if (DEBUG) printf("BEGIN OF GET DIR FROM PATH\n\n");
    puts(pathname);
    if (pathname == NULL) return NULL_POINTER_EXCEPTION;

    const char slash[2] = "/";
    char path_copy[MAX_FILE_NAME_SIZE];
    strcpy(path_copy, pathname);

    // tokenize the path of directories

    char *direct_child_pathname;
    direct_child_pathname = strtok(path_copy, slash);
    if ( direct_child_pathname == NULL ) return NOT_A_PATH_EXCEPTION;

    // reads from disk first parent, the root director

    Directory *parent_directory = malloc(SECTOR_SIZE * sectors_per_block);
    initialize_directory(parent_directory, 0);

    int root_result = get_root_directory(parent_directory);
    assert(parent_directory->hash_table[0].key != NULL);
    printf("begin of hash print\n");
    int sos =0;
    for (sos =0; sos < SIZE; sos ++) {
        puts(parent_directory->hash_table[sos].key);
    }
    printf("\n\n");
    if (root_result != SUCCESS_CODE) return root_result;

    //parent_directory = root_dir;

    while (direct_child_pathname != NULL) {

        printf("child name = \n");
        if (DEBUG) puts(direct_child_pathname);

        DIRENT2 *entry = malloc(sizeof(DIRENT2));
        if (entry == NULL) return MALLOC_ERROR_EXCEPTION;

        // check if subdir is on parent's hash

        int result = getValue(direct_child_pathname, &entry, parent_directory->hash_table);
        if (result != SUCCESS_CODE) return result;

        if (DEBUG) printf("deu get value\n");

        if (entry->fileType != 'd') return FILE_NOT_FOUND;

        Block *block = malloc(SECTOR_SIZE * sectors_per_block);
        if (block == NULL) return MALLOC_ERROR_EXCEPTION;

        // get the logical block from the child directory

        DWORD entry_first_block = entry->first_block;
        if (DEBUG) printf("entry first block = %d\n", entry_first_block);
        int get_dir_result = read_block(block, entry_first_block);
        if (DEBUG) printf("entry first block = %d\n", entry_first_block);
        if (get_dir_result != SUCCESS_CODE) return get_dir_result;

        if (DEBUG) printf("deu read block\n");

        Directory *new_dir = malloc(SECTOR_SIZE * sectors_per_block);
        if (new_dir == NULL) return NULL_POINTER_EXCEPTION;

        if (DEBUG) printf("deu MALLOC DIR\n");

        initialize_directory(new_dir, 0);

        assert(block->data != NULL);

        new_dir = (Directory *) block->data;
        if (new_dir == NULL) return NULL_POINTER_EXCEPTION;

        //if (DEBUG) printf("new dir id = %d\n", new_dir-);
        if (DEBUG) printf("new dir block = %d\n", new_dir->block_number);

        if (DEBUG) printf("deu new dir\n");

        if (NULL == memcpy(parent_directory, new_dir, SECTOR_SIZE * sectors_per_block)) return NULL_POINTER_EXCEPTION;

        if (DEBUG) printf("copiou certinho\n");

        free(new_dir);
        free(entry);

        direct_child_pathname = strtok(NULL, slash);

    }

    memcpy(directory, parent_directory, SECTOR_SIZE * sectors_per_block);

    printf("\n%d\n", directory->block_number);
    printf("%d\n", directory->identifier);
    printf("%d\n\n", directory->opened);

    //free(*parent_directory);

    if (DEBUG) printf("END GET DIR FROM PATH\n\n");

    return SUCCESS_CODE;

}

/**
 *
 * @param handle o index de um diretório aberto
 * @return 0 se é um index válido. != 0 caso contrario
 */

int validate_dir_handle(unsigned int handle) {

    if ((handle >= 0) && (handle < MAX_DIRECTORIES_NUMBER)) {
        return SUCCESS_CODE;
    } else {
        return ERROR_CODE;
    }

}

/**
 *
 * @param handle o index de um arquivo aberto
 * @return 0 se é um index entre 0-9. != 0 caso contrário
 */

int validate_file_handle(unsigned int handle) {

    if (handle >= 0 && handle < MAX_FILES_OPENED) {
        return SUCCESS_CODE;
    } else {
        return ERROR_CODE;
    }

}

void substring(char originString[], char finalSubstring[], int start, int last);

// TODO: verify the /0 and limits (need to test and debug this function)
int getPathAndFileName (char *filePath, char *path, char *name) {


    printf("BEGIN OF GETPATH AND FILE NAME\n");
    int i;

    if (filePath == NULL) return NULL_POINTER_EXCEPTION;
    int size = strlen(filePath);
    if (size <= 0) return EMPTY_LINE_EXCEPTION;
    if (filePath[0] != '/') return NOT_A_PATH_EXCEPTION;

    char* temp_path = (char*) malloc(MAX_FILE_NAME_SIZE);
    char* temp_name = (char*) malloc(MAX_FILE_NAME_SIZE);

    for (i = size-1; filePath[i] != '/'; i--);
    strncpy(temp_path, filePath, i);
    substring(filePath, temp_name, i + 2, size);

    if (strlen(temp_name) > 31)
        return INVALID_SIZE_FOR_FILE_NAME; //O T2FS deverá suportar arquivos com nomes formados por até 31 caracteres alfanuméricos (0‐9, a‐z e A‐Z). Os nomes são case‐sensitive.

    strcpy(path, temp_path);
    strcpy(name, temp_name);


    printf("END OF GETPATH AND FILE NAME\n");
    return SUCCESS_CODE;

}

int copyBlock(int first_sector, Block *copied_block) {



    return ERROR_CODE;
}



//C substring function definition
void substring(char originString[], char finalSubstring[], int start, int last) {
    int c = 0;

    while (c < last) {
        finalSubstring[c] = originString[start + c - 1];
        c++;
    }
    finalSubstring[c] = '\0';
}

int superBlockToBuffer(SuperBloco *superBloco, unsigned char *buffer) {

    if(buffer == NULL) return NULL_POINTER_EXCEPTION;
    if(superBloco == NULL) return NULL_POINTER_EXCEPTION;

    // copy the struct information to the serialization buffer
    snprintf((char *) buffer, SECTOR_SIZE, "%u#%u#%u#%u#%u#%u",
             (unsigned int) superBloco->rootDirBegin,
             (unsigned int) superBloco->rootDirEnd,
             (unsigned int) superBloco->generalBlocksBegin,
             (unsigned int) superBloco->numberOfBlocks,
             (unsigned int) superBloco->bitmap_sector,
             (unsigned int) superBloco->bitmap_size
    );

    return SUCCESS_CODE;

}

int bufferToSuperBlock(unsigned char *buffer, SuperBloco *superBloco) {
    if(buffer == NULL) return NULL_POINTER_EXCEPTION;
    if(superBloco == NULL) return NULL_POINTER_EXCEPTION;

    superBloco->rootDirBegin = (unsigned int) 0;
    superBloco->rootDirEnd = (unsigned int) 0;
    superBloco->generalBlocksBegin = (unsigned int) 0;
    superBloco->numberOfBlocks = (unsigned int) 0;
    superBloco->bitmap_sector = (unsigned int) 0;
    superBloco->bitmap_size = (unsigned int) 0;

    sscanf((char *)buffer, "%u#%u#%u#%u#%u#%u",
           &superBloco->rootDirBegin,
           &superBloco->rootDirEnd,
           &superBloco->generalBlocksBegin,
           &superBloco->numberOfBlocks,
           &superBloco->bitmap_sector,
           &superBloco->bitmap_size);

    return SUCCESS_CODE;

}

void printSuperblock(SuperBloco *superBloco) {
    printf("\nSuperBloco info:\n\trootDirBegin: %u \n\trootDirEnd: %u\n\tgeneralBlocksBegin: %u\n\tnumberOfBlocks: %u\n\tbitmap_sector: %u\n\tbitmap_size: %u bytes\n\n",
           (unsigned int) superBloco->rootDirBegin,
           (unsigned int) superBloco->rootDirEnd,
           (unsigned int) superBloco->generalBlocksBegin,
           (unsigned int) superBloco->numberOfBlocks,
           (unsigned int) superBloco->bitmap_sector,
           (unsigned int) superBloco->bitmap_size
    );
}

int get_superblock(SuperBloco *superBloco) {

    if (DEBUG) printf("BEGIN GET SUPERBLOCK\n");

    SuperBloco localSuperBlock;
    unsigned char *super_block_buffer = malloc(SECTOR_SIZE);
    if (read_sector(SUPER_BLOCK_SECTOR, super_block_buffer) != SUCCESS_CODE) return ERROR_CODE;
    if (bufferToSuperBlock(super_block_buffer, &localSuperBlock) != SUCCESS_CODE) return ERROR_CODE;
    *superBloco = localSuperBlock;

    if (DEBUG) printf("END GET SUPERBLOCK\n");
    return SUCCESS_CODE;

}

void print_buffer(unsigned char *buffer) {
    puts("\nPrint buffer");
    int size = SECTOR_SIZE;
    const unsigned char *byte;
    for (byte = buffer; size--; ++byte) {
        printf("%02u ", *byte);
    }
}

void printBits(size_t const size, void const const* ptr) {
    unsigned char *b = (unsigned char *) ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}


/**
 * Take the sector, serialize it someway and persist
 *
 */
int writeBlock(unsigned int block_index, Block *block) {

    if (DEBUG) printf("\nENTROU NA WRITE BLOCK COM %d\n", block_index);

    unsigned int first_sector; // = malloc(sizeof(int));
    unsigned char *ptr = (unsigned char *) block;
    unsigned char *buffer = malloc(SECTOR_SIZE);
    int i;
    int nr_of_bytes_written_in_buffer = 0;
    int nr_of_current_sector = 0;
    int block_size_in_bytes = sizeof(char) * SECTOR_SIZE * sectors_per_block; //bytes per block
    const unsigned char *byte;

    if (get_block_first_sector(block_index, &first_sector) != SUCCESS_CODE) return ERROR_CODE;
    printf("first sector = %8u\n", first_sector);

    printf("Size of block: %d \n ", block_size_in_bytes);

    for (byte = ptr; block_size_in_bytes--; ++byte) // I want to copy all bytes from block
    {
        buffer[nr_of_bytes_written_in_buffer] = *byte;
        nr_of_bytes_written_in_buffer++;

        if (nr_of_bytes_written_in_buffer >= SECTOR_SIZE) {
            if (write_sector(nr_of_current_sector + first_sector, buffer) != SUCCESS_CODE) return ERROR_CODE;
            //if (DEBUG) print_buffer(buffer);
            nr_of_current_sector++;
            nr_of_bytes_written_in_buffer = 0;

            for (i = 0; i < SECTOR_SIZE; i++) {
                buffer[i] = 0;
            }

        }
    }

    free(buffer);
    if (DEBUG) printf("\nSAIU DA WRITE BLOCK\n");
    return SUCCESS_CODE;
}

int read_block(Block *block, unsigned int block_index) {

    if (DEBUG) printf("BEGIN READ BLOCK COM INDEX = %d\n", block_index);
    unsigned int initial_sector; // = malloc(sizeof(int));

    if (get_block_first_sector(block_index, &initial_sector) != SUCCESS_CODE) return ERROR_CODE;
    printf("initial sector = %8u\n", initial_sector);

    unsigned char *great_buffer = malloc(SECTOR_SIZE * sectors_per_block);
    int i = 0, current_sector;

    for (current_sector = 0; current_sector < sectors_per_block; current_sector++) {

        unsigned char *sector_buffer = malloc(SECTOR_SIZE);
        if (sector_buffer == NULL) return MALLOC_ERROR_EXCEPTION;
        for (i = 0; i < SECTOR_SIZE; i++) sector_buffer[i] = 0;
        if (read_sector(initial_sector + current_sector, sector_buffer) != SUCCESS_CODE) return FAILED_TO_READ_SECTOR;
        if (memcpy(great_buffer + (SECTOR_SIZE * current_sector), sector_buffer, SECTOR_SIZE) == NULL)
            return NULL_POINTER_EXCEPTION;
    }

    Block *aux = (Block *) great_buffer;

    //assert(great_buffer != NULL);
    assert(aux != NULL);
    assert(aux->data != NULL);
    memcpy(block, aux, SECTOR_SIZE * sectors_per_block);
    if (DEBUG) printf("SAINDO DA READ BLOCK\n");
    //free(aux);
    //free(great_buffer);
    return SUCCESS_CODE;
}

int assert_blocks_are_equal(Block *block1, Block *block2) {
    int nr_of_bytes = sectors_per_block * SECTOR_SIZE;
    unsigned char *ptr1 = (unsigned char *) block1;
    unsigned char *ptr2 = (unsigned char *) block2;
    int i;
    for (i = 0; i < nr_of_bytes; i++) {
        if (ptr1[i] != ptr2[i]) {
            printf("Falhou no byte %d\n", i);
            printf("No ptr1 temos: %2u; já no ptr2 temos: %2u\n", ptr1[i], ptr2[i]);
            return 0;
        }
    }
    return 1;
}

int init_bitmap(unsigned char *bitMap, unsigned int bitMapSize) {
    int i = 0;
    for(i = 0; i < SECTOR_SIZE; i++){
        *(bitMap + (i * sizeof(char))) = 0;
    }
    return SUCCESS_CODE;
}

//int buffer_to_block(unsigned char* buffer, Block **block) {
//    &block = (Block *) buffer;
//}

int read_bitmap(unsigned char *bitmap){

    SuperBloco superBloco;
    if (get_superblock(&superBloco) != SUCCESS_CODE) return ERROR_CODE;
    if (DEBUG) printf("Here the bitmap sector: %d\n", superBloco.bitmap_sector);
    if (read_sector(superBloco.bitmap_sector, bitmap) != SUCCESS_CODE) return ERROR_CODE;

    return SUCCESS_CODE;
}

int write_bitmap(unsigned char *bitmap){

    SuperBloco superBloco;
    if (get_superblock(&superBloco) != SUCCESS_CODE) return ERROR_CODE;
    if (write_sector(superBloco.bitmap_sector, bitmap) != SUCCESS_CODE) return ERROR_CODE;

    return SUCCESS_CODE;
}


int is_block_free(unsigned int block_address){

    unsigned int locationByte;
    unsigned int offset;
    unsigned char *bitmap = malloc(SECTOR_SIZE);

    if (read_bitmap(bitmap) != SUCCESS_CODE) return  ERROR_CODE; // LÊ O BITMAP

    locationByte = block_address/8; //vai retornar o byte no qual o bloco se encontra
    offset = block_address%8; //vai retornar o bit dentro do byte onde o bloco está

    BYTE byte_of_interest = *(bitmap+locationByte);
    BYTE byte_mask = my_awesome_pow(2, offset);

    if((byte_of_interest & byte_mask) != 0){
        return 0; //bloco está ocupado (1xxx xxxx & 1000 0000 = 1000 0000 (128))
    }else {
        return 1; //bloco está livre (0xxx xxxx & 1000 0000 = 0000 0000 (0))
    }

}

int set_block_as_occupied(unsigned int block_address){

    unsigned char* bitmap = malloc(SECTOR_SIZE);
    unsigned int bitmapSize;

    if (read_bitmap(bitmap) != SUCCESS_CODE) return  ERROR_CODE; // LÊ O BITMAP

    if(!is_block_free(block_address)) return ERROR_CODE;

    unsigned int locationByte;
    unsigned int offset;

    locationByte = block_address/8; //vai retornar o byte no qual o bloco se encontra
    offset = (unsigned int) block_address%8; //vai retornar o bit dentro do byte onde o bloco está

    BYTE* byte_of_interest = (bitmap+locationByte);
    BYTE byte_mask = (unsigned int) my_awesome_pow((unsigned int) 2, offset);

    *byte_of_interest = *byte_of_interest | byte_mask;

    if (write_bitmap(bitmap) != SUCCESS_CODE) return ERROR_CODE; // ESCREVE O BITMAP (parece n estar gravando o bitmap alterado

    if(is_block_free(block_address)){
        return ERROR_CODE; //bloco não foi ocupado, função executada com erro
    }else {

        return SUCCESS_CODE; //bloco ocupado com sucesso
    }


}


int free_block(unsigned int block_address){

    unsigned char* bitmap = malloc(sizeof(SECTOR_SIZE));
    unsigned int bitmapSize;
    if (read_bitmap(bitmap) != SUCCESS_CODE) return ERROR_CODE;

    unsigned int locationByte;
    unsigned int offset;

    locationByte = block_address/8; //vai retornar o byte no qual o bloco se encontra
    offset = (unsigned int)block_address%8; //vai retornar o bit dentro do byte onde o bloco está

    BYTE* byte_of_interest = (bitmap+locationByte);

    BYTE tester = my_awesome_pow((unsigned int)2, offset);

    tester = 255 - tester; // 1111 1111 com um zero na posição do bit

    *byte_of_interest = *byte_of_interest & tester; //exemplo de bit na posição 2: 1101 1111 & 1010 1010 = 1000 1010

    write_bitmap(bitmap);

    if(!is_block_free(block_address)){
        return ERROR_CODE; //bloco não foi liberado, função executada com erro
    }else {
        return SUCCESS_CODE; //bloco liberado com sucesso
    }

}

/**
 * Searches for free blocks
 * @return an unsigned int that corresponds to the first free block found
 */
unsigned int get_free_block(){

    unsigned char* bitmap = malloc(SECTOR_SIZE);
    unsigned int bitmapSize;
    unsigned char *super_block_buffer = malloc(SECTOR_SIZE);
    unsigned int block;
    SuperBloco superBloco;

    if (read_bitmap(bitmap) != SUCCESS_CODE) return ERROR_CODE;
    if (get_superblock(&superBloco) != SUCCESS_CODE) return ERROR_CODE;

    for(block = 0; block < superBloco.bitmap_size; block++){
        if (is_block_free(block)) {
            return block;
        }
    }

    return FULL_BLOCKS;
}


/*
 * Returns by parameters the block index and data pointer for a file current index
 * returns
 */
int get_block_and_position_by_index(unsigned int index, unsigned int *block_nr, unsigned int *block_data_pointer) {
    if (block_nr == NULL || block_data_pointer == NULL) return NULL_POINTER_EXCEPTION;
    if (sectors_per_block < 1 || index < 0) return ERROR_CODE;

    unsigned int block_data_util = SECTOR_SIZE * sectors_per_block - sizeof(unsigned int) * 2; // we miss 8 bytes for index and next (2*int)

    *block_data_pointer = (unsigned int) (index % block_data_util);
    *block_nr = (unsigned int) (index / block_data_util);

    return SUCCESS_CODE;
}

/*
 * Returns by parameters the first sector of a block. It may be used inside write and read blocks
 * if sectors_per_block = 4, block_index = 0 than first_sector = 0
 * if sectors_per_block = 4, block_index = 1 than first_sector = 4
 * if sectors_per_block = 4, block_index = 2 than first_sector = 8
 * and so on.. we have a simple multiplication
 */
int get_block_first_sector(unsigned int block_index, unsigned int *first_sector) {
    if (first_sector == NULL) return NULL_POINTER_EXCEPTION;
    if (sectors_per_block < 1 || index < 0) return ERROR_CODE;

    SuperBloco superBloco;

    if (get_superblock(&superBloco) != SUCCESS_CODE) return ERROR_CODE;
    unsigned int sector_offset = superBloco.generalBlocksBegin;

    *first_sector = (unsigned int) block_index * sectors_per_block + sector_offset;

    return SUCCESS_CODE;
}

int initialize_block(Block **block) {

    SuperBloco superBloco;
    if (get_superblock(&superBloco) != SUCCESS_CODE) return ERROR_CODE;

    unsigned int sector_offset = superBloco.generalBlocksBegin;
    unsigned int block_index = get_free_block();
    set_block_as_occupied(block_index);

    *block = (Block *) malloc(sizeof(Block));
    (*block)->address = block_index;
    (*block)->next = 0;
    (*block)->data = malloc((SECTOR_SIZE * sectors_per_block) - (sizeof(unsigned int) * 2));

    return SUCCESS_CODE;
}

int get_root_directory(Directory *root_directory) {

    if (DEBUG) printf("BEGIN OF GET ROOT DIR\n\n");
    assert(root_directory != NULL);

    SuperBloco *super_bloco = malloc(SECTOR_SIZE);
    int result = get_superblock(super_bloco);
    if (result != SUCCESS_CODE) return result;

    Block *root_dir_block = malloc(SECTOR_SIZE * sectors_per_block);
    if (root_dir_block == NULL) return MALLOC_ERROR_EXCEPTION;

    int read_result = read_block(root_dir_block, super_bloco->rootDirBegin);
    if (read_result != SUCCESS_CODE) return read_result;

    printBits(SECTOR_SIZE * sectors_per_block, root_dir_block->data);

    printf("root dir begin %d\n", super_bloco->rootDirBegin);

    Directory *local_dir = malloc(SECTOR_SIZE * sectors_per_block);
    if (local_dir == NULL) return MALLOC_ERROR_EXCEPTION;

    //initialize_directory(local_dir, super_bloco->rootDirBegin);
    //if (local_dir == NULL ) return MALLOC_ERROR_EXCEPTION;

    local_dir = (Directory *) root_dir_block->data;
    assert(local_dir->hash_table[0].key != NULL);

    int sos = 0;
    printf("\nbegin of hash print no root\n");
    for (sos =0; sos < SIZE; sos ++) {
        puts(local_dir->hash_table[sos].key);
    }

    printf("deu mem cpy 1\n");

    //root_directory = malloc(sizeof(SECTOR_SIZE * sectors_per_block));

    assert(root_directory != NULL);
    memcpy(root_directory, local_dir, SECTOR_SIZE * sectors_per_block);

    printf("deu mem cpy 2\n");

//    free(root_dir_block);
//    free(super_bloco);
    if (DEBUG) printf("END OF GET ROOT DIR\n\n");

    return SUCCESS_CODE;
}

int write_dir(Directory *directory) {

    printf("WRITE DIR\n");

    Block *root_dir_block = malloc(SECTOR_SIZE * sectors_per_block);

    printf("sextor per block = %d\n", sectors_per_block);
    printf("Mlaloc foi\n");
    root_dir_block->data = (unsigned char *) directory;
    root_dir_block->address = directory->block_number;
    root_dir_block->next = UINT_MAX;

    printf("END WRITE DIR\n");

    return writeBlock(root_dir_block->address, root_dir_block);

}