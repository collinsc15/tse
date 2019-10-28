/* indexer.c --- 
 * 
 * 
 * Author: Claire C. Collins
 * Created: Sun Oct 27 18:39:02 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>
#include <stdlib.h>
#include <hash.h>
#include <queue.h>                                                             // Function that adds all word occurences in a hash
static int total_count = 0; // initialize the total_count = 0;
static int total_obj = 0; // initialize the total_count = 0;
typedef struct word {
	char name[50];
	int occurences;
} word_t;

void adderFunc(void* hashWord) {
  total_count += ((word_t*)hashWord) -> occurences; // add the datA to the totalCount
	printf("%s:%d\n",((word_t*)hashWord)->name,((word_t*)hashWord)->occurences);

}

void delete(void* hashWord) {
  free((word_t*)hashWord); // add the datA to the totalCount
}

void addObj(void* hashWord){
	total_obj+=1;
}

bool hashContainsWord(void* hashWord, const void* searchWord){
	word_t *obj =(word_t*)hashWord;
	//strcpy(h,obj->name);
	//printf("objectname:%s\n",obj->name);
	if (!strcmp(obj->name, (char*)searchWord)) {
		return true;
	}
  return false;
}

void NormalizeWord(char word[]){                                               
  char *p = word;                                                                //check that word has more than three characters
	int size = strlen(word);
	if(size < 3){                                                                
    strcpy(word, "");
	}
		while (*p){
			if(!isalpha(*p)){
				strcpy(word, "");
				break;                                                                 
			}
			*p = tolower(*p);
			p++;
		}                                                                        
}


int main(void){
	webpage_t *w = pageload(1, "pages");
	webpage_fetch(w);
		 
	
	//free(webpage_getURL(w));
  //get next word
	hashtable_t *wordHash = hopen(100);
  int pos = 0;
	char *currWord;
  //testing NormalizeWord  
	//chdir("../forIndexer");
	
	//FILE *f=fopen("1","a");
	while ((pos = webpage_getNextWord(w, pos, &currWord)) > 0) {
		NormalizeWord(currWord);
    if(strcmp(currWord,"")){
       // if we can find an element under the "normWord" key grab its int
			printf("%s\n",currWord);
			word_t *e=(word_t*)hsearch(wordHash, hashContainsWord, currWord, strlen(currWord));
			if ( NULL != e){
				e->occurences += 1; // add 1 to the word occurence
				//printf("more than 1 %s",currWord);
			}
			else {
				word_t* newWord;
				if (!(newWord=(word_t*) malloc(sizeof(word_t)))){
					printf("malloc issue");
					return 7;
				}
				memset(newWord->name, 0,(50*sizeof(newWord->name[0])));
				strcpy(newWord->name, currWord);
				//snprintf(newWord->name, 50,"%s", currWord);
				newWord->occurences=1;
				hput(wordHash, newWord, currWord, strlen(currWord)); // if we don't find it add that word to the hash
				//free(newWord);
			}
			
		}  
		
		free(currWord);
	}
	happly(wordHash, adderFunc); // calculate total_count
	happly(wordHash, addObj);
	happly(wordHash, delete);
	hclose(wordHash);

	webpage_delete(w);
	//fclose(f);
	printf("%d",total_count);
	printf("\nobjects:%d",total_obj);
	return 0;
}
