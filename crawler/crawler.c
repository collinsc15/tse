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

// A simple helper function that checks whether our hash has a URL
int hashContainsURL(void* currPage, const void* searchAddress){
	char* currPageURL = webpage_getURL((webpage_t*)currPage);
	if (*currPageURL != *(char *)searchAddress){
		return false;
	}
	return true;
}

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
	char *currPageURL; // charr array that stores the URl we are looking at


	// while there are URLs to be read
	while((pos = webpage_getNextURL(w, pos, &currPageURL)) > 0 ) {
		if (IsInternalURL(currPageURL)){    //if the url is internal
			//	if hsearch(hashOfTables,hashContainsURL
			webpage_t* currPage=webpage_new(currPageURL, 0, NULL); //our webpage
			qput(qOfWebPages, currPage); //put it in our queue
		  free(currPageURL);
		}
	}
	free(currPageURL);


	// while our queue is non-empty
	void* currGet;
	while ((currGet = qget(qOfWebPages)) != NULL){
    char*	poppedURL = webpage_getURL((webpage_t*)currGet);
		printf("%s\n",poppedURL); //print the URL of the popped page
		free(poppedURL); // free the url memory			
		free(currGet); //  free the entire webpage
	}
	
	qclose(qOfWebPages);
	webpage_delete(w);         //deletes webpage
	exit(EXIT_SUCCESS);
}
