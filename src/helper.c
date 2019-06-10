//
// Created by gabriel on 25/05/19.
//

#include "../include/error.h"
#include "../include/data.h"
#include "../include/apidisk.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

int bufferToSuperBlock(char *buffer, SuperBloco *superBloco) {
    if(buffer == NULL) return NULL_POINTER_EXCEPTION;
    if(superBloco == NULL) return NULL_POINTER_EXCEPTION;

    superBloco->rootDirBegin = (unsigned int) 0;
    superBloco->rootDirEnd = (unsigned int) 0;
    superBloco->generalBlocksBegin = (unsigned int) 0;
    superBloco->numberOfBlocks = (unsigned int) 0;
    superBloco->bitmap_sector = (unsigned int) 0;
    superBloco->bitmap_size = (unsigned int) 0;

    sscanf(buffer, "%u#%u#%u#%u#%u#%u",
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

// add this
int initialize_block(Block **block, int sectors_per_block) {

    *block = (Block *) malloc(sizeof(Block));

    (*block)->address = 0; //TODO: aqui a gente ja pode fazer um get pra achar o setor que pode ser o inicio do bloco, ou seja, tem que ter o númeor de setores disponível consecutivamente
    (*block)->next = 0;
    (*block)->data = malloc((SECTOR_SIZE * sectors_per_block) - (sizeof(unsigned int) * 2));

    return SUCCESS_CODE;
}

void print_buffer(unsigned char *buffer) {
    puts("Print buffer");
    int size = SECTOR_SIZE;
    const unsigned char *byte;
    for (byte = buffer; size--; ++byte) {
        printf("%02X ", *byte);
    }
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

int readBlock(unsigned int first_sector, int sectors_per_block, Block *block) {
    return ERROR_CODE;
}

//int buffer_to_block(unsigned char* buffer, Block **block) {
//    &block = (Block *) buffer;
//}



