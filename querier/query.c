#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE BUFSIZ

int main(int argc, const char **argv) {
	int valid = 0;
	char result[100];
	char input[100];
	printf(">");
	fgets(input, 100, stdin);
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
	//printf("%d",strlen(input));
	if(valid == 0){
		printf("%s\n", result);
	}
	memset(result, '\0', sizeof(char)*100);
	printf(">");
	fgets(input, 100, stdin);

	}
	


	return 0;

}
