//
// Created by gabriel on 06/06/19.
//

#ifndef T2FS_HASHTABLE_H
#define T2FS_HASHTABLE_H

#include "../include/t2fs.h"
#include "../include/data.h"

// Definitions
// Hash is a directory's content

// Functions

//int initialize_hashTable(DataItem **hashArray);

/**
 *
 * @param path the key in the hashtable
 * @param entry new entry (value) in hashtable
 * @param hashArray an array or pairs (key, value) representing a directory's hashtable
 * @return 0 when success != 0 when error
 */

int addEntry(char *path, DIRENT2 *entry, DataItem **hashArray);

/*
 * removes an entry to a given directory's hashtable
 * returns success or error codes
 */

int removeEntry(char *path, DataItem **hashArray);

/*
 * get the entry struct for a given file or directory path from a directory's hashtable
 * returns success or error codes
 */

int getValue(char *path, DIRENT2 **entry, DataItem *hashArray);


//TIRAR ESSA coisa

int readdir1 (DIR2 handle, DIRENT2 *dentry);

#endif //T2FS_HASHTABLE_H