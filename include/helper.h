//
// Created by gabriel on 25/05/19.
//

#ifndef T2FS_HELPER_H
#define T2FS_HELPER_H

#include "../include/error.h"
#include "../include/data.h"
#include "../include/apidisk.h"
#include <stdlib.h>

typedef void const *const STRANGE_POINTER;

int initialize_directory(Directory **directory, int next_valid_block);

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>


typedef void const *const STRANGE_POINTER;

unsigned get_free_block();

int validate_dir_handle(int handle);
int validate_file_handle(int handle);

unsigned int my_awesome_pow(unsigned int base, unsigned int exp);
int get_dir_from_path(char *pathname, Directory **directory);

int getPathAndFileName (char *filePath, char *path, char *name);

int copyBlock(int first_sector, int sectors_per_block, unsigned char *copied_block);

int writeBlock(unsigned int block_index, int sectors_per_block, Block *block);

int freeBlock(Block *block);

void printBits(size_t const size, void const const* ptr);

/**
 * Used o copy a super block to a buffer of chars
 */
int superBlockToBuffer(SuperBloco *superBloco, unsigned char *buffer);

/**
 * Used o copy a buffer of chars to a super block
 */
int bufferToSuperBlock(unsigned char *buffer, SuperBloco *superBloco);

void printSuperblock(SuperBloco *superBloco);

int get_superblock(SuperBloco *superBloco);

int initialize_block(Block **block, int sectors_per_block);

/**
 *
 * @param block block that will contain read information
 * @param initial_sector logical block we want to read
 * @param sectors_per_block number of sectors per logical block, informed in format
 * @return
 */


int get_block_first_sector(unsigned int block_index, int sectors_per_block, unsigned int *first_sector);

int read_block(Block **block, unsigned int block_index, int sectors_per_block);

int assert_blocks_are_equal(Block *block1, Block *block2, int sectors_per_block);

unsigned get_free_block();

int is_block_free(unsigned int block_address);

int set_block_as_occupied(unsigned int block_address);

int free_block(unsigned int block_address);

int read_bitmap(unsigned char* bitmap);

int write_bitmap(unsigned char* bitmap);

void print_bitmap(size_t const size, void const const* ptr);

/**
 *
 * @param handle o identificador do diretório
 * @return 1 if directory is on list and is opened
 *         0 in contrary
 */

int verifyIfDirIsOpened(DIR2 dir_id);

int get_block_and_position_by_index(unsigned int index, int sectors_per_block, unsigned int *block_nr, unsigned int *block_data_pointer);

int init_bitmap(unsigned char *bitMap, unsigned int bitMapSize);

FILE2 get_file_handler(char *file_name);

int get_file_by_handler (FILE2 handler, File &file);

int free_file_blocks(int handler);

int write_data_(int handler);

int get_block_from_write_pointer(unsigned int write_pointer, File file, Block **block);

#endif //T2FS_HELPER_H
