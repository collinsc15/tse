/* lqueue.c --- 
 * 
 * 
 * Author: Vlado Vojdanovski
 * Created: Thu Nov 14 18:21:25 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <queue.h>
#include <lqueue.h>

typedef locked_queue
{
	queue_t* queue;
	pthread_mutex_t lock;
} lockedQ;
	
void lqopen(void)
{
	lockedQ* newLockedQ = malloc(sizeof(lockedQ));
	newLockedQ -> queue = qopen();
	pthread_mutex_init(newLockedQ->lock, NULL); 
}

void lqclose(lockedQ *qp)
{
	pthread_mutex_lock(qp -> lock);
	sleep(10);
	qclose(qp -> queue);
	pthread_mutex_unlock(qp -> lock);
	pthread_mutex_destroy(qp -> lock);
	free(qp);
}

int32_t lqput(lockedQ *qp, void *elementp)
{
	pthread_mutex_lock(qp -> lock);
	sleep(10);
	qput(qp -> queue, elementp);
	pthread_mutex_unlock(qp -> unlock);
}

void *lqget(lockedQ *qp)
{
	pthread_mutex_lock(qp -> lock);
	sleep(10);
	void* element = qpget(qp -> queue);
	pthread_mutex_unlock(qp -> unlock);
	return element;
}

void lqapply(lockedQ *qp, void(*fn)(void* elementp))
{
	pthread_mutex_lock(qp -> lock);
	sleep(10);
	qapply(qp->queue, void(*fn)(void* elementp));
	pthread_mutex_unlock(qp ->unlock);
}

void* lqsearch(lockedQ *qp, bool(*searchfn)(void* elementp. const void* keyp), const void* skeyp)
{
	pthread_mutex_lock(qp->lock);
	sleep(10);
	void* element = qsearch(qp -> queue, bool(*searchfn)(void* elementp. const void* keyp), const void* skeyp);
	pthread_mutex_unlock(qp -> unlock);
	return element;
}

//void* lqremove(locked *qp, bool(*searchfn)(void* elementp, const void* keyp),
//							const void* skeyp)
//{
//	pthread_mutex_lock(qp -> lock);
//	void* element = qremove(qp->queue,bool(*searchfn)(void* elementp, const void* keyp),
//													const void* skeyp);
//	pthread_mutex_unlock(qp -> lock);
//	return element;
//}

