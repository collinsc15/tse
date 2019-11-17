/* lqueue_test.c --- 
 * 
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

void make5(void *ep)
{
	(*(int*)ep) = 5;
}

bool get5(void *p, const void *keyp)
{
	if ((*(int*)p) == 5)
		{
			return true;
		}
	return false;
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
	return (void*)((int*)element);
}

int main(void)
{
	pthread_t tid1, tid2;
	lqueue_t *lqOne = lqopen();
	lqput(lqOne, (void*)1);
	lqput(lqOne, (void*)2);
	lqput(lqOne, (void*)3);

	
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
	exit(EXIT_SUCCESS);
}
