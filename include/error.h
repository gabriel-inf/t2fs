//
// Created by gabriel on 25/05/19.
//

#ifndef T2FS_ERROR_H
#define T2FS_ERROR_H

// EXCEPTIONS ARE NEGATIVE NUMBERS
#define EXCEPTION -1
#define EMPTY_LINE_EXCEPTION -2
#define NULL_POINTER_EXCEPTION -3
#define MALLOC_ERROR_EXCEPTION -7
#define NOT_A_PATH_EXCEPTION -8
#define RELATIVE_PATH_NOT_SUPPORTED_EXCEPTION -9
#define FAILED_TO_READ_SECTOR -10
#define FAILED_TO_WRITE_SECTOR -11
#define INVALID_SIZE_FOR_FILE_NAME -12
#define HASH_TABLE_FULL -13
#define INDEX_OUT_OF_RANGE -14
#define KEY_ALREADY_PRESENT -15

#define FILE_NOT_FOUND -404

// -------------------------------
#define SUCCESS_CODE 0
#define ERROR_CODE -100
#define FULL_BLOCKS -999
// -------------------------------
#define DEBUG 0

#endif //T2FS_ERROR_H
