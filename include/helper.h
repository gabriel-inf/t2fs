//
// Created by gabriel on 25/05/19.
//

#ifndef T2FS_HELPER_H
#define T2FS_HELPER_H

#include "../include/error.h"
#include "../include/data.h"
#include "../include/apidisk.h"
#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>


typedef void const *const STRANGE_POINTER;

unsigned get_free_block();

int read_block(Block **block, unsigned int block_index);

int get_root_directory(Directory *root_directory);

int initialize_directory(Directory* directory, unsigned int next_valid_block);

int get_dir_from_path(char *pathname, Directory *directory);

unsigned int my_awesome_pow(unsigned int base, unsigned int exp);

int validate_file_handle(unsigned int handle);
int validate_dir_handle(unsigned int handle);

int getPathAndFileName (char *filePath, char *path, char *name);

int copyBlock(int first_sector, unsigned char *copied_block);

int writeBlock(unsigned int block_index, Block *block);

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

int initialize_block(Block **block);



/**
 *
 * @param block block that will contain read information
 * @param initial_sector logical block we want to read
 * @return
 */


int get_block_first_sector(unsigned int block_index, unsigned int *first_sector);



int assert_blocks_are_equal(Block *block1, Block *block2);

unsigned get_free_block();

int is_block_free(unsigned int block_address);

int set_block_as_occupied(unsigned int block_address);

int free_block(unsigned int block_address);

int read_bitmap(unsigned char* bitmap);

int write_bitmap(unsigned char* bitmap);

void print_bitmap(size_t const size, void const const* ptr);

void print_buffer(unsigned char *buffer);

int get_block_and_position_by_index(unsigned int index, unsigned int *block_nr, unsigned int *block_data_pointer);

int init_bitmap(unsigned char *bitMap, unsigned int bitMapSize);

FILE2 get_file_handler(char *file_name);

int write_in_chain(File file, char *buffer, int size, unsigned int *current_block, unsigned int *current_written_bytes, unsigned int *next_block_address);

int write_allocating_new_blocks(char *buffer, int size, unsigned int *current_block, unsigned int *current_written_bytes, unsigned int *next_block_address);

FILE2 is_file_opened(FILE2 handler);

int get_file_by_handler (FILE2 handler, File *file);

int verifyIfDirIsOpened(DIR2 dir_id);

#endif //T2FS_HELPER_H
