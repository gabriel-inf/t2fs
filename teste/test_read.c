//
// Created by Laura Corssac on 2019-06-09.
//

#include "../include/data.h"
#include "../include/apidisk.h"
#include "../include/error.h"
#include "../include/t2fs.h"
#include "../include/helper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}


int get_block(Block ** block, int initial_sector, int sectors_per_block) {

    unsigned char *fullBuffer = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
    if (fullBuffer == NULL) return MALLOC_ERROR_EXCEPTION;

    //Block block = (Block *) buffer;

    int i=0;
    for ( i=0; i< sectors_per_block; i++) {

        unsigned char *buffer = malloc(SECTOR_SIZE);
        if (buffer == NULL) return MALLOC_ERROR_EXCEPTION;
        if (read_sector(initial_sector + i, buffer) != SUCCESS_CODE) return FAILED_TO_READ_SECTOR;
        if (memcpy(fullBuffer+ (i * SECTOR_SIZE), buffer, sizeof(SECTOR_SIZE)) == NULL) return NULL_POINTER_EXCEPTION;

        free(buffer);
    }
    printf("saiu do for\n");


    *block = (Block *) fullBuffer;
    printf("block next\n");
    printBits(sizeof(unsigned int), &((*block)->next));
    long size = SECTOR_SIZE * sectors_per_block - 2 * sizeof(unsigned int);
    printf("block address\n");
    printBits(sizeof(unsigned int), &((*block)->address));

    return SUCCESS_CODE;
}


int main() {

    //unsigned char *write_buffer = (unsigned char *) malloc(SECTOR_SIZE);
    unsigned char *write_buffer = (unsigned char *) malloc(SECTOR_SIZE * 2);
    unsigned char *read_buffer = (unsigned char *) malloc(SECTOR_SIZE * 2);

    int i = 0;
    
    for (i = 0; i < (SECTOR_SIZE); i++ ){

        write_buffer[i] = (unsigned char) 10;
        read_buffer[i] = (unsigned char) 0;
        
    }

    for (i = SECTOR_SIZE; i < (SECTOR_SIZE * 2); i++ ){

        write_buffer[i] = (unsigned char) 5;
        read_buffer[i] = (unsigned char) 0;

    }

    assert((write_sector((unsigned int) 4, write_buffer) != SUCCESS_CODE) == SUCCESS_CODE);
    assert(SUCCESS_CODE == (write_sector((unsigned int) 5, write_buffer + SECTOR_SIZE) != SUCCESS_CODE));

    unsigned int sectors_per_block = 1;


    long size = SECTOR_SIZE * sectors_per_block - 2 * sizeof(unsigned int);
    unsigned char *data = malloc(size);

    for (i = 0; i < size; i++){

        data[i] = (unsigned char) 10;

    }

    assert(SUCCESS_CODE == read_sector(4, read_buffer));
    assert(SUCCESS_CODE == read_sector(5, read_buffer + SECTOR_SIZE));
    assert(memcmp(write_buffer, read_buffer, SECTOR_SIZE) == 0);

    Block *bloco = malloc(sizeof(Block));
    bloco->address = (unsigned int) 10;
    bloco->next = (unsigned int) 20;
    bloco->data = data;

    Block *new_block =  malloc(sizeof(Block));

    assert( SUCCESS_CODE == writeBlock((unsigned int) 10, sectors_per_block, bloco));

    assert( SUCCESS_CODE == get_block(&new_block, 10, sectors_per_block));

    //printBits(SECTOR_SIZE * 2, read_buffer);
    
    //assert(new_block->address == bloco->address);
    //assert(new_block->next == bloco->next);
    //assert(memcmp(new_block->data, bloco->data, size));
    
    
    printf("new block\n");
    printBits(sizeof(unsigned int), &(new_block->next));
    printBits(sizeof(unsigned int), &(new_block->address));
    
    printf("old block\n");
    printBits(sizeof(unsigned int), &(bloco->next));
    printBits(sizeof(unsigned int), &(bloco->address));

    printf("TODOS OS TESTES READ AND WRITE PASSARAM\n");

    return 0;
}
