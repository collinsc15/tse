#include <stdio.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>

void NormalizeWord(char word[]){
	//printf("normalizing");
	char *p = word;
	
	//check that word has more than three characters
	//TODO: not working correctly
	int size = strlen(word);
	char temp[size];
	if(size < 3){
		//while(*p){
			//*p = "";
			//p++;
		//}
		//printf("less than 3");
		//temp[]="";
		strcpy(word, "");
	}
	else{
	while (*p){
		//printf("%s\n",p);
		if(!isalpha(*p)){
			//*p = "";
			//printf("%s",p);
			strcpy(word, "");
			//word=NULL;
			break;
		}
			*p = tolower(*p);
			p++;
		
	}
	}
}

int main(void){
	webpage_t *w = pageload(1, "pages");
	webpage_fetch(w);
	
	//get next word
	int pos = 0;
	char *result; 
	
	
	//testing NormalizeWord
	char a[] = "TEST!test TESTING123";
	NormalizeWord(a);
	printf("%s", a);

	chdir("../forIndexer");
	char* html = webpage_getHTML(w);
	FILE *f=fopen("1","a");
	while ((pos = webpage_getNextWord(w, pos, &result)) > 0) {
		NormalizeWord(result);
		
			
		if(strcmp(result,"")){
			fprintf(f, "%s\n", result);
			}
		free(result);
	}
	
	webpage_delete(w);
	fclose(f);

	fflush(stdin);
	fflush(stdout);
	
	return 0;
	
}