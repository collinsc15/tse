/* crawler.c --- crawls a website
1;5202;0c * 
 * 
 * Author: Claire C. Collins
 * Created: Fri Oct 18 13:52:36 2019 (-0400)
 * Edited by: Vlado Vojdanovski, Sat Oct 19
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <webpage.h>
#include <queue.h> 
#include <hash.h>

int main(void){
	char seed[40]="https://thayer.github.io/engs50/";   //url to be pointed at
	char *url=seed;//point at url
	webpage_t *w=webpage_new(seed, 0, NULL);    //creates webpage from url
	queue_t* qOfWebPages = qopen(); // opens a queue for internal pages
	//hashtable_t*  hashOfPages = hopen();
	
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

	// initiating search of all internal hyperlinks
	int pos = 0;
	char *currPage; // charr array that stores the URl we are looking at


	// while there are URLs to be read
	while((pos = webpage_getNextURL(w, pos, &currPage)) > 0 ) {
		if (IsInternalURL(currPage)){    //if the url is internal
			qput(qOfWebPages, currPage); //put it in our queue
		}
	}
	free(currPage);

	// while our queue is non-empty
	void* currGet;
	while ((currGet = qget(qOfWebPages)) != NULL){
		printf("%s\n",(char*)currGet); //print what it returns
		free(currGet);
	}
	
	qclose(qOfWebPages);
	webpage_delete(w);         //deletes webpage
	exit(EXIT_SUCCESS);
}
