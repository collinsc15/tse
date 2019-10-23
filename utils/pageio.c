/* pageio.c --- 
 * 
 * 
 * Author: Claire C. Collins
 * Created: Wed Oct 23 12:41:27 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <webpage.h>
#include <string.h>

int32_t pagesave(webpage_t *pagep, long int id, char *dirname){
	
	//	free(webpage_getHTML(pagep));
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
	//	
	//strcat(url,"\n"); //concat a newline to our url 
	
	// append everything to our character array
	snprintf(webpageData,size,"%s\n%d\n%d\n%s", url, webpage_getDepth(pagep), sizeh, htmlCode);
	//free(htmlCode);
	// save address for the file
	char relSavePath[300] = {0};
	//EX: ../pages/1
	//	printf("%s", relSavePath);
	//	fflush(stdout);
	//char dir[50];
	snprintf(relSavePath,300,"%s%s%s%ld","../",dirname,"/",id);
	//snprintf(dir,50, "../%s",dirname);

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
			//			fclose(f); //and close the file
			return 1; //function exit failure
		}

	}
	else {
		return 1;
	}
	//free(htmlCode);
	return 0;	// function exit success
}
