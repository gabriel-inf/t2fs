//
// Created by gabriel on 25/05/19.
//

#ifndef T2FS_HELPER_H
#define T2FS_HELPER_H


int getPathAndFileName (char *filePath, char *path, char *name);

int copyBlock(int first_sector, int sectors_per_block, char *copied_block);


#endif //T2FS_HELPER_H
