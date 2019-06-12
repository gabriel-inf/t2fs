//
// Created by gabriel on 25/05/19.
//

#ifndef T2FS_HELPER_H
#define T2FS_HELPER_H

#include "../include/error.h"
#include "../include/data.h"
#include "../include/apidisk.h"

int getPathAndFileName (char *filePath, char *path, char *name);

int copyBlock(int first_sector, int sectors_per_block, unsigned char *copied_block);

int writeBlock(unsigned int first_sector, int sectors_per_block, Block *block);

int freeBlock(Block *block);

void printBits(size_t const size, void const *const ptr);

/**
 * Used o copy a super block to a buffer of chars
 */
int superBlockToBuffer(SuperBloco *superBloco, unsigned char *buffer);

/**
 * Used o copy a buffer of chars to a super block
 */
int bufferToSuperBlock(unsigned char *buffer, SuperBloco *superBloco);
void printSuperblock(SuperBloco *superBloco);

int initialize_block(Block **block, int sectors_per_block);

/**
 *
 * @param block block that will contain read information
 * @param initial_sector logical block we want to read
 * @param sectors_per_block number of sectors per logical block, informed in format
 * @return
 */

int read_block(Block **block, int initial_sector, int sectors_per_block);

int assert_blocks_are_equal(Block *block1, Block *block2, int sectors_per_block);

#endif //T2FS_HELPER_H
