/* crawler.c --- crawls a website
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
	char seed[40]="https://thayer.github.io/engs50/";   //url to be pointed at
	char *url=seed;                                    //point at url
	webpage_t *w=webpage_new(seed, 0, NULL);    //creates webpage from url
	if(!webpage_fetch(w)){    //fetches web page if false exits
		exit(EXIT_FAILURE);
	}
	char *content=webpage_getHTML(w); //gets html of website
	printf("%s", content);       //prints content of website
	if (IsInternalURL(url)){    //scans if website is internal
		printf("internal");
	}
	else {
		printf("external");
	}
	webpage_delete(w);         //deletes webpage
	exit(EXIT_SUCCESS);
}
