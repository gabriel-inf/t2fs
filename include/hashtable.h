//
// Created by gabriel on 06/06/19.
//

#ifndef T2FS_HASHTABLE_H
#define T2FS_HASHTABLE_H

#include "../include/t2fs.h"

// Definitions
// Hash is a directory's content

#define SIZE 20

typedef struct {
    DIRENT2 value;
    char *key;
}DataItem;

// Functions

/*
 * adds a new entry to a given directory's hashtable
 * returns success or error codes
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

int getValue(char *path, DIRENT2 **entry, DataItem **hashArray);

#endif //T2FS_HASHTABLE_H