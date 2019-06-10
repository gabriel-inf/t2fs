//
// Created by gabriel on 10/06/19.
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


void printBits(size_t const size, void const *const ptr) {
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


int get_block(Block *block, int initial_sector) {

    int sectors_per_block = 2;

    unsigned char *fullBuffer = malloc(sizeof(SECTOR_SIZE * sectors_per_block));
    if (fullBuffer == NULL) return MALLOC_ERROR_EXCEPTION;

    //Block block = (Block *) buffer;

    int i = 0;
    for (i = 0; i < sectors_per_block; i++) {

        printf("entrou no for\n");
        unsigned char *buffer = malloc(SECTOR_SIZE);
        if (buffer == NULL) return MALLOC_ERROR_EXCEPTION;
        printf("fez malloc\n");
        if (read_sector(initial_sector + i, buffer) != SUCCESS_CODE) return FAILED_TO_READ_SECTOR;
        printf("fez read sector\n");
        if (memcpy(fullBuffer + (i * SECTOR_SIZE), buffer, sizeof(SECTOR_SIZE)) == NULL) return NULL_POINTER_EXCEPTION;

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
    int sectors_per_block = 4;
    unsigned char *write_buffer = (unsigned char *) malloc(SECTOR_SIZE * sectors_per_block);
//    unsigned char read_buffer[SECTOR_SIZE];



    int i = 0;

    printf("aaaaaaaaaaaaa\n");

    for (i = 0; i < SECTOR_SIZE * sectors_per_block; i++) {

        write_buffer[i] = (unsigned char) 0;

    }
    Block *block1;
    initialize_block(&block1, sectors_per_block);
    block1->address = 120;
    block1->next = 23;

    for (i = 0; i < 1000; i++) {
        block1->data[i] = '1';
    }
    printf("%d %d\n", block1->address, block1->next);
    writeBlock(30, sectors_per_block, block1);


    return 0;
}