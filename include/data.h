//
// Created by eduar on 28/05/2019.
//

#ifndef T2FS_DATA_H
#define T2FS_DATA_H

typedef struct{
    unsigned int rootDirBegin;
    unsigned int rootDirEnd;
    unsigned int generalBlocksBegin;
    unsigned int numberOfBlocks;
    unsigned int bitmap_sector;
    unsigned int bitmap_size; // tamanho em bytes do bitmap

}SuperBloco;

typedef struct {
    unsigned int address;
    unsigned int next;
    char* data;
}Block;

typedef struct {
    Entry directoryEntry;
    HashTable *entries; // entries for all the directories and files
}Directory;

typedef struct {
    unsigned int block_address; // address for the first block
    char identifier; // f for file, d for directory
}Entry;

typedef struct {

}HashTable;

#endif //T2FS_DATA_H
