//
// Created by gabriel on 25/05/19.
//

#include "../include/error.h"
#include "../include/data.h"
#include "../include/apidisk.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

unsigned int my_awesome_pow(unsigned int base, unsigned int exp) {
    unsigned int res = 1;
    int i;
    for (i=0; i<exp; i++) res *= base;
    return res;
}

void substring(char originString[], char finalSubstring[], int start, int last);

// TODO: verify the /0 and limits (need to test and debug this function)
int getPathAndFileName (char *filePath, char *path, char *name) {

    int i;

    if (filePath == NULL) return NULL_POINTER_EXCEPTION;
    int size = strlen(filePath);
    if (size <= 0) return EMPTY_LINE_EXCEPTION;
    if (filePath[0] != '/') return NOT_A_PATH_EXCEPTION;

    char* temp_path = (char*) malloc(sizeof(char));
    char* temp_name = (char*) malloc(sizeof(char));

    for (i = size-1; filePath[i] != '/'; i--);
    strncpy(temp_path, filePath, i);
    substring(filePath, temp_name, i + 2, size);

    if (strlen(temp_name) > 31)
        return INVALID_SIZE_FOR_FILE_NAME; //O T2FS deverá suportar arquivos com nomes formados por até 31 caracteres alfanuméricos (0‐9, a‐z e A‐Z). Os nomes são case‐sensitive.

    strcpy(path, temp_path);
    strcpy(name, temp_name);

    return SUCCESS_CODE;

}

int copyBlock(int first_sector, int sectors_per_block, Block *copied_block) {



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

int initialize_block(Block **block, int sectors_per_block) {

    *block = (Block *) malloc(sizeof(Block));

    (*block)->address = 0; //TODO: aqui a gente ja pode fazer um get pra achar o setor que pode ser o inicio do bloco, ou seja, tem que ter o númeor de setores disponível consecutivamente
    (*block)->next = 0;
    (*block)->data = malloc((SECTOR_SIZE * sectors_per_block) - (sizeof(unsigned int) * 2));

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
int writeBlock(unsigned int first_sector, int sectors_per_block, Block *block) {
    unsigned char *ptr = (unsigned char *) block;
    unsigned char *buffer = malloc(SECTOR_SIZE);
    int i;
    int nr_of_bytes_written_in_buffer = 0;
    int nr_of_current_sector = 0;
    int block_size_in_bytes = sizeof(char) * SECTOR_SIZE * sectors_per_block; //bytes per block
    const unsigned char *byte;

    if (DEBUG) printf("Size of block: %d \n ", block_size_in_bytes);

    for (byte = ptr; block_size_in_bytes--; ++byte) // I want to copy all bytes from block
    {
        buffer[nr_of_bytes_written_in_buffer] = *byte;
        nr_of_bytes_written_in_buffer++;

        if (nr_of_bytes_written_in_buffer >= SECTOR_SIZE) {
            if (write_sector(nr_of_current_sector + first_sector, buffer) != SUCCESS_CODE) return ERROR_CODE;
            if (DEBUG) print_buffer(buffer);
            nr_of_current_sector++;
            nr_of_bytes_written_in_buffer = 0;

            for (i = 0; i < SECTOR_SIZE; i++) {
                buffer[i] = 0;
            }

        }
    }

    free(buffer);
    return SUCCESS_CODE;
}

int read_block(Block **block, int initial_sector, int sectors_per_block) {

    unsigned char *great_buffer = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
    int i = 0, current_sector;

    for (current_sector = 0; current_sector < sectors_per_block; current_sector++) {
        unsigned char *sector_buffer = malloc(SECTOR_SIZE);
        if (sector_buffer == NULL) return MALLOC_ERROR_EXCEPTION;
        for (i = 0; i < SECTOR_SIZE; i++) sector_buffer[i] = 0;
        if (read_sector(initial_sector + current_sector, sector_buffer) != SUCCESS_CODE) return FAILED_TO_READ_SECTOR;
        if (memcpy(great_buffer + (SECTOR_SIZE * current_sector), sector_buffer, SECTOR_SIZE) == NULL)
            return NULL_POINTER_EXCEPTION;
    }

    *block = (Block *) great_buffer;
    return SUCCESS_CODE;
}

int assert_blocks_are_equal(Block *block1, Block *block2, int sectors_per_block) {
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

    unsigned char *super_block_buffer = malloc(SECTOR_SIZE);

    SuperBloco superBloco;
    if (read_sector(SUPER_BLOCK_SECTOR, super_block_buffer) != SUCCESS_CODE) return ERROR_CODE;
    if (bufferToSuperBlock(super_block_buffer, &superBloco) != SUCCESS_CODE) return ERROR_CODE;
    if (DEBUG) printf("Here the bitmap sector: %d\n", superBloco.bitmap_sector);
    if (read_sector(superBloco.bitmap_sector, bitmap) != SUCCESS_CODE) return ERROR_CODE;



    return SUCCESS_CODE;
}

int write_bitmap(unsigned char *bitmap){

    unsigned char *super_block_buffer = malloc(SECTOR_SIZE);

    SuperBloco superBloco;
    if (read_sector(SUPER_BLOCK_SECTOR, super_block_buffer) != SUCCESS_CODE) return ERROR_CODE;
    if (bufferToSuperBlock(super_block_buffer, &superBloco) != SUCCESS_CODE) return ERROR_CODE;
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
    if (read_sector(SUPER_BLOCK_SECTOR, super_block_buffer) != SUCCESS_CODE) return ERROR_CODE;
    bufferToSuperBlock(super_block_buffer, &superBloco);

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
int get_block_and_position_by_index(unsigned int index, int sectors_per_block, unsigned int *block_nr, unsigned int *block_data_pointer) {
    if (block_nr == NULL || block_data_pointer == NULL) return NULL_POINTER_EXCEPTION;
    if (sectors_per_block < 1 || index < 0) return ERROR_CODE;

    unsigned int block_data_util = SECTOR_SIZE * sectors_per_block - sizeof(unsigned int) * 2; // we miss 8 bytes for index and next (2*int)

    *block_data_pointer = (unsigned int) (index % block_data_util);
    *block_nr = (unsigned int) (index / block_data_util);

    return SUCCESS_CODE;
}


