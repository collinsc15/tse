/* crawler.c --- crawls a website
 * Author: Claire C. Collins
 * Created: Fri Oct 18 13:52:36 2019 (-0400)
 * Edited by: Vlado Vojdanovski, Sat Oct 19
 * Description:  The crawler has the following usage:
 * crawler <seedurl> <pagedir> <max depth> where:
        • seed url is the starting url where the crawl begins,
	• pagedir is where the saved pages are stored, and 
	• maxdepth is the  maximum  depth that the  crawler may crawl;  it should  be  non-negative.
 * 
 */
#define _POSIX_SOURCE
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


int32_t pagesave(webpage_t *pagep, long int id, char *dirname){
	webpage_fetch(pagep); // self-evident
	char* url = webpage_getURL(pagep); // self-evident
	char* htmlCode = webpage_getHTML(pagep); // self-evident
 
	int sizeh = 0;
	sizeh = webpage_getHTMLlen(pagep); // we grab the charsize of the html of our page
	int size;
  	size = sizeh+300; // we add an extra 100 to store the other data we will be using
	FILE *f; // create a file object pointer
	char webpageData[size]; // the data we will be storing in our webpage file 
	//struct stat dirn;
	memset(webpageData, 0, size*sizeof(char));
	
	// append everything to our character array
	snprintf(webpageData,size,"%s\n%d\n%d\n%s", url, webpage_getDepth(pagep), sizeh, htmlCode);

	// save address for the file
	char relSavePath[300] = {0};
	//EX: ../pages/1
	
	snprintf(relSavePath,300,"%s%s%s%ld","../",dirname,"/",id);

	//if the file doesn't exist or the file is readable
	if((access(relSavePath, F_OK)!=0) || (access(relSavePath, W_OK)==0)){
		f=fopen(relSavePath,"w");  //opens/creates our file
		if ( f != NULL) { //if we sucessfully open a file
			fprintf(f,"%s",webpageData); // store our data
			printf("%s", relSavePath);
			fflush(stdout);
			fclose(f); //and close the file
		}
		
		else {
			webpage_delete(pagep);
			return 1; //function exit failure
		}

	}
	else {
		webpage_delete(pagep);
		return 1;
	}
	return 0;	// function exit success
}

// A simple helper function that checks whether our hash has a given URL
bool hashContainsURL(void* currPage, const void* searchAddress){
	char* curr =(char*)currPage;
 	if (strcmp((char *)searchAddress,curr) == 0){ // if the strings are the same
			return true;
	}
	return false;
}

int main(int argc, char* argv[]){
	if (argc !=4){
		printf("crawler <seedurl> <pagedir> <maxdepth>");
		return 6;
	}
	char* seed = (char*) malloc((70)*sizeof(char));
	strcpy(seed, argv[1]);   //url to be pointed at
	
	webpage_t *w=webpage_new(seed, 0, NULL);    //creates webpage from url
	queue_t* qOfWebPages = qopen(); // opens a queue for internal pages
	hashtable_t*  hashOfPages = hopen(1000);
	
	if(!webpage_fetch(w)){    //exits if unable to fetch webpage
		qclose(qOfWebPages);
		hclose(hashOfPages);
		exit(EXIT_FAILURE);
	}

	char *content=webpage_getHTML(w); //gets html of website
	free(content);
	if (IsInternalURL(seed)){    //scans if website is internal
		hput(hashOfPages,seed,seed,strlen(seed)); // add the charr array to our hash table
		qput(qOfWebPages, w); //put the initial page in the queue
	}
	else {
		printf("external");
	}

	char *strPtr;
	char id[100] = {0};
	strcpy(id, argv[3]);
	long int maxIter = strtol(id, &strPtr, 10);
	long int curr_id = 1;
	if (maxIter<0){
		printf("depth cannot be negative");
		return 3;
	}
	char ph[50] = {0};
	strcpy(ph,argv[2]);
  char dirCheck[100] = "../";
	strcat(dirCheck,ph);
	DIR* dir = opendir(dirCheck);
	if (dir) {
    closedir(dir);
	}
	else{
		webpage_delete(w);
		hclose(hashOfPages); // closes hash table
		qclose(qOfWebPages); // closes queue			
    printf("Directory does not exist");
		exit(EXIT_FAILURE);
	}

	
	while((w = qget(qOfWebPages)))
		{
			if(webpage_fetch(w)){
			// initiating search of all internal hyperlinks
			char *html = webpage_getHTML(w);
			int pos = 0;
			char *currPageURL; // charr array that stores the URl we are looking at
			// while there are URLs to be read
			while((pos = webpage_getNextURL(w, pos, &currPageURL)) > 0 ) {
				if (IsInternalURL(currPageURL)){    //if the url is internal
					void* hashSearch = hsearch(hashOfPages,hashContainsURL,currPageURL, strlen(currPageURL)); //read in the result of the search
					if ((!hashSearch) && (webpage_getDepth(w) < maxIter)){ // if we got a NULL pointer
						webpage_t* currPage=webpage_new(currPageURL,(webpage_getDepth(w)+1), NULL); // create our web page
						hput(hashOfPages,currPageURL,currPageURL,strlen(currPageURL)); // add the charr array to our hash table
						qput(qOfWebPages, currPage); //put the site in the queue
					}
					else{
						free(currPageURL);
					}
				}
				else{
					free(currPageURL);
				}				
			}
		 	free(html);
			pagesave(w,curr_id,ph);	// save the page of interest
			webpage_delete(w); //deletes webpage
			curr_id += 1;
			}
		}
			hclose(hashOfPages); // closes hash table
			qclose(qOfWebPages); // closes queue
			exit(EXIT_SUCCESS);	
}
