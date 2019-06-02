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
    char* bitmap;
    unsigned int bitmap_size;
    unsigned int bitmapBegin;
    unsigned int bitmapEnd;

}SuperBloco;

#endif //T2FS_DATA_H
