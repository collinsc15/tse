/* indexio.c --- 
 * 
 * 
 * Author: Vlado Vojdanovski
 * Created: Thu Oct 24 16:59:51 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <webpage.h>
#include <queue.h> 
#include <hash.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <indexio.h>

#define MAXCHAR 1000



typedef struct doc
{
  char name[50];   
  int occurences; 
} doc_t;

typedef struct word
{   
  char name[50]; 
  queue_t *docs;                                                               
} word_t;

bool hashContainsWord(void* hashWord, const void* searchWord)
{  
  word_t *obj =(word_t*)hashWord;
  if (!strcmp(obj->name, (char*)searchWord))
		{
			return true;  
		}  
  return false;   
}

void closeInternalQueues(void* hashWord)
{    
  qclose(((word_t*)hashWord)->docs);
}

void docSprintf(void *doc){
  fprintf(f," %s",((doc_t*)doc)->name);
  fprintf(f," %d ",((doc_t*)doc)->occurences);  
}

void indexSprintf(void* hashWord)
{ 
  fprintf(f,"%s",((word_t*)hashWord)->name);
  qapply(((word_t*)hashWord)->docs, docSprintf);
  fprintf(f,"\n"); 
} 

int32_t indexsave(hash_t* index, char *indexnm, char *dirname)
{                                                                                                      
  char fullSave[100] = {0};                                                     
  sprintf(fullSave,"../%s/%s",dirname,indexnm);    
	FILE *f = fopen(fullSave, "w");
	if (index)
		{ 
    happly(wordHash, indexSprintf);
    fclose(hashFile);                                                                                                                   
		}
	happly(wordHash, closeInternalQueues); 
  hclose(wordHash);
	return 0;
}

index_t indexload(char *indexnm, char *dirname)
{
	char str[MAXCHAR];
	char word[50];
	char accessPath[150] = {0};
	hashtable_t* newHash;
 
	sprintf(accessPath,"../%s/%s",dirnmane, indexnm);
	FILE *f = fopen(accessPath, "r");
	if (f != NULL)
		{
			while((fgets(str, MAXCHAR, f)))
				{
					char *ptr = strtok(str, " ");

					word_t* newWord;    //creates word
					
          newWord=(word_t*) malloc(sizeof(word_t));  
          memset(newWord->name, 0,(50*sizeof(newWord->name[0])));                                                                       
          strcpy(newWord->name, ptr);     
					newWord -> doc = qopen();
					
					hput(newHash, newWord, newWord, strlen(newWord);
					while(ptr != NULL)
						{
							doc_t *newDoc;                                                                                                           
							newDoc = (doc_t*)malloc(sizeof(doc_t));
							
							ptr = strtok(str, " ");
							strcpy(newDoc->name, ptr);
							
							ptr = strtok(str, " ");
							strcpy(newDoc->occurences, ptr);
							qput(newWord->docs,newDoc);
						}
				}
		}
			return newHash;
}


