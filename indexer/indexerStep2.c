#include <stdio.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>

void NormalizeWord(char word[]){
	char *p = word;
	
	//check that word has more than three characters
	//TODO: not working correctly
	int size = sizeof(word) / sizeof(word[0]);
	if(size < 4){
		while(*p){
			*p = "";
			p++;
		}
	}
	else{
	while (*p){
		if(!isalpha(*p)){
			*p = "";
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


	char* html = webpage_getHTML(w);
	
	while ((pos = webpage_getNextWord(w, pos, &result)) > 0) {
		NormalizeWord(result);
		printf("%s\n", result);	
		free(result);
	}
	
	webpage_delete(w);
	

	fflush(stdin);
	fflush(stdout);
	
	return 0;
	
}
