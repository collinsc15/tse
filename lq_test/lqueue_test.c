/* lqueue_test.c --- 
0;136;0c0;136;0c0;136;0c * 
 * 
 * Author: Vlado Vojdanovski
 * Created: Sat Nov 16 18:00:31 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include "../utils/lqueue.h"
#include "../utils/queue.h"
#include "../utils/queue.c"
#include "../utils/lqueue.c"
#include <pthread.h>

typedef struct ppl{                                                             
  int year;                                                                     
  //char name[10];                                                              
} ppl_t;  

void make5(void *ep)
{
	(*(int*)ep) = 5;
}

bool get5(void *p, const void *keyp)
{
	//	if (*((int*)p) == 5)
	//	{
			return true;
			//	}
			//	return false;
}

void *tfunc1(lqueue_t *lq)
{
	lqput(lq, (void*)5);
	return lq;
}

void *tfunc2(lqueue_t *lq)
{
	int  element = *(int*)(lqsearch(lq, get5, (const void*)3));
	printf("%d", element);
	return lq;
}

int main(void)
{
	pthread_t tid1, tid2;
	lqueue_t *lqOne = lqopen();
	int *a=(int *)malloc(sizeof(int));
	*a=1;
	int *b=(int *)malloc(sizeof(int));
	*b=2;
	int *c=(int *)malloc(sizeof(int));
	*c=3;
	lqput(lqOne, (void*)a);
	lqput(lqOne, (void*)b);
	lqput(lqOne, (void*)c);

	
	if (pthread_create(&tid1, NULL, tfunc1, lqOne)!=0)
		{
			exit(EXIT_FAILURE);
		}
	if (pthread_create(&tid2, NULL, tfunc2, lqOne)!=0)
		{
			exit(EXIT_FAILURE);
		}

	if (pthread_join(tid1, NULL) != 0)
		{
			exit(EXIT_FAILURE);
		}
	if (pthread_join(tid2, NULL) != 0)
		{
			exit(EXIT_FAILURE);
		}
	lqclose(lqOne);
	exit(EXIT_SUCCESS);
}
