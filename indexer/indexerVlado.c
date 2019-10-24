/* indexer.c --- 
 * 
 * 
 * Author: Vlado Vojdanovski
 * Created: Thu Oct 24 16:55:18 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <hash.h>
#include <queue.h>


// Function that adds all word occurences in a hash

static int total_count = 0; // initialize the total_count = 0;

typedef struct word
{
	char name[50] = {0};
	int  occurences = 0;
}word_t;

void adderFunc(void* hashWord)
{
	totalCount += ((word_t*)hashWord) -> occurences; // add the datA to the totalCount
}

bool hashContainsWord(void* hashWord,(const char*)searchWord)
{
	if (strcmp((word_t*)hashWord->name, searchWord))
		{
			return true;
		}
	return false;
}

void main()
{
	// open a hash table
	wordHash = hopen(10000);

	// if we can find an element under the "normWord" key grab its int
	if ((currWord = hsearch(wordHash, hashContainsWord, normWord, strlen(currWord))))
		{
			((word_t*)currWord) -> occurences += 1; // add 1 to the word occurence
		}
	else
		{
			word_t* newWord;
			strcpy(newWord -> name, normWord);
			hput(wordHash, newWord, normWord, len(normWord)); // if we don't find it add that word to the hash
		}
	
	happly(hashWord, adderFunc); // calculate total_count
}
							 
