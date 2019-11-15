// tests for locked hash table module

#include "locked-hash.h"

int main(void){

	//test hopen
	lockedhast_t* h = lhopen(100);	
	if (h){printf("hopen works"); }
	else{ exit(EXIT_FAILURE); }

	//test lhput
	int var = 20;
	int *ip = &var;
	int result = lhput(h, ip, ip, 2);
	if(result == 0){ printf("lhput works"); }
	else{ exit(EXIT_FAILURE); }

	//test lhclose
	lhclose(h);
	if(!h){ printf("hclose works!"); }
	else{ exit(EXIT_FAILURE); }

	return 0; 
}
