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
				 //total_count += ((word_t*)hashWord) -> occurences; // add the datA to the totalCount
	//printf("%s:%d\n",((word_t*)hashWord)->name,((word_t*)hashWord)->occurences)
}

void docSprintf(void *doc){ 
  fprintf(hashFile," %s",((doc_t*)doc)->name);
	//fprintf(hashFile," NAME:%s",((doc_t*)doc)->name); 
  //fprintf(hashFile," OCCURENCES:%d ",((doc_t*)doc)->occurences);
	fprintf(hashFile," %d ",((doc_t*)doc)->occurences); 
}

void indexSprintf(void* hashWord){
  fprintf(hashFile,"%s",((word_t*)hashWord)->name);
	//fprintf(hashFile,"WORD:%s",((word_t*)hashWord)->name);  
  lqapply(((word_t*)hashWord)->docs, docSprintf); 
  fprintf(hashFile,"\n");                                                                                                                                                                                                                                                                   
}


void closeThoseDamnQueues(void* hashWord) {
  lqclose(((word_t*)hashWord)->docs); // add the datA to the totalCount
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
				//printf("%s\n",currWord);
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
					//word_t *e=(word_t*)lhsearch(wordHash, hashContainsWord, currWord, strlen(currWord));
				//sees if hash contains word
				
					//	if (e){  //if hash contains word
					//doc_t *d=(doc_t*)lqsearch(e->docs, queueContainsDoc, id);  //checks if word queue contains doc
					//if(d!=NULL){           //if it does
				//d->occurences += 1; // add 1 to the word occurence
					//}
					//else{
					//doc_t *newDoc;                           //create new doc
					//if (!(newDoc=(doc_t*) malloc(sizeof(doc_t)))){
					//	printf("malloc issue");
					//	return NULL;
					//}
					//memset(newDoc->name, 0,(50*sizeof(newDoc->name[0])));
					//strcpy(newDoc->name, id);
					//newDoc->occurences=1;
					//lqput(e->docs,newDoc);      //put doc in word queue
					//}
					//}
					//else {             // if hash does not contain word
					//word_t* newWord;    //creates word
					//if (!(newWord=(word_t*) malloc(sizeof(word_t)))){
					//printf("malloc issue");
					//return NULL;
					//}
					//memset(newWord->name, 0,(50*sizeof(newWord->name[0])));
					//strcpy(newWord->name, currWord);
					//newWord->docs=lqopen();
					//doc_t *newDoc;     //create doc
					//if (!(newDoc=(doc_t*) malloc(sizeof(doc_t)))){
					//printf("malloc issue");
					//return NULL;
					//}
					//strcpy(newDoc->name, id);
					//newDoc->occurences=1;
					//lqput(newWord->docs,newDoc);
					//lhput(wordHash, newWord, currWord, strlen(currWord)); // if we don't find it add that word to the hash
					//}		
			}
			//printf("w:%d",sizeof(webpage_t));
			
			free(currWord);
		}
		//free(webpage_getURL(w));
		webpage_delete(w);
		num+=args->modNum;
		//free(currWord);
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
	//long int num=atol(argv[1]);
	//printf("%d",num);
	hashtable_t *wordHash = lhopen(100);
	char* loadDir = (char*)malloc(100*sizeof(char));
	char* saveDir = "../indexes/";

	char* nameOfFile = (char*)malloc(100*sizeof(char));
	strcpy(nameOfFile, argv[2]); // NEED TO : change 3 to 2

	char fullSave[100] = {0};
	sprintf(fullSave,"%s%s",saveDir,nameOfFile);
	strcpy(loadDir, argv[1]); // NEED TO: change 2 to 1
	long int num=atol(argv[3]);
	
	generateThreads(num, loadDir, wordHash);
	
	//webpage_delete(w);
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
	//fclose(f);
	//printf("words:%d",total_count);
	//printf("\nobjects:%d",total_obj);
	return 0;
}
