//
// Created by gabriel on 06/06/19.
//

#ifndef T2FS_HASHTABLE_H
#define T2FS_HASHTABLE_H

#include "data.h"

// Definitions
// Hash is a directory's content

#define SIZE 20

typedef struct {
    Entry value;
    char *key;
}DataItem;

// Functions

/*
 * adds a new entry to a given directory's hashtable
 * returns success or error codes
 */

int addEntry(char *path, Entry *entry, DataItem **hashArray);

/*
 * removes an entry to a given directory's hashtable
 * returns success or error codes
 */

int removeEntry(char *path, DataItem **hashArray);

/*
 * get the entry struct for a given file or directory path from a directory's hashtable
 * returns success or error codes
 */

int getValue(char *path, Entry **entry, DataItem **hashArray);

#endif //T2FS_HASHTABLE_H