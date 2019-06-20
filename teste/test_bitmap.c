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

    unsigned char *super_block_buffer = malloc(SECTOR_SIZE);
    SuperBloco superBloco;
    if (read_sector(SUPER_BLOCK_SECTOR, super_block_buffer));
    bufferToSuperBlock(super_block_buffer, &superBloco);

    printf("\t*Testing bitmaps\n\n");

    int sectors_per_block = 4;
    format2(sectors_per_block);
    unsigned char *bitmap_sector_buffer = malloc(SECTOR_SIZE);

    unsigned int first_sector = 30;
    unsigned char *bitmap = malloc(SECTOR_SIZE);
    unsigned int bitmap_size = superBloco.bitmap_size;

    assert (read_bitmap(bitmap) == SUCCESS_CODE);
    printf("1. Read bitmap to buffer - OK\n . bimap: ");
    if (bitmap_size % 8 == 0) {
        printBits((int)bitmap_size/8, bitmap);
    } else {
        printBits((int)(bitmap_size/8) + 1, bitmap);
    }

    printf("2. Testing if all the bitmap is entire free: \n\n");
    int k;
    for (k = 1; k<bitmap_size; k++) {

        assert(is_block_free(k) == 1);
        printf("b%d - OK; ", k);

    }
    printf("\n\n-> todos blocos livres: OK \n\n");

    printf("3. Testing bitmap allocation: \n\n");
    for (k = 1; k<bitmap_size; k++) {

        assert(set_block_as_occupied(k) == SUCCESS_CODE);
        assert(!is_block_free(k));
        printf("(b%d - OK; ", k);
        printf("next free: %d), ", get_free_block());

    }
    printf("\n\n-> todos blocos ocupados: OK \n\n");
    printf("Bitmap apos ocupar todos: ");
    read_bitmap(bitmap);
    printBits(4, bitmap);


    printf("4. Testing bitmap de-allocation: \n\n");
    for (k = 1; k<bitmap_size; k++) {

        assert(free_block(k) == SUCCESS_CODE);
        assert(is_block_free(k));
        printf("b%d - OK; ", k);

    }
    printf("\n\n-> todos blocos desalocados: OK \n\n");

    read_bitmap(bitmap);
    printf("Bitmap apos desocupar todos: ");
    printBits(4, bitmap);

    printf("SUCESSO PESSOAL\n");



    //printf("Bloco 1 tá livre\n");





    return 0;
}
