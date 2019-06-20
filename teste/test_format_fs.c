#include "../include/t2fs.h"
#include "../include/helper.h"
#include "../include/error.h"

#include <stdio.h>

int main()
{

    //int ret = format2(10);

    format2(4);

    Block *bloco;
    initialize_block(&bloco);

    printf("Returned teste\n");

    return 0;
}