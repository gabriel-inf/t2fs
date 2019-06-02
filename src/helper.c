//
// Created by gabriel on 25/05/19.
//

#include "../include/error.h"
#include <stdio.h>
#include <string.h>

void substring(char [], char[], int, int);


// TODO: verify the /0 and limits (need to test and debug this function)
int getPathAndFileName (char *filePath, char *path, char *name) {

    int i, last_separator_position;

    if (filePath == NULL) return NULL_POINTER_EXCEPTION;
    int size = strlen(filePath);
    if (size <= 0) return EXCEPTION;
    if (filePath[0] != '/') return NOT_A_PATH_EXCEPTION;
    if (strstr(filePath, '.') != NULL) return RELATIVE_PATH_NOT_SUPPORTED_EXCEPTION;

    char* temp_path = (char*) malloc(sizeof(char));
    char* temp_name = (char*) malloc(sizeof(char));

    for (i = size-1; filePath[i] != '/', i--);
    strncpy(temp_path, filePath, i);
    substring(filePath, temp_name, i, size-i);

    strcpy(path, temp_path);
    strcpy(name, temp_name);

}

//C substring function definition
void substring(char s[], char sub[], int p, int l) {
    int c = 0;

    while (c < l) {
        sub[c] = s[p+c-1];
        c++;
    }
    sub[c] = '\0';
}