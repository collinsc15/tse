/* indexio.c --- 
1;5202;0c * 
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
static FILE *f;


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

int32_t indexsave(hashtable_t* index, char *indexnm, char *dirname)
{                                                                                                      
  char fullSave[100] = {0};                                                     
  sprintf(fullSave,"../%s/%s",dirname,indexnm);    
	f = fopen(fullSave, "w");
	if (index)
		{ 
    happly(index, indexSprintf);
    fclose(f);                                                                                                                   
		}
	happly(index, closeInternalQueues); 
  hclose(index);
	return 0;
}

hashtable_t* indexload(char *indexnm, char *dirname)
{
	char str[MAXCHAR];
	char accessPath[150] = {0};
	hashtable_t* newHash = hopen(100);
 
	sprintf(accessPath,"../%s/%s",dirname, indexnm);
	FILE *f = fopen(accessPath, "r");
	if (f)
		{
			while((fgets(str, MAXCHAR, f)))
				{
					char *ptr = strtok(str, " ");

					word_t* newWord;    //creates word
					
          newWord=(word_t*) malloc(sizeof(word_t));  
          memset(newWord->name, 0,(50*sizeof(newWord->name[0])));                                                                       
          strcpy(newWord->name, ptr);     
					newWord -> docs = qopen();
					
					hput(newHash, (void*)newWord, (const char*)newWord, strlen(newWord->name));
					while(ptr != NULL)
						{
							doc_t* newDoc;                                                                                                           
							newDoc = (doc_t*)malloc(sizeof(doc_t));
							
							ptr = strtok(str, " ");
							strcpy(newDoc->name, ptr);
							
							ptr = strtok(str, " ");
							newDoc->occurences = (int)ptr;
							qput(newWord->docs,newDoc);
						}
				}
		}
			return newHash;
}


