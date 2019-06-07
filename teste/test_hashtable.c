//
// Created by Laura Corssac on 2019-06-06.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../include/data.h"
#include "../include/hashtable.h"
#include "../include/error.h"

int main()
{

    char * gabriel = "gabriel";
    char * guerra = "guerra";

    Entry entry1;
    Entry *entry2 = malloc(sizeof(Entry));
    Entry *entry3;

    entry1.block_address = 10;
    entry1.identifier = 'f';
    entry2->block_address = 20;
    entry2->identifier = 'd';

    entry3 = malloc(sizeof(Entry));

    DataItem *hashArray[SIZE] = {NULL};

    assert(SUCCESS_CODE == addEntry(gabriel, &entry1, hashArray));
    assert(hashArray[0]->value.block_address == entry1.block_address);
    assert(SUCCESS_CODE == getValue(gabriel, &entry3, hashArray));

    assert(FILE_NOT_FOUND == removeEntry(guerra, hashArray));
    assert(FILE_NOT_FOUND == getValue(guerra, &entry3, hashArray));
    assert(entry3->block_address == 10);
    assert(entry3->identifier == 'f');

    assert(SUCCESS_CODE == removeEntry(gabriel, hashArray));

    assert(hashArray[0] == NULL);
    assert(FILE_NOT_FOUND == getValue(gabriel, &entry3, hashArray));
    assert(SUCCESS_CODE == addEntry(guerra, entry2, hashArray));
    assert(hashArray[0]->value.block_address == entry2->block_address);


    return 0;
}