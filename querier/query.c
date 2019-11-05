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
bool hWord(void* hashWord, const void* searchWord){                                                                                                                        
  word_t *obj =(word_t*)hashWord;                                                                                                                                                     
  //strcpy(h,obj->name);                                                                                                                                                              
  //printf("objectname:%s\n",obj->name);                                                                                                                                              
  if (!strcmp(obj->name, (char*)searchWord)) {                                                                                                                                        
    return true;                                                                                                                                                                      
  }                                                                                                                                                                                   
  return false;                                                                                                                                                                       
}  
int main(int argc, const char **argv) {
	int valid = 0;
	char result[100];
	char input[100];
	hashtable_t *words;
	char *ws[100];
	printf(">");
	fgets(input, 100, stdin);
	//chdir("../indexes");
	words =indexload("depth0","indexes");
	
	while(strcmp(input, "quit\n") !=0){
		valid = 0;
		//clear new line 
		input[strlen(input) -1] = '\0';
		result[0]='\0';
		int i;
		for (i = 0; i<strlen(input); i++){
			char current = input[i];
			char previous = input[i-1];
			if(isalpha(current)!=0 || (isspace(current) != 0 && isspace(previous) == 0)){
				result[strlen(result)] = tolower(current);
				result[strlen(result) + 1] = '\0';
				
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
		
		if(valid == 0){
			char *word;
			word=strtok(result, " ");
			int rank=NULL;
			while(word!=NULL){
				char searched[50];
				memset(searched, '\0', sizeof(char)*50);
				if(strlen(word)>2){
					//printf("%s\n",word);
					//while(fscanf(f,"%s%*[^\n]",searched)==1){
					word_t *w=(word_t*)hsearch(words,hWord,word,strlen(word));
					if (w){
						//printf("found");
						if(strcmp(w->name,"and")!=0){
							doc_t *d=(doc_t*)qget(w->docs);
							if (d){
								int amount = d->occurences;
								printf("%s:%d ",word,amount);
								fflush(stdout);
								if ((rank==NULL) || (amount<rank)){
									rank=amount;
								}
							}
						}
					}
				}
				word=strtok(NULL," ");	
			}
			
			printf("-%d",rank);
		}
		
		//printf("%d",strlen(input));
		hclose(words);
		memset(result, '\0', sizeof(char)*100);
		printf(">");
		fgets(input, 100, stdin);
		
	}
	

	
	return 0;

}
