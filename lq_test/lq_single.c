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
#include <lqueue.h>
#include <pthread.h>

typedef struct ppl{                                                             
  int year;                                                                     
  //char name[10];                                                              
} ppl_t;  

void make5(void *ep)
{
	((ppl_t*)ep)->year = 5;
	printf("L'age de homme est %d", ((ppl_t*)ep)->year);
}

bool get5(void *p, const void *keyp)
{
	ppl_t *g=(ppl_t*)p; 
	if (g->year== 5)
	{
			return true;
	}
	return false;
}

void *tfunc1(lqueue_t *lq)
{
	ppl_t *d=(ppl_t *)malloc(sizeof(ppl_t));
	d->year=5;
	lqput(lq, (void*)d);
	ppl_t *element = ((ppl_t*)lqsearch(lq, get5, (const void*)5));
	if (element){
		printf("%d", element->year);
	}
	lqapply(lq,make5);

	ppl_t *elementp = lqget(lq);
	if (elementp){
		printf("%d", elementp->year);
	}
	return lq;
}

void *tfunc2(lqueue_t *lq)
{
	ppl_t *element = ((ppl_t*)lqsearch(lq, get5, (const void*)5));
	if (element){
		printf("%d", element->year);
	}
	fflush(stdout);
	return lq;
}

int main(void)
{
	pthread_t tid1;
	pthread_t tid2;
	lqueue_t *lqOne = lqopen();
	ppl_t *a=(ppl_t *)malloc(sizeof(ppl_t));
	a->year=1;
	ppl_t *b=(ppl_t *)malloc(sizeof(ppl_t));
	b->year=2;
	ppl_t *c=(ppl_t *)malloc(sizeof(ppl_t));
	c->year=3;
	lqput(lqOne, (void*)a);
	lqput(lqOne, (void*)b);
	lqput(lqOne, (void*)c);

	
	if (pthread_create(&tid1, NULL, tfunc1, lqOne)!=0)
	{
		exit(EXIT_FAILURE);
	}
	//		if (pthread_create(&tid2, NULL, tfunc2, lqOne)!=0)
	//{
	//	exit(EXIT_FAILURE);
	//}

		if (pthread_join(tid1, NULL) != 0)
		{
			exit(EXIT_FAILURE);
		}
		//	if (pthread_join(tid2, NULL) != 0)
		//{
	//exit(EXIT_FAILURE);
	//}
	fflush(stdout);
	lqclose(lqOne);
	exit(EXIT_SUCCESS);
}
