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

int main() {

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

    unsigned int sectors_per_block_global = 1;


    long size = (SECTOR_SIZE * sectors_per_block_global); // - (2 * sizeof(unsigned int));
    unsigned char *data = malloc(size);

    for (i = 0; i < size; i++){

        data[i] = (unsigned char) 10;

    }

    assert(SUCCESS_CODE == read_sector(4, read_buffer));
    assert(SUCCESS_CODE == read_sector(5, read_buffer + SECTOR_SIZE));
    assert(memcmp(write_buffer, read_buffer, SECTOR_SIZE) == 0);

    Block *bloco = (Block *) data; //malloc(sizeof(Block));
    //bloco->address = (unsigned int) 1;
    //bloco->next = (unsigned int) 20;
    //bloco->data = data;

    Block *new_block =  malloc(sizeof(Block));

    printf("old block before\n");
    printf("\n");
    printBits(sizeof(unsigned int), &(bloco->next));
    printBits(sizeof(unsigned int), &(bloco->address));
    printBits(size, &(bloco->data));

    assert( SUCCESS_CODE == writeBlock((unsigned int) 10, sectors_per_block_global, bloco));

    assert( SUCCESS_CODE == read_block(&new_block, 10, sectors_per_block_global));

    //printBits(SECTOR_SIZE * 2, read_buffer);

    //assert(new_block->address == bloco->address);
    //assert(new_block->next == bloco->next);
    //assert(memcmp(new_block->data, bloco->data, size));


    printf("new block\n");
    printf("\n");
    printBits(sizeof(unsigned int), &(new_block->next));
    printBits(sizeof(unsigned int), &(new_block->address));
    printBits(size, &(new_block->data));

    printf("old block after\n");
    printf("\n");
    printBits(sizeof(unsigned int), &(bloco->next));
    printBits(sizeof(unsigned int), &(bloco->address));
    printBits(size, &(bloco->data));

    printf("TODOS OS TESTES READ AND WRITE PASSARAM\n");

    return 0;
}
