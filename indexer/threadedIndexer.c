/* indexer.c --- 
 * 
 * Author: Claire C. Collins, Vlado Vojdanovski
 * Created: Sun Oct 27 18:39:02 2019 (-0400)
 * Description: An elegant multi-threaded implementation of our indexer. Uses several helper functions as well as a
 * helper struct to ensure the main loop is as clean as readable as possible. We ensure that no webpage is indexed twice by using 
 * modular arithmetic. The shared resource is the document we save the indeces in.
 * 
 */
#include <stdio.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>
#include <stdlib.h>
#include <lhash.h>
#include <hash.h>
#include <pthread.h>
#include <lqueue.h>
#include <queue.h>                                                             // Function that adds all word occurences in a hash
// Function that adds all word occurences in a hash
static int total_count = 0; // initialize the total_count = 0;
static int total_obj = 0; // initialize the total_count = 0;
static FILE *hashFile;

typedef struct doc {
	char name[50];
	int occurences;
} doc_t;

typedef struct args {
	lockedhash_t *index;
	int modNum;
	int threadNum;
	char *loadDir;
} args_t;

typedef struct word {
	char name[50];
	lqueue_t *docs;
} word_t;

void add(void *docq){
	total_count+=((doc_t*)docq)->occurences;
}

void adderFunc(void* hashWord) {
	lqapply(((word_t*)hashWord)->docs,add);
}

void docSprintf(void *doc){ 
  fprintf(hashFile," %s",((doc_t*)doc)->name);
  fprintf(hashFile," %d ",((doc_t*)doc)->occurences); 
}

void indexSprintf(void* hashWord){
  fprintf(hashFile,"%s",((word_t*)hashWord)->name);
  lqapply(((word_t*)hashWord)->docs, docSprintf); 
  fprintf(hashFile,"\n");                                                                                                                                                                                                                                                                   
}


void closeThoseDamnQueues(void* hashWord) {
  lqclose(((word_t*)hashWord)->docs); // add the data to the totalCount
}

void addObj(void* hashWord){
	total_obj+=1;
}

bool hashContainsWord(void* hashWord, const void* searchWord){
	word_t *obj =(word_t*)hashWord;
	if (!strcmp(obj->name, (char*)searchWord)) {
		return true;
	}
  return false;
}

bool queueContainsDoc(void* doc, const void* searchWord){
	doc_t *obj =(doc_t*)doc;
	if (!strcmp(obj->name, (char*)searchWord)) {
		return true;
	}
  return false;
}

void NormalizeWord(char word[]){                                               
  char *p = word;  
	//check that word has more than three characters
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

void *makeIndex(void *arg){
	args_t *args= (args_t *)arg;
	int num = args->threadNum;
	webpage_t* w;
	lockedhash_t *wordHash=args->index;
	while ((w = pageload(num, args->loadDir))){
		webpage_fetch(w);
		char id[5];
		sprintf(id,"%d",num);
		int pos = 0;
		char *currWord;
		
		while ((pos = webpage_getNextWord(w, pos, &currWord)) > 0) { //for words in webpage w
			
			NormalizeWord(currWord);        //normalize word
			if(strcmp(currWord,"")){        //run if valid word
				word_t* newWord;    //creates word
					if (!(newWord=(word_t*) malloc(sizeof(word_t)))){
						printf("malloc issue");
						return NULL;
					}
					memset(newWord->name, 0,(50*sizeof(newWord->name[0])));
					strcpy(newWord->name, currWord);
					newWord->docs=lqopen();
					word_t *hashWord=(word_t *)lhadd(wordHash, hashContainsWord, currWord, strlen(currWord), (void *)newWord);
					if(hashWord!=newWord){
						lqclose(newWord->docs);
						free(newWord);
					}
					doc_t *newDoc;                           //create new doc
					if (!(newDoc=(doc_t*) malloc(sizeof(doc_t)))){
						printf("malloc issue");
						return NULL;
					}
					memset(newDoc->name, 0,(50*sizeof(newDoc->name[0])));
					strcpy(newDoc->name, id);
					newDoc->occurences=0;
					doc_t *doc= (doc_t *)lqadd(hashWord->docs,queueContainsDoc, id, (void *)newDoc);
					doc->occurences+=1;	
			}			
			free(currWord);
		}
		webpage_delete(w);
		num+=args->modNum;
	}	
	return NULL;
}

void generateThreads(int num, char* dirName, lockedhash_t *wordHash){
	pthread_t threads[num];
	args_t *args[num];
	for(int i=0; i<num; i++){
		args[i]=(args_t *)malloc(sizeof(args_t));
		args[i]->index=wordHash;
		args[i]->modNum=num;
		args[i]->threadNum=i+1;
		args[i]->loadDir=dirName;
		pthread_create(&(threads[i]), NULL, makeIndex, (void *)args[i]);
	}
	for(int i=0; i<num; i++){
		pthread_join(threads[i], NULL);
		free(args[i]);
	}

}

int main(int argc, char *argv[]){
	if (argc != 4){
		printf("usage: indexer <pagedir> <indexnm> <numThreads>\n");
		exit(EXIT_FAILURE);
	}
	hashtable_t *wordHash = lhopen(100);
	char* loadDir = (char*)malloc(100*sizeof(char));
	char* saveDir = "../indexes/";

	char* nameOfFile = (char*)malloc(100*sizeof(char));
	strcpy(nameOfFile, argv[2]);

	char fullSave[100] = {0};
	sprintf(fullSave,"%s%s",saveDir,nameOfFile);
	strcpy(loadDir, argv[1]); 
	long int num=atol(argv[3]);
	
	generateThreads(num, loadDir, wordHash);

	lhapply(wordHash, adderFunc); // calculate total_count
	lhapply(wordHash, addObj);
	hashFile = fopen(fullSave,"w");
	if (hashFile){
		lhapply(wordHash, indexSprintf);
		fclose(hashFile);
	}
	lhapply(wordHash, closeThoseDamnQueues);
	lhclose(wordHash);
	free(nameOfFile);
	free(loadDir);
	fflush(stdout);
	return 0;
}
