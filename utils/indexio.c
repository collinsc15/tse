/* indexio.c --- 
 * 
 * 
 * Author: Claire C. Collins
 * Created: Thu Oct 24 16:59:51 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
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
#include <indexio.h>

#define MAXCHAR 1000

int32_t indexsave(hash_t* index, char *indexnm, char *dirname)
{


}

index_t indexload(char *indexnm, char *dirname)
{
	char str[MAXCHAR];
	char word[50];
	char accessPath[150] = {0};
	hashtable_t* newHash;
 
	sprintf(accessPath,"%s%s",dirnmane, indexnm);
	FILE *f = fopen(accessPath, "r");
	if (f != NULL)
		{
			while((fgets(str, MAXCHAR, f)))
				{
					char *ptr = strtok(str, " ");
					hput(newHash, *ptr, *ptr, strlen(prt));
					while(ptr != NULL)
						{
							ptr = strtok(str, " ");
							
							
							
					
}
