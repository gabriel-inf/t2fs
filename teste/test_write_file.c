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

    File file1 = malloc(File);
    file1.read_write_pointer = 0;
    file1.first_block = 2;
    strcpy(file1.name, "testee");


    files_opened[0] = file1;

    return 0;
}
