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
#define _POSIX_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <webpage.h>
#include <queue.h> 
#include <hash.h>
#include <string.h>

int32_t pagesave(webpage_t *pagep, int id, char *dirname){
	int sizeh= webpage_getHTMLlen(pagep);
	int size=sizeh+100;
	FILE *f;
	char data[size];
	char path[40];
	//	sprintf(path,"~/engs50/tse/%s",dirname);
	sprintf(path,"/%s",dirname);
	printf("path: %s",path);
	webpage_fetch(pagep);
	char* URL = webpage_getURL(pagep);
	char* forspaces = webpage_getHTML(pagep);
	strcat(URL,"\n");
	sprintf(data,"%s%d\n%d\n%s", URL, webpage_getDepth(pagep), sizeh, forspaces);
	printf("%s",data);
	if((access("~/engs50/tse/pages/1", F_OK)!=0) || (access("~/engs50/tse/pages/1", W_OK)==0)){
	//if((access(path, F_OK)!=0) || (access(path, W_OK)==0)){
		if (chdir("../pages")==0){           //changes directory to pages needs to be changed so it changes to dirname
			printf("directory changed");
		}
		else {
			printf("directory didn't change");
		}
		char snum[10];        //created for id
		sprintf(snum, "%d",id);   //turns id into a string
		f=fopen(snum,"w");          //opens file with id name
		fprintf(f,"%s",data);
		fclose(f);
		printf("closed");
	}
	else {
		printf("error file 1 cannot be written");
		return 1;
	}
	return 0;
	
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
	char ph[10];
	strcpy(ph, "pages");
	pagesave(w,1,ph);
	hclose(hashOfPages); // closes hash table
	qclose(qOfWebPages); // closes queue
	webpage_delete(w);         //deletes webpage
	exit(EXIT_SUCCESS);
}
