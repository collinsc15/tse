/* crawler.c --- 
 * 
 * 
 * Author: Claire C. Collins
 * Created: Fri Oct 18 13:52:36 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <webpage.h>

int main(void){
	char seed[40]="https://thayer.github.io/engs50/Labs";
	char *url=seed;
	webpage_t *w=webpage_new(seed, 0, NULL);
	if(!webpage_fetch(w)){
		exit(EXIT_FAILURE);
	}
	char *content=webpage_getHTML(w); 
	printf("%s", content);
	if (IsInternalURL(url)){
		printf("internal");
	}
	else {
		printf("external");
	}
	webpage_delete(w);
	exit(EXIT_SUCCESS);
}
