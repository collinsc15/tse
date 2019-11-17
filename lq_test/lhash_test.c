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
#include <lhash.h>
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

void *tfunc1(lockedhash_t *lh)
{
	int *b=(int *)malloc(sizeof(int));
	*b=5;
	lhput(lh, (void *) b, "5", 1);
	return lh;
}

void *tfunc2 (lockedhash_t *lh)
{
	int element = (int*)(lhsearch(lh, get5, "5", 1));
	printf("%d", element);
	return (void*)((int*)element);
}

int main(void)
{
	pthread_t tid1, tid2;
	lockedhash_t *lh = lhopen(10);
	int *a=(int *)malloc(sizeof(int));
	int *c=(int *)malloc(sizeof(int));
	int *d=(int *)malloc(sizeof(int));
	*a=1;
	*c=3;
	*d=44;
	lhput(lh, (void *) a, "1", 1);
	lhput(lh, (void *) c, "3", 1);
	lhput(lh, (void *) d, "44", 2);
	
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
