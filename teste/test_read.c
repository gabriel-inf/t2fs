//
// Created by Laura Corssac on 2019-06-09.
//

#include "../include/data.h"
#include "../include/apidisk.h"
#include "../include/error.h"
#include "../include/t2fs.h"
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


int get_block(Block * block, int initial_sector) {

    int sectors_per_block = 2;

    unsigned char *fullBuffer = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
    if (fullBuffer == NULL) return MALLOC_ERROR_EXCEPTION;

    //Block block = (Block *) buffer;

    int i=0;
    for ( i=0; i< sectors_per_block; i++) {

        printf("entrou no for\n");
        unsigned char *buffer = malloc(SECTOR_SIZE);
        if (buffer == NULL) return MALLOC_ERROR_EXCEPTION;
        printf("fez malloc\n");
        if (read_sector(initial_sector + i, buffer) != SUCCESS_CODE) return FAILED_TO_READ_SECTOR;
        printf("fez read sector\n");
        if (memcpy(fullBuffer+ (i * SECTOR_SIZE), buffer, sizeof(SECTOR_SIZE)) == NULL) return NULL_POINTER_EXCEPTION;

        printf("fez mem cpy\n");
        free(buffer);
        printf("fez fren\n");
    }
    printf("saiu do for\n");


    block = (Block *) fullBuffer;
    printf("block next %d\n", block->next);
    puts(block->data);
    printf("block address %d\n", block->address);

    return SUCCESS_CODE;
}


int main() {

    //unsigned char *write_buffer = (unsigned char *) malloc(SECTOR_SIZE);
    unsigned char *write_buffer = (unsigned char *) malloc(SECTOR_SIZE);
    unsigned char read_buffer[SECTOR_SIZE];
    
    printf("SECTOR SIZE %d\n", SECTOR_SIZE);

    int i = 0;
    
    printf("aaaaaaaaaaaaa\n");
    
    for (i = 0; i < SECTOR_SIZE; i++ ){

        write_buffer[i] = (unsigned char) 10;
        read_buffer[i] = (unsigned char) 0;
        
    }
    
    unsigned int sector_number = 0;
    
    //assert( SUCCESS_CODE == write_sector(sector_number, write_buffer));
    if(read_sector((unsigned int) 0, write_buffer) != SUCCESS_CODE) {
    
    	printf("deu ruim\n");
    
    }
    printf("read sector result = %d\n", read_sector(sector_number, read_buffer));

    printBits(SECTOR_SIZE, read_buffer);

    assert(memcmp(write_buffer, read_buffer, SECTOR_SIZE) == 0);

    return 0;
}
