#pragma once
/*
 * indexio.h -- saving and loading indexer index files
 *
 * 
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <hash.h>

/*
 * indexsave -- save the index in directory dirnm
 *
 * returns: 0 for sucess; nonzero otherwise
 *
 */

int32_t indexsave(hashtable_t *index, char* filename,  char *dirnm);

/*
 *
 * indexload -- load the index from the filename in directory dirnm
 *
 * returns: non-NULL for success; NULL otherwise;
 *
 */

hashtable_t *indexload(char* filename, char*dirnm);
