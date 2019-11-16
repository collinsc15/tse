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

#include <lqueue.h>
#include <lqueue.c>
#include <pthread.h>

void make5(void *ep)
{
	(int*)ep = 5;
}

bool get5(void *p, const void *keyp)
{
	if ((int*)p == 5)
		{
			return true;
		}
	return false;
}

void tfunc1(lqueue *lq)
{
	lqput(lq, 5);
}

void tfunc2(lqueue *lq)
{
	int element = lqsearch(lq, get5, 3);
}

void main()
{
	pthread_t tid1, tid2;
	lqueue_t *lqOne = lqopen();
	lqput(lqOne, 1);
	lqput(lqOne, 2);
	lqput(lqOne, 3);

	
	if (pthread_create(&tid1, NULL, tfunc1, lqOne)!=0)
		{
			exit(EXIT_FAILURE);
		}
	if (pthread_create(%tid2, NULL, tfunc2, lqOne)!=0)
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
}
