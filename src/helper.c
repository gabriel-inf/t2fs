//
// Created by gabriel on 25/05/19.
//

#include "../include/error.h"
#include "../include/data.h"
#include "../include/apidisk.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void substring(char originString[], char finalSubstring[], int start, int last);


// TODO: verify the /0 and limits (need to test and debug this function)
int getPathAndFileName (char *filePath, char *path, char *name) {

    int i;

    if (filePath == NULL) return NULL_POINTER_EXCEPTION;
    int size = strlen(filePath);
    if (size <= 0) return EMPTY_LINE_EXCEPTION;
    if (filePath[0] != '/') return NOT_A_PATH_EXCEPTION;

    char* temp_path = (char*) malloc(sizeof(char));
    char* temp_name = (char*) malloc(sizeof(char));

    for (i = size-1; filePath[i] != '/'; i--);
    strncpy(temp_path, filePath, i);
    substring(filePath, temp_name, i + 2, size);

    if (strlen(temp_name) > 31)
        return INVALID_SIZE_FOR_FILE_NAME; //O T2FS deverá suportar arquivos com nomes formados por até 31 caracteres alfanuméricos (0‐9, a‐z e A‐Z). Os nomes são case‐sensitive.

    strcpy(path, temp_path);
    strcpy(name, temp_name);

    return SUCCESS_CODE;

}

int copyBlock(int first_sector, int sectors_per_block, Block *copied_block) {



    return ERROR_CODE;
}



//C substring function definition
void substring(char originString[], char finalSubstring[], int start, int last) {
    int c = 0;

    while (c < last) {
        finalSubstring[c] = originString[start + c - 1];
        c++;
    }
    finalSubstring[c] = '\0';
}

int superBlockToBuffer(SuperBloco *superBloco, char *buffer) {

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

int bufferToSuperBlock(char *buffer, SuperBloco *superBloco) {
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

/**
 * Take the sector, serialize it someway and persist
 *
 */
int writeBlock(unsigned int first_sector, int sectors_per_block, Block *block) {

    int sector_number;
    char a, b, c, d, e, f, g, h, i;
    char first_bytes[8];

    char current_sector[256] = {'\0'};

    block->address = 10;
    // extrating bytes from address
    first_bytes[0] = (char) (block->address & 0xFF); //extract first byte
    first_bytes[1] = (char) ((block->address >> 8) & 0xFF); //extract second byte
    first_bytes[2] = (char) ((block->address >> 16) & 0xFF); //extract third byte
    first_bytes[3] = (char) ((block->address >> 24) & 0xFF); //extract fourth byte

    // extrating bytes from next
    first_bytes[4] = (char) (block->address & 0xFF); //extract first byte
    first_bytes[5] = (char) ((block->address >> 8) & 0xFF); //extract second byte
    first_bytes[6] = (char) ((block->address >> 16) & 0xFF); //extract third byte
    first_bytes[7] = (char) ((block->address >> 24) & 0xFF); //extract fourth byte

    for (i = 0; i < 8; i++) {
        current_sector[i] = first_bytes[i];
    }

    // nesse ponto me sobraram 256 - 8 bytes = 248 no primeiro setor
    //vamos fazer o stream a partir do byte 8
    int end_stream = 256 * sectors_per_block;
    int current_writting_byte = sizeof(unsigned int) * 2; //considerando os valores que guardamos para endereço e next
    int recorded_sectors = 0;
    int written_bytes_from_data = 0;
    int current_sector_byte = current_writting_byte;


    while (current_writting_byte < end_stream && written_bytes_from_data < strlen(block->data)) {
        while (current_sector_byte < 256 && written_bytes_from_data < strlen(block->data)) {
            current_sector[current_sector_byte++] = block->data[written_bytes_from_data++];
            current_writting_byte++;
        }
        printf("\n -> Write sector nr {%d}:\n", recorded_sectors + first_sector);
        for (i = 0; i < 256; i++) {
            printf("%c", current_sector[i]);

        }


        recorded_sectors++;
        current_sector_byte = 0;

        for (i = 0; i < 256; i++) {
            current_sector[i] = '\0';
        }
    }
}


// add this
int initialize_block(Block **block, int sectors_per_block) {

    *block = (Block *) malloc(sizeof(Block));

    (*block)->address = 0; //TODO: aqui a gente ja pode fazer um get pra achar o setor que pode ser o inicio do bloco, ou seja, tem que ter o númeor de setores disponível consecutivamente
    (*block)->next = 0;
    (*block)->data = malloc(sizeof(char) * sectors_per_block);
}

