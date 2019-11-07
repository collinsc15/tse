#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <hash.h>
#include <queue.h>
#include <indexio.h>
#define BUFFER_SIZE BUFSIZ
typedef struct doc {                                                                                                                                                                  
  char name[50];                                                                                                                                                                      
  int occurences;                                                                                                                                                                     
} doc_t;                                                                                                                                                                              
                                                                                                                                                                                       
typedef struct word {                                                                                                                                                                 
  char name[50];                                                                                                                                                                      
  queue_t *docs;                                                                                                                                                                      
} word_t;
typedef struct rankedDoc{
	int rank;
	char id[20];
	char url[100];
} rank_t;

void freeDoc(void *doc){
	free(doc);
}

void CIQ(void* hashWord){
	//qapply(((word_t*)hashWord)->docs,freeDoc);
  qclose((((word_t*)hashWord)->docs));
}



bool hWord(void* hashWord, const void* searchWord){
  word_t *obj =(word_t*)hashWord;
	if (!strcmp(obj->name, (char*)searchWord)) {
    return true;
  }	
  return false;
}

bool hRank(void* rank, const void* searchWord){
  rank_t *obj =(rank_t*)rank;
	if (!strcmp(obj->id, (char*)searchWord)) {
    return true;
  }	
  return false;
}

void printR(void *r){
	rank_t *rank=(rank_t*)r;
	printf("Rank:%d:DocID:%s:URL:%s\n",rank->rank,rank->id,rank->url);
	fflush(stdout);
}

int main(int argc, const char **argv) {
	int valid = 0;
	char result[100];
	char input[100];
	hashtable_t *words;
	queue_t *ranked;
	printf(">");
	fgets(input, 100, stdin);
	//chdir("../indexes");
	bool or = false;
	
	while(strcmp(input, "quit\n") !=0){
		valid = 0;
		words =indexload("depth1","indexes");
		//ranked=hopen(100);
		//clear new line 
		input[strlen(input) -1] = '\0';
		result[0]='\0';
		int i;
		for (i = 0; i<strlen(input); i++){
			char current = input[i];
			char previous = '\0';
			if(i>0){
				previous = input[i-1];
			}
			if(isalpha(current)!=0 || (isspace(current) != 0 && isspace(previous) == 0)){
				int len = strlen(result);
				result[len] = tolower(current);
				result[len + 1] = '\0';
				
			}
			if(ispunct(current)!=0 || isdigit(current)!= 0){
				printf("[invalid query]\n");
				valid = 1;
				continue;
			}	
		}
		if (strlen(input)==0){
			valid = 1;
		}
		int count =0;
		if(valid == 0){
			char *word;
			word=strtok(result, " ");
			//int rank=NULL;
			ranked=qopen();
			while(word!=NULL){
				//char *exists[100];
				count +=1;
				
				//memset(searched, '\0', sizeof(char)*50);
				if((strlen(word)>2)||(strcmp(word,"or")==0)){
					//printf("%s\n",word);
					//while(fscanf(f,"%s%*[^\n]",searched)==1){
					//if(strcmp(word,"or")==0){
					//or=true;
					//count=1;
					//}
					queue_t *newQueue = qopen();
					word_t *w=(word_t*)hsearch(words,hWord,word,strlen(word));
					if (w){
						//printf("found");
						if(strcmp(w->name,"and")!=0){
							doc_t *d=(doc_t*)qget(w->docs);
							while(d){
								int amount = d->occurences;
								rank_t *r=(rank_t *) qsearch(ranked, hRank, d->name);
								if (r){
									//printf("%s:%d ",word,amount);
									rank_t *rr=qremove(ranked, hRank,r->id);
									printf("success");
									qput(newQueue,rr);
									fflush(stdout);
									if ((amount<r->rank)&&(!or)){
										r->rank=amount;
									}
									else if (or){
										r->rank+=amount;
									}
								}
								else if (count==1){
									rank_t *newRanked;
									newRanked = (rank_t *)malloc(sizeof(rank_t));
									chdir("../FQc");
									FILE *f=fopen(d->name,"r");
									fscanf(f,"%s",newRanked->url);
									fclose(f);
									newRanked->rank = d->occurences;
									strcpy(newRanked->id, d->name);
									qput(newQueue, newRanked);
									//printf("%s",newRanked->id);
									//free(newRanked);
								}
								free(d);
								d=(doc_t*)qget(w->docs);
							}
							qclose(ranked);
							ranked =NULL;
							ranked=newQueue;
						}
						
						
					}
					
					
					
				}
				//if((or)&&(strcmp(word,"or")!=0)){
				//or=false;
				//}

				word=strtok(NULL," ");	
			}
			
			
		}
		or=false;
		//printf("made it through");
		qapply(ranked,printR);
		qclose(ranked);
		//indexsave(words, "oof", "indexes");
		happly(words,CIQ);
		hclose(words);
		//printf("%d",strlen(input));
		memset(result, '\0', sizeof(char)*100);
		printf(">");
		fgets(input, 100, stdin);
		
	}
	

	
	return 0;
	
}
