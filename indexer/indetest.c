/* indetest.c --- 
 * 
 * 
 * Author: Vlado Vojdanovski
 * Created: Wed Oct 30 13:42:56 2019 (-0400)
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
#include <webpage.h> 
#include <indexio.h>

int32_t main(void)
{
  hashtable_t *w= indexload("indexes", "indexes");  
	indexsave(w,"BillClinton2", "indexes");
	exit(EXIT_SUCCESS);
}    
