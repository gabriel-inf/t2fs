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

    printf("Testing bitmaps\n");
    int sectors_per_block = 4;
    format2(sectors_per_block);
    unsigned char *bitmap_sector_buffer = malloc(SECTOR_SIZE);
    read_sector(66, bitmap_sector_buffer);
    printBits(4, bitmap_sector_buffer);

    unsigned int first_sector = 30;

    unsigned char *bitmap = malloc(SECTOR_SIZE);
    unsigned int bitmap_size;

    if (read_bitmap(bitmap, &bitmap_size) == SUCCESS_CODE) {
        printf("Bitmap sector: %d\n", bitmap_size);
        if (bitmap_size % 8 == 0) {
            printBits((int)bitmap_size/8, bitmap);
        } else {
            printBits((int)(bitmap_size/8) + 1, bitmap);
        }

    }

    assert(is_block_free(1, bitmap));

    printf("bitmap size: %d; sectors_per_block: %d,  first_sector: %d\n", bitmap_size, sectors_per_block, first_sector);



    return 0;
}
