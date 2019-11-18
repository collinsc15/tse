// tests for locked hash table module


#include <stdio.h>
#include <stdlib.h>
#include "lhash.h"

void doubleMe(void* ep){

	*(int *)ep = (2*(*(int *)ep));
}

bool getFirstInSearch(void* elementp, const void* searchkeyp){
	return true;

}
int main(void){

	//test hopen
	lockedhash_t* h = lhopen(100);	
	if (h){printf("hopen works\n"); }
	else{ exit(EXIT_FAILURE); }

	//test lhput
	int var = 20;
	int *ip = &var;
	const char* keyTest = "hash table test";
	int result = lhput(h, ip, keyTest, 2);
	if(result == 0){ printf("lhput works\n"); }

	//test lhsearch
	int* searchresult = lhsearch(h, getFirstInSearch, keyTest, 2);
	if(*searchresult == 20){ printf("lhsearch works\n"); }

	
	//test happly
	lhapply(h, doubleMe);
	searchresult = lhsearch(h, getFirstInSearch, keyTest, 2);
	if(*searchresult == 40){
		printf("lhapply works\n");
	}

/*
	//test lhclose
	lhclose(h);
	if(!h){ printf("hclose works!"); }
	else{ exit(EXIT_FAILURE); }
*/
	return 0; 
}
