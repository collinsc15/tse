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
#include <lhash.h>
#include <pthread.h>

typedef struct ppl{                                                             
  int year;                                                                     
  //char name[10];                                                              
} ppl_t;  

void make5(void *ep)
{
	((ppl_t*)ep)->year = 5;
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

void *tfunc1(lockedhash_t *lq)
{
	ppl_t *d=(ppl_t *)malloc(sizeof(ppl_t));
	d->year=5;
	lhput(lq, (void*)d, "5",1);
	//printf("success");
	fflush(stdout);
	return lq;
}

void *tfunc2(lockedhash_t *lq)
{
	ppl_t *element = ((ppl_t*)lhsearch(lq, get5, "5",1));
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
	lockedhash_t *lh = lhopen(10);
	ppl_t *a=(ppl_t *)malloc(sizeof(ppl_t));
	a->year=1;
	ppl_t *b=(ppl_t *)malloc(sizeof(ppl_t));
	b->year=2;
	ppl_t *c=(ppl_t *)malloc(sizeof(ppl_t));
	c->year=3;
	lhput(lh, (void*)a, "1", 1);
	lhput(lh, (void*)b, "2", 1);
	lhput(lh, (void*)c, "3",1);

	
	if (pthread_create(&tid1, NULL, tfunc1, lh)!=0)
	{
		exit(EXIT_FAILURE);
	}
		if (pthread_create(&tid2, NULL, tfunc2, lh)!=0)
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
	lhclose(lh);
	exit(EXIT_SUCCESS);
}
