/* working_queue_test.c --- 
 * 
 * 
 * Author: Claire C. Collins
 * Created: Sat Nov 16 17:53:21 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "../utils/lqueue.h"
#include "../utils/lqueue.c"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void make5(void *ele){
	(*(int*))ele=5;
}
bool get5(void *p, const void *keyp){
	if ((*(int *)p) ==5){
		return true;
	}
	return false;
}

void function(void){
	lqueue_t *q=lqopen(30);
	lqput(q, 3);
	if (lqget(q)!=3){
		return 3;
	}
	lqput(q, 4);
	lqapply(q,make5);
	if (lqget(q)!=5){
		return 3;
	}
	lqclose(q);
}

int32_t main(void){
	pthread_t tid 1;

	if(pthread_create(&tid1,NULL,function,argp1)!=0){  
    exit(EXIT_FAILURE);
	}
	if (pthread_join(tid1,NULL)!=0){
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
