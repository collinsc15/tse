/* indexer.c --- 
1;5202;0c * 
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
static FILE *hashFile;

typedef struct doc {
	char name[50];
	int occurences;
} doc_t;

typedef struct word {
	char name[50];
	queue_t *docs;
} word_t;

void add(void *docq){
	total_count+=((doc_t*)docq)->occurences;
}

void adderFunc(void* hashWord) {
	qapply(((word_t*)hashWord)->docs,add);
				 //total_count += ((word_t*)hashWord) -> occurences; // add the datA to the totalCount
	//printf("%s:%d\n",((word_t*)hashWord)->name,((word_t*)hashWord)->occurences)
}

void docSprintf(void *doc){ 
  fprintf(hashFile," %s",((doc_t*)doc)->name); 
  fprintf(hashFile," %d ",((doc_t*)doc)->occurences); 
}

void indexSprintf(void* hashWord){
  fprintf(hashFile,"%s",((word_t*)hashWord)->name);  
  qapply(((word_t*)hashWord)->docs, docSprintf); 
  fprintf(hashFile,"\n");                                                                                                                                                                                                                                                                   
}


void closeThoseDamnQueues(void* hashWord) {
  qclose(((word_t*)hashWord)->docs); // add the datA to the totalCount
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

bool queueContainsDoc(void* doc, const void* searchWord){
	doc_t *obj =(doc_t*)doc;
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


int main(int argc, char *argv[]){
	if (argc != 3){
		printf("usage: indexer <id>\n");
		exit(EXIT_FAILURE);
	}
	//long int num=atol(argv[1]);
	//printf("%d",num);
	hashtable_t *wordHash = hopen(100);
	char* loadDir = (char*)malloc(100*sizeof(char));
	char* saveDir = "./indexes/";

	char* nameOfFile = (char*)malloc(100*sizeof(char));
	strcpy(nameOfFile, argv[2]); // NEED TO : change 3 to 2

	char fullSave[100] = {0};
	sprintf(fullSave,"%s%s",saveDir,nameOfFile);
	strcpy(loadDir, argv[1]); // NEED TO: change 2 to 1

	int num = 1;
	webpage_t* w;
	while ((w = pageload(num, loadDir))){
		
		num+=1;
		webpage_fetch(w);
		char id[5];
		sprintf(id,"%d",num);
	
	

	
	//free(webpage_getURL(w));
  //get next word
	
	
  int pos = 0;
	char *currWord;
  //testing NormalizeWord  
	//chdir("../forIndexer");
	
	//FILE *f=fopen("1","a");
	while ((pos = webpage_getNextWord(w, pos, &currWord)) > 0) { //for words in webpage w
		
		NormalizeWord(currWord);        //normalize word
    if(strcmp(currWord,"")){        //run if valid word
			printf("%s\n",currWord);
			word_t *e=(word_t*)hsearch(wordHash, hashContainsWord, currWord, strlen(currWord));
			//sees if hash contains word
			
			if (e){  //if hash contains word
				doc_t *d=(doc_t*)qsearch(e->docs, queueContainsDoc, id);  //checks if word queue contains doc
				if(d!=NULL){           //if it does
					d->occurences += 1; // add 1 to the word occurence
				}
				else{
					doc_t *newDoc;                           //create new doc
					if (!(newDoc=(doc_t*) malloc(sizeof(doc_t)))){
						printf("malloc issue");
						return 7;
					}
					memset(newDoc->name, 0,(50*sizeof(newDoc->name[0])));
					strcpy(newDoc->name, id);
					newDoc->occurences=1;
					qput(e->docs,newDoc);      //put doc in word queue
				}
					//printf("more than 1 %s",currWord);
			}
			else {             // if hash does not contain word
				word_t* newWord;    //creates word
				if (!(newWord=(word_t*) malloc(sizeof(word_t)))){
					printf("malloc issue");
					return 7;
				}
				memset(newWord->name, 0,(50*sizeof(newWord->name[0])));
				strcpy(newWord->name, currWord);
				//snprintf(newWord->name, 50,"%s", currWord);
				newWord->docs=qopen();
				doc_t *newDoc;     //create doc
				if (!(newDoc=(doc_t*) malloc(sizeof(doc_t)))){
						printf("malloc issue");
						return 7;
				}
				strcpy(newDoc->name, id);
				newDoc->occurences=1;
				qput(newWord->docs,newDoc);
				hput(wordHash, newWord, currWord, strlen(currWord)); // if we don't find it add that word to the hash
				//free(newWord);
			}
			
		}  
		
		free(currWord);
	}
	
	
	
	webpage_delete(w);
	}
	happly(wordHash, adderFunc); // calculate total_count
	happly(wordHash, addObj);
	hashFile = fopen(fullSave,"w");
	if (hashFile){
		happly(wordHash, indexSprintf);
		fclose(hashFile);
	}
	happly(wordHash, closeThoseDamnQueues);
	hclose(wordHash);
	free(nameOfFile);
	free(loadDir);
	//fclose(f);
	printf("words:%d",total_count);
	printf("\nobjects:%d",total_obj);
	return 0;
}
