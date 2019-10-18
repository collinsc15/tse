/* test1.c --- test has open and close
 * 
 * 
 * Author: Claire C. Collins
 * Created: Sun Oct 13 16:42:07 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <hash.h>
#include <queue.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ppl{
	int year;
	//char name[10]; 
} ppl_t;

ppl_t *make(int year){
	ppl_t *p;
	if (!(p=(ppl_t*) malloc(sizeof(ppl_t)))) {
			printf("Problem allocating memory");
				return NULL;
		}
	p->year=year;
		return p;
}

bool findYear(void* elementp, const void* keyp){
	printf("%d", *(int *)keyp);
	printf("\n");
	printf("%d", ((ppl_t*)elementp)->year);
  printf("\n");
	fflush(stdout);
  if (((ppl_t*)elementp)->year == *(int *)keyp)
    {
      //      printf("found match at %d", ((car_t*)elementp)->year);
      return true;
    }
	else
		{
			return false;
		}
}
void mak(void* ele){       //turns all ppl year to 80
	((ppl_t*)ele)->year=80;
}
bool is33(void* ele, const void* keyp){
	if (((ppl_t*)ele)->year==33){
		return true;
	}
	else {
		return false;
	}
}

int main(void){
	hashtable_t *hash = hopen(2);
	hashtable_t *hash2 = hopen(7);
 	ppl_t *g=make(33);
	ppl_t *y=make(37);
	if(hput(hash,(void*) g, "whats", 5)!=0){    //puts to empty hash
	exit(EXIT_FAILURE);
	}
	if(hput(hash, (void*) y, "whats", 5)!=0){   //puts to nonempty hash
		exit(EXIT_FAILURE);
	}
	//ppl_t *result= (ppl_t*)
	hsearch(hash2, findYear, "whats", 5);                //searches filled hash table
	if(hsearch(hash, findYear, "whats", 5)!=NULL){       //searches empty hashtable
		exit(EXIT_FAILURE);
	}
	
	if(hremove(hash, is33, "whats", 5)!=g){ //removes from valid key
		exit(EXIT_FAILURE);
	}
	
	if(hremove(hash, is33, "whats", 5)!=NULL){ //no func exists
			exit(EXIT_FAILURE);
	}
	happly(hash, mak);
	if(y->year!=80){       //tests happly
		exit(EXIT_FAILURE);
	}
	
	
	hclose(hash);
	hclose(hash2);
	free(g);
	free(y);
	exit(EXIT_SUCCESS);
	return 0;
}
