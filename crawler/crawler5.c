/* crawler.c --- crawls a website
1;5202;0c1;5202;0c1;5202;0c * 
 * 
 * Author: Claire C. Collins
 * Created: Fri Oct 18 13:52:36 2019 (-0400)
 * Edited by: Vlado Vojdanovski, Sat Oct 19
 * Version: 
 * 
 * Description: 
 * 
 */
#define _POSIX_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <webpage.h>
#include <queue.h> 
#include <hash.h>
#include <string.h>

int32_t pagesave(webpage_t *pagep, int id, char *dirname){
	int sizeh = webpage_getHTMLlen(pagep); // we grab the charsize of the html of our page
	int size = sizeh+300; // we add an extra 100 to store the other data we will be using
	FILE *f; // create a file object pointer
	char webpageData[size]; // the data we will be storing in our webpage file 

	free(webpage_getHTML(pagep));
	webpage_fetch(pagep); // self-evident
	char* url = webpage_getURL(pagep); // self-evident
	char* htmlCode = webpage_getHTML(pagep); // self-evident
	strcat(url,"\n"); //concat a newline to our url 
	
	// append everything to our character array
	sprintf(webpageData,"%s%d\n%d\n%s", url, webpage_getDepth(pagep), sizeh, htmlCode);
	
	// save address for the file
	char relSavePath[300] = {0};
	//EX: ../pages/1
	sprintf(relSavePath,"%s%s%s%d","../",dirname,"/",id);
	
	//if the file doesn't exist or the file is readable
	if((access(relSavePath, F_OK)!=0) || (access(relSavePath, W_OK)==0)){
		f=fopen(relSavePath,"w");  //opens/creates our file
		if ( f != NULL){ //if we sucessfully open a file
			fprintf(f,"%s",webpageData); // store our data
			fclose(f); //and close the file
		}
		else
			{
				return 1; //function exit failure
			}
	}
	else {
		return 1;
	}
	return 0;	// function exit success
}

// A simple helper function that checks whether our hash has a given URL
bool hashContainsURL(void* currPage, const void* searchAddress){
	char* currPageURL =(char*)currPage; 
	if (!strcmp(currPageURL,(char *)searchAddress)){ // if the strings are the same
		return true;
	}
	return false;
}

int main(void){
	char seed[40]="https://thayer.github.io/engs50/";   //url to be pointed at
	char *url=seed;//point at url
	webpage_t *w=webpage_new(seed, 0, NULL);    //creates webpage from url
	queue_t* qOfWebPages = qopen(); // opens a queue for internal pages
	hashtable_t*  hashOfPages = hopen(30);
	
	if(!webpage_fetch(w)){    //exits if unable to fetch webpage
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
			char* hashSearch = (char *)hsearch(hashOfPages,hashContainsURL,currPageURL,strlen(currPageURL)); //read in the result of the search
			if (!hashSearch){ // if we got a NULL pointer
					webpage_t* currPage=webpage_new(currPageURL, 0, NULL); // create our web page
					char currURL[80]; // and store the URL in a char array
					strcpy(currURL, currPageURL);
					hput(hashOfPages,currURL,currURL,strlen(currPageURL)); // add the charr array to our hash table
					qput(qOfWebPages, currPage); //put the site in the queue
				}
		}
		free(currPageURL); //always free the currPageUrl memory				
	}
	
	// while our queue is non-empty
	void* currGet;
	while ((currGet = qget(qOfWebPages)) != NULL){
    char*	poppedURL = webpage_getURL((webpage_t*)currGet);
		printf("%s\n",poppedURL); //print the URL of the popped page
		free(poppedURL); // free the url memory			
		free(currGet); //  free the entire webpage
	}
	char ph[] = "pages";
	pagesave(w,1,ph); // save the page of interest
	hclose(hashOfPages); // closes hash table
	qclose(qOfWebPages); // closes queue
	webpage_delete(w);         //deletes webpage
	
	exit(EXIT_SUCCESS);
}
