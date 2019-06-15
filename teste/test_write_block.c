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

int main() {

    int sectors_per_block = 4;
    unsigned int first_sector = 30;
    unsigned char *write_buffer = (unsigned char *) malloc(SECTOR_SIZE * sectors_per_block);

    int i = 0;

    printf("Size of unsigned int: %d\n", sizeof(unsigned int));

    for (i = 0; i < SECTOR_SIZE * sectors_per_block; i++) {

        write_buffer[i] = (unsigned char) 0;

    }
    Block *block1;
    Block *block2;

    initialize_block(&block1, sectors_per_block);
    block1->address = 120;
    block1->next = 25;
    
    initialize_block(&block2, sectors_per_block);
    block2->address = 120;
    block2->next = 100;
    
    unsigned char one_data[1000] = {1};
    memcpy(block1->data, one_data, 1000);

    printf("Bloco escrito na memória: %d %d\n", block1->address, block1->next);
    // faz a escrita do bloco no disco
    writeBlock(first_sector, sectors_per_block, block1);
    // lê o bloco escrito no disco para outro endereço na memória
    read_block(&block2, first_sector, sectors_per_block);
    // imprime os valores do bloco lido
    printf("\nBloco lido da memoria: %d %d\n", block2->address, block2->next);
    assert(assert_blocks_are_equal(block1, block2, sectors_per_block));
    printf("Todos os testes passaram!\n");


    return 0;
}
