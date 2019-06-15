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
    unsigned int first_sector = 30;

    unsigned char *bitmap;
    unsigned int bitmap_size;

    if (read_bitmap(&bitmap, &bitmap_size) == SUCCESS_CODE) {
        printf("Bitmap size: %d\n", bitmap_size);
        printBits(bitmap_size, bitmap);
    }

//    printf("sectors_per_block: %d,  first_sector: %d\n", sectors_per_block, first_sector);



    return 0;
}
