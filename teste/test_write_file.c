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


    format2(4);
    File file1;
    file1.read_write_pointer = 0;
    file1.first_block = 2;
    strcpy(file1.name, "/orange/file1");
    file1.fileSize = 29 + 8;
    file1.valid = 1;

    files_opened[0] = file1;

    char *buffer = malloc(sizeof(char));
    int i;
    for (i=0; i< 29; i++) {
        buffer[i] = 'g';
    }

    assert(write2(0, buffer, 29) >= 0);
    printf("Escreveu o arquivo\n");

    return 0;
}
