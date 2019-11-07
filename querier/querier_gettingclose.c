/*
1;5202;0c1;5202;0c0;136;0c1;5202;0c





made


 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <hash.h>
#include <queue.h>
#include <indexio.h>
#include <dirent.h>
#define BUFFER_SIZE BUFSIZ


static int counter = 0;

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
	int rankWithOr;
	bool orEd;
	int rCount;
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
//void calculateRanks(void* ranked){
	//int rank=0;
	//for(int i=0;i<20;i++){
		//	rank+=((rank_t *)ranked)->ranks[i];
//	}
//	((rank_t*)ranked)->rank=rank;
//}



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

void setR(void *r){
	rank_t *rank=(rank_t*)r;
	rank->rank = rank-> rankWithOr;
}

void setRZero(void *r){
	rank_t *rank = (rank_t*)r;
	printf("THE COUNTER%d\n", counter);
	if (rank->rCount < counter){
		rank -> rank = 0;
		rank -> rankWithOr  = 0;
	}
}

int main(int argc, char *argv[]) {
	//	char* loadFrom = (char*)malloc(100*sizeof(char));
	//char* fileName = (char*)malloc(100*sizeof(char));
	//if ((argc != 3) && (argc!=6)){
	//	printf("usage incorrect num");
	//	exit(EXIT_FAILURE);
	//}
	//if (argc == 6){
	//	if (strcmp(argv[3],"-q")){
	//		printf("usage 3 is not q");
	//		exit(EXIT_FAILURE);
	//	}
	//	else{
	//		strcpy(loadFrom,argv[5]);
	//		DIR* dir = opendir(loadFrom);
	//		if (!dir){
	//			printf("usage q directory");
	//			exit(EXIT_FAILURE);
	//		}
	//		strcpy(fileName,argv[4]);
	//	}
	//}
	//else
	//	{
	//		printf("%d",argc);
	//		printf("%s", argv[1]);
	//		char* crawlDir = (char*)malloc(100*sizeof(char));
	//		strcpy(crawlDir,argv[1]);
	//		DIR* dir = opendir(crawlDir);
	//		if (!dir){
	//			printf("usage non q directory");
	//			exit(EXIT_FAILURE);
	//		}
	//	strcpy(fileName,argv[2]);
	//		loadFrom = "../indexes";
	//	char executeCommand[200] = {0};
	//	sprintf(executeCommand,"../indexer/indexer ../%s ../indexes/%s", crawlDir, fileName);
	//		int status = system(executeCommand);
	//	if (status != 0){
	//			printf("cannot execute system command");
	//			exit(EXIT_FAILURE);
	//		}
	//	}
	int valid = 0;
	
	char result[100];
	char input[100];
	hashtable_t *words;
	hashtable_t *ranked;

	
	
	printf(">");
	fgets(input, 100, stdin);
	bool or = false;
		
	while((strcmp(input, "quit\n") !=0)){
		//int counter = 0;
		valid = 0;
		words =indexload("depth1","indexes"); // make sure to have "../" when calling in bash
		ranked=hopen(100);
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
			if(ispunct(current)!=0 || isdigit(current)!= 0){
				printf("[invalid query]\n");
				valid = 1;
				continue;
			}
			if(isalpha(current)!=0 || (isspace(current) != 0 && isspace(previous) == 0)){
				int len = strlen(result);
				result[len] = tolower(current);
				result[len + 1] = '\0';				
			}
		}
		if (strlen(input)==0){
			valid = 1;
		}
		
		if(valid == 0){
			char *word;
			char **searchArray = NULL;
			int n_spaces = 0;
			
			word=strtok(result, " ");
			
			while (word){
				searchArray = realloc(searchArray, sizeof(word) * ++n_spaces); //reallocate for each new word
				if (searchArray == NULL){
					exit(EXIT_FAILURE); //realloc fail
				}
				
				searchArray[n_spaces-1] = word;
				word = strtok(NULL, " ");
			}
			
		  searchArray = realloc(searchArray, sizeof(char*) * (n_spaces+1));
			searchArray[n_spaces] = 0;
			//int rankSize = 0;
			int k;
			for (k = 0; k < (n_spaces-1); k++){
				//		if ((strcmp(searchArray[k], "and")) && ((strcmp(searchArray[k],"or")))){
				//	counter +=1;
				//	}
				int z = k+1;
				if (!(strcmp(searchArray[k], "and")) || (!(strcmp(searchArray[k],"or")))){
					if (!(strcmp(searchArray[z], "and")) || (!(strcmp(searchArray[z],"or"))))
						{
							free(searchArray);
							hclose(ranked);
							happly(words,CIQ);
							hclose(words);
							exit(EXIT_FAILURE);
						}
				}
			}
			//			if ((strcmp(searchArray[n_spaces-1], "and")) && ((strcmp(searchArray[n_spaces-1],"or")))){
				//counter+=1;
			//	printf("this is counter %d \n", counter);
			///	}
			if ((strcmp(searchArray[0],"and")) && (strcmp(searchArray[0], "or")) && (strcmp(searchArray[n_spaces-1],"and")) && (strcmp(searchArray[n_spaces-1], "or"))){
					for (int l=0; l < n_spaces; l++){
						printf("split:%s\n",searchArray[l]);
						char *token = searchArray[l]; // remove = smth and use next line if error
						if(strcmp(token,"or")){
							printf("token is :%s ",token); 
							if((strlen(token)>2)){ // if our word is longer than 2 or is "or"
								counter+=1;
								word_t *w=(word_t*)hsearch(words,hWord,token,strlen(token)); // try and find it in our index
								if (w){ // if we find it
									if(strcmp(w->name,"and")){ // and it is not "and"
										doc_t *d=(doc_t*)qget(w->docs); // get its associated document queue
										while(d){  // while the doc exists
											int amount = d->occurences; // number of times word occurs in document
											rank_t *r=(rank_t *) hsearch(ranked, hRank, d->name, strlen(d->name)); // not fully clear at the moment.
											if (r){ // if this rand struct isn't null
												fflush(stdout); // why?
												r->rCount +=1;
												if (r->orEd){
													r -> rank = INT16_MAX;
													printf("SUCCES %d \n", r->rank);
													fflush(stdout);
													r ->orEd = false;											
												}
												if (( amount < (r->rank) )){ // if there are fewer occurences than the rank or !or

													printf("My status is %d and %d \n", r->rank, amount);
													fflush(stdout);
													r->rank = amount; // update the rank toa ammount     
												}
												//												printf("%d", strcmp(searchArray[l-1],"or"));
												printf("\n\ncounter %d rcount %d \n\n", counter, r->rCount);
												fflush(stdout);
												//	if (r->rCount < counter){
													//	r->rank = 0;
													//}
												
												if (!(searchArray[l+1])){
													//fflush(stdout);
													//if (!(strcmp(searchArray[l-1],"or"))){
													//		r->rankWithOr += amount;
													//		r->rank = r->rankWithOr;
													//	}
													//else{
													//if (r->rCount < counter){
														//	r->rank = 0;
														//}
													r->rankWithOr += r->rank;
													r->rank = r->rankWithOr;
														//	}
												}
																						
												else if (!(strcmp(searchArray[l+1],"or"))){
													//	if (r->rCount < counter){
													//	r->rank = 0;
													//	}
													r ->orEd = true;								 
													r->rankWithOr += r->rank;
													r->rank = r->rankWithOr;
													happly(ranked,setRZero);
													counter = 0;
												}
									
											}
											else{ // if the word isn't found
												rank_t *newRanked; // create a new rank_t
												newRanked = (rank_t *)malloc(sizeof(rank_t));
												chdir("../pages");
												FILE *f=fopen(d->name,"r"); // open some file
												fscanf(f,"%s",newRanked->url); 
												fclose(f);
												newRanked->rank = d->occurences; // update the current rank
												newRanked -> orEd = false;
												newRanked->rankWithOr = 0;
												strcpy(newRanked->id, d->name);
												newRanked -> rCount = 1;
												if ((searchArray[l+1])){
													if (!(strcmp(searchArray[l+1],"or"))){
														
														newRanked -> orEd = true;
														newRanked->rankWithOr += newRanked->rank;
														newRanked->rank = newRanked->rankWithOr;
														happly(ranked,setRZero);
														counter = 0;
													}
												}
												happly(ranked,setRZero);
												//	if (newRanked->rCount < counter){
												//	newRanked->rank = 0;
												//	}
												hput(ranked, newRanked, d->name, strlen(d->name));
											}
											free(d);
											d=(doc_t*)qget(w->docs);
										}
									}
								}
								//								else{
								//	happly(ranked,setR);
								happly(ranked, setRZero);
								//						}
							}
							
						}
					}
			}
			else{
				hclose(ranked);
				free(searchArray);
				happly(words,CIQ);
				hclose(words);
				exit(EXIT_FAILURE);
			}
			free(searchArray);
		}
		or=false;
		
		//printf("made it through");
		//happly(ranked,calculateRanks);
		happly(ranked, setRZero);
		happly(ranked,printR);
		hclose(ranked);
		//indexsave(words, "oof", "indexes");
		happly(words,CIQ);
		hclose(words);
		//printf("%d",strlen(input));
		memset(result, '\0', sizeof(char)*100);
		counter = 0;
		printf(">");
		fgets(input, 100, stdin);
		
		}
		

	
		exit(EXIT_SUCCESS);
	
}
