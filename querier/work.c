
/*
0;136;0c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <hash.h>
#include <queue.h>
#include <indexio.h>
#include <dirent.h>
#include <unistd.h>
#define BUFFER_SIZE BUFSIZ

static int l=0;
static FILE *fOut;

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
	int scores[20];
	char id[20];
	char url[100];
} rank_t;

void CIQ(void* hashWord){
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

void fprintR(void *r){
	rank_t *rank=(rank_t*)r;
	if(rank->rank!=0){
		fprintf(fOut,"Rank:%d:DocID:%s:URL:%s\n",rank->rank,rank->id,rank->url);
		fflush(stdout);
	}
}

void printScores(void *r){
	rank_t *rank=(rank_t*)r;
	
	if (rank->rank > 0){
	printf("Rank:%d:DocID:%s:URL:%s\n",rank->rank,rank->id,rank->url);
	//while(i<20){
			//printf(":%d:",num);
	//i+=1;
	//num=rank->scores[i];
	//}
	fflush(stdout);
	}
}
void setRankOr(void *r){
	rank_t *rank=(rank_t*)r;
	rank->scores[l]=-1;
}

void setRankAnd(void *r){
	rank_t *rank=(rank_t*)r;
	rank->scores[l]=-2;
}
void lessThanThree(void *r){
	rank_t *rank=(rank_t*)r;
	rank->scores[l]=-3;
}

void calculate_score(int lastOr,int lengthOfSA, hashtable_t *htp, hashtable_t *ranked, char *word){
	if ((strlen(word)<3)){ // if our word is shorter than 3
		happly(ranked, lessThanThree);
	}
	else{
		word_t *w=(word_t*)hsearch(htp,hWord,word,strlen(word)); // try and find it in our index
		if (w){
			if(strcmp(w->name,"and")){ // and it is not "and"
				doc_t *d=(doc_t*)qget(w->docs); // get its associated document queue
				while(d){  // while the doc exists
					rank_t *r=(rank_t *) hsearch(ranked, hRank, d->name, strlen(d->name)); // not fully clear at the moment.
					//printf("\nCOUNTER: %d\n",counter);
					if (r){
						r->scores[l]=d->occurences;
					}
					else{
						rank_t *newRanked; // create a new rank_t
						newRanked = (rank_t *)malloc(sizeof(rank_t));
						chdir("../pages");
						FILE *f=fopen(d->name,"r"); // open some file
						fscanf(f,"%s",newRanked->url); 
						fclose(f);
						newRanked->rank=0;
						strcpy(newRanked->id, d->name);
						
						for(int x=0; x<20; x++){
							newRanked->scores[x]=0;
							if(x==lastOr){
								newRanked->scores[x]=-1;
							}
							if (x==(lengthOfSA)){
								newRanked->scores[x]=-20;
							}
							if(x==lastOr){
								newRanked->scores[x]=-1;
							}
						}
						newRanked->scores[l]=d->occurences;
						hput(ranked,newRanked, newRanked->id, strlen(newRanked->id));
					}
					free(d);
					d=(doc_t*)qget(w->docs); // get its associated document queue
				}
			}
			else{
				happly(ranked, setRankAnd);
			}
		}
	}
	if(!strcmp(word,"or")){
		happly(ranked, setRankOr);
	}
}
void freeStrings(char *loadFrom, char *fileName, char *queryFile, char *queryOut){
	free(loadFrom);
	free(fileName);
	free(queryFile);
	free(queryOut);
}

void calculateRank(void *r){
	//printf("calculate rank gets called");
	fflush(stdout);
	rank_t *rank=(rank_t*)r;
	int temps[20];
	int place=0;
	bool skip=false;
	temps[place]=rank->scores[0];
	for (int i=0; i<20; i++){
		int num=rank->scores[i];
		if (!skip){
			if ((num>0)&&(num<temps[place])){
				temps[place]=num;
			}
			else if (num==0){
				temps[place] = 0;
				skip=true;
			}
			if (num ==-1){
				place+=1;
				temps[place]=rank->scores[i+1];
			}
			if (num ==-20){
				temps[place+1]=-20;
				break;
			}
		}
		else {
			if (num ==-1){
				skip=false;
				place+=1;
				temps[place]=rank->scores[i+1];
			}
			if (num ==-20){
				temps[place+1]=-20;
				break;
			}
		}
	}
	int inTemp=0; 
	for(int i=0;inTemp!=-20;i++){
		inTemp=temps[i];
		rank->rank+=inTemp;
		inTemp=temps[i+1];
	}
}

int main(int argc, char *argv[]) {
	char* loadFrom = (char*)malloc(100*sizeof(char));
	char* fileName = (char*)malloc(100*sizeof(char));
	char* queryFile = (char*)malloc(100*sizeof(char));
	char* queryOut = (char*)malloc(100*sizeof(char));
	char qIn[100] = {0};
	char qOut[100] = {0};
	
	
	if ((argc != 3) && (argc!=6)){
		
		printf("usage incorrect num");
		fflush(stdout);
		freeStrings(loadFrom, fileName, queryFile, queryOut);
		exit(EXIT_FAILURE);
		}

	if (argc == 6){
		if (strcmp(argv[3],"-q")){
			freeStrings(loadFrom, fileName, queryFile, queryOut);
			printf("usage 3 is not q");
			fflush(stdout);
			exit(EXIT_FAILURE);
		}
		
			else{
		//sprintf(loadFrom,"%s%s","../",argv[5]);
		// DIR* dir = opendir(loadFrom);
		//  if (!dir){
		//  		printf("usage q directory");
		//			freeStrings(loadFrom, fileName, queryFile, queryOut);
		//			fflush(stdout);
		//			exit(EXIT_FAILURE);
		//	 }
		//	 closedir(dir);
			 sprintf(queryFile,"%s",argv[4]);
			 // sprintf(quer,"%s%s%s","../",loadFrom,argv[4]);
			 if ( access(queryFile, F_OK) == -1){
				 printf("usage: Invalid q file");
				 freeStrings(loadFrom, fileName, queryFile, queryOut);
				 fflush(stdout);
				 exit(EXIT_FAILURE);
			 }
			 	 sprintf(qOut,"%s",argv[4]);
		  }
  }
	
	printf("%d",argc);
	printf("%s", argv[1]);
	char* crawlDir = (char*)malloc(100*sizeof(char));
	sprintf(crawlDir,"%s%s", "../",argv[1]);
	DIR* dir = opendir(crawlDir);
	if (!dir){
		printf("usage non q directory");
		free(crawlDir);
		fflush(stdout);
		freeStrings(loadFrom, fileName, queryFile, queryOut);
		exit(EXIT_FAILURE);
	}
	closedir(dir);
	sprintf(crawlDir, "%s", argv[1]);
	sprintf(fileName,"%s",argv[2]);
	strcpy(loadFrom, "indexes");
	char executeCommand[200] = {0};
	sprintf(executeCommand,"../indexer/indexer %s %s", crawlDir, fileName);
	int status = system(executeCommand);
	free(crawlDir);
	if (status != 0){
		printf("cannot execute system command");
		freeStrings(loadFrom, fileName, queryFile, queryOut);
		fflush(stdout);
		exit(EXIT_FAILURE);
	}


	int valid = 0;
	
	char result[100];
	char input[100];
	hashtable_t *words;
	hashtable_t *ranked;

	char load[100] = {0};
	char dirLoad[100] = {0};

	strcpy(load, fileName);
	strcpy(dirLoad, loadFrom);

	free(loadFrom);
	free(fileName);
	strcpy(qIn, queryFile);
	strcpy(qOut, queryOut);
	free(queryFile);
	free(queryOut);
	if ( access(qIn, F_OK) == -1){
		printf("usage: Invalid qIn file");
		fflush(stdout);
		exit(EXIT_FAILURE);
		 }
	FILE *f;
	if (argc != 6){
	f = stdin;
	}
	else{
		f = fopen(qIn, "r");
		fOut = fopen(qOut, "w");
	}
	printf(">");
	fgets(input, 100, f);
	//bool or = false;
	
	while(!(feof(f))){
		if((words = indexload(load,dirLoad))){
			//int counter = 0;
			valid = 0;
		
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
						if (!(strcmp(searchArray[z], "and")) || (!(strcmp(searchArray[z],"or")))){
							//free(searchArray);
							//	hclose(ranked);
							//	happly(words,CIQ);
							//	hclose(words);
							//	exit(EXIT_FAILURE);
							printf("[invalid query]\n");
							continue;
						}
					}
				}
				//			if ((strcmp(searchArray[n_spaces-1], "and")) && ((strcmp(searchArray[n_spaces-1],"or")))){
				//counter+=1;
				//	printf("this is counter %d \n", counter);
				///	}
				if ((strcmp(searchArray[0],"and")) && (strcmp(searchArray[0], "or")) && (strcmp(searchArray[n_spaces-1],"and")) && (strcmp(searchArray[n_spaces-1], "or"))){
					int lastOr = -1;
					for (l=0; l < n_spaces; l++){
						char *word=searchArray[l];
						if (!strcmp(word,"or")){
							//printf("set last or");
							lastOr=l;
						}
						calculate_score(lastOr,n_spaces,words,ranked,word);
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
			
			happly(ranked,calculateRank);

			if (argc != 6){
				happly(ranked,printScores);
			}
			else{
				happly(ranked, fprintR);
			}
			hclose(ranked);
			//indexsave(words, "oof", "indexes");
			happly(words,CIQ);
			hclose(words);
			//printf("%d",strlen(input));
			memset(result, '\0', sizeof(char)*100);
			printf(">");
			fgets(input, 100, f);
			
		}
	}
	
	fclose(f);
	fclose(fOut);
	//free(loadFrom);

	exit(EXIT_SUCCESS);
	
}
