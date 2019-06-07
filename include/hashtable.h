//
// Created by gabriel on 06/06/19.
//

#ifndef T2FS_HASHTABLE_H
#define T2FS_HASHTABLE_H

#include "data.h"

// hash is the directory content
int addEntry(char *path, Entry *entry);
int removeEntry(char *path);
int getValue(char *path, Entry **entry);

#endif //T2FS_HASHTABLE_H