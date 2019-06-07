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
    unsigned int address; // endereço do setor do bloco
    unsigned int next;    // próximo bloco a ser escrito
    char *data;           // aqui a gente vai ter que fazer a conta pro malloc do data quando soubermos o nr de setores por bloco
} Block;

typedef struct {
    Entry directoryEntry;
    HashTable *entries; // entries for all the directories and files
}Directory;

typedef struct {
    unsigned int blockAddress; // address for the first block
    char identifier; // f for file, d for directory
}Entry;

typedef struct {

}HashTable;

#endif //T2FS_DATA_H
