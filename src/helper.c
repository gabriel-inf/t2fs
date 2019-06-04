//
// Created by gabriel on 25/05/19.
//

#include "../include/error.h"
#include "../include/data.h"
#include "../include/apidisk.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void substring(char [], char[], int, int);


// TODO: verify the /0 and limits (need to test and debug this function)
int getPathAndFileName (char *filePath, char *path, char *name) {

    int i;

    if (filePath == NULL) return NULL_POINTER_EXCEPTION;
    int size = strlen(filePath);
    if (size <= 0) return EXCEPTION;
    if (filePath[0] != '/') return NOT_A_PATH_EXCEPTION;
    if (strstr(filePath, (char *)'.') != NULL) return RELATIVE_PATH_NOT_SUPPORTED_EXCEPTION;

    char* temp_path = (char*) malloc(sizeof(char));
    char* temp_name = (char*) malloc(sizeof(char));

    for (i = size-1; filePath[i] != '/'; i--);
    strncpy(temp_path, filePath, i);
    substring(filePath, temp_name, i, size-i);

    strcpy(path, temp_path);
    strcpy(name, temp_name);

    return SUCCESS_CODE;

}

int copyBlock(int first_sector, int sectors_per_block, Block *copied_block) {



    return ERROR_CODE;
}



//C substring function definition
void substring(char s[], char sub[], int p, int l) {
    int c = 0;

    while (c < l) {
        sub[c] = s[p+c-1];
        c++;
    }
    sub[c] = '\0';
}

int superBlockToBuffer(SuperBloco *superBloco, char* buffer) {

    if(buffer == NULL) return NULL_POINTER_EXCEPTION;
    if(superBloco == NULL) return NULL_POINTER_EXCEPTION;

    // copy the struct information to the serialization buffer
    snprintf(buffer, SECTOR_SIZE, "%u#%u#%u#%u#%u#%u",
             (unsigned int) superBloco->rootDirBegin,
             (unsigned int) superBloco->rootDirEnd,
             (unsigned int) superBloco->generalBlocksBegin,
             (unsigned int) superBloco->numberOfBlocks,
             (unsigned int) superBloco->bitmap_sector,
             (unsigned int) superBloco->bitmap_size
    );

    return SUCCESS_CODE;

}

int bufferToSuperBlock(char* buffer, SuperBloco *superBloco) {
    if(buffer == NULL) return NULL_POINTER_EXCEPTION;
    if(superBloco == NULL) return NULL_POINTER_EXCEPTION;

    superBloco->rootDirBegin = (unsigned int) 0;
    superBloco->rootDirEnd = (unsigned int) 0;
    superBloco->generalBlocksBegin = (unsigned int) 0;
    superBloco->numberOfBlocks = (unsigned int) 0;
    superBloco->bitmap_sector = (unsigned int) 0;
    superBloco->bitmap_size = (unsigned int) 0;

    sscanf(buffer, "%u#%u#%u#%u#%u#%u",
           &superBloco->rootDirBegin,
           &superBloco->rootDirEnd,
           &superBloco->generalBlocksBegin,
           &superBloco->numberOfBlocks,
           &superBloco->bitmap_sector,
           &superBloco->bitmap_size);

    return SUCCESS_CODE;

}

void printSuperblock(SuperBloco *superBloco) {
    printf("\nSuperBloco info:\n\trootDirBegin: %u \n\trootDirEnd: %u\n\tgeneralBlocksBegin: %u\n\tnumberOfBlocks: %u\n\tbitmap_sector: %u\n\tbitmap_size: %u bytes\n\n",
           (unsigned int) superBloco->rootDirBegin,
           (unsigned int) superBloco->rootDirEnd,
           (unsigned int) superBloco->generalBlocksBegin,
           (unsigned int) superBloco->numberOfBlocks,
           (unsigned int) superBloco->bitmap_sector,
           (unsigned int) superBloco->bitmap_size
           );
}


