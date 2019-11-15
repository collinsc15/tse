/* tests for locked queue module */

#include <stdlib.h>
#include <stdint.h>
#include "queue.h"
#include "lqueue.c"

void doubleMe (a){
	return 2*a;
}
int main(void){
	//test qopen
	lockedQ* q = lqopen();
	if(q){ printf("lqopen() works");}
	else{ exit(EXIT_FAILURE); }

	//test lqput and lqget
	lqput(q, 1);
	int testget = lqget(q);
	if(testget){ printf("lqput and lqget work");}
	else{ exit(EXIT_FAILURE); }

	//test lqapply
	lqput(q, 1);
	lqapply(q, doubleMe);
        int shouldB2 = lqget(q);
	if (shouldB2 == 2) { printf("lqapply works"); }
	else{ exit(EXIT_FAILURE); }

	//test lqclose
	lqclose(q);	
	if(!q){ printf("lqclose works!"); }
	else{ exit(EXIT_FAILURE); }
	`

}
