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
#include "queue.h"
#include "lqueue.h"

typedef struct locked_queue
{
	queue_t* queue;
	pthread_mutex_t lock;
} lockedQ;
	
void* lqopen(void)
{
	lockedQ* newLockedQ = malloc(sizeof(lockedQ));
	(newLockedQ -> queue) = qopen();
	pthread_mutex_init((newLockedQ->lock), NULL); 
}

void lqclose(void *qp)
{
	pthread_mutex_lock((((lockedQ*)qp) -> lock));
	sleep(10);
	qclose((((lockedQ*)qp) -> queue));
	pthread_mutex_unlock((((lockedQ*)qp) -> lock));
	pthread_mutex_destroy((((lockedQ*)qp) -> lock));
	free(((lockedQ*)qp));
}

int32_t lqput(void *qp, void *elementp)
{
	pthread_mutex_lock((qp -> lock));
	sleep(10);
	qput((qp -> queue), elementp);
	pthread_mutex_unlock((qp -> lock));
}

void *lqget(void *qp)
{
	pthread_mutex_lock((((lockedQ*)qp) -> lock));
	sleep(10);
	void* element = qget((((lockedQ*)qp) -> queue));
	pthread_mutex_unlock((((lockedQ*)qp) -> lock));
	return element;
}

void lqapply(void *qp, void(*fn)(void* elementp))
{
	pthread_mutex_lock((((lockedQ*)qp) -> lock));
	sleep(10);
	qapply((((lockedQ*)qp)->queue), void(*fn)(void* elementp));
	pthread_mutex_unlock((((lockedQ*)qp) ->lock));
}

void* lqsearch(void *qp, bool(*searchfn)(void* elementp. const void* keyp), const void* skeyp)
{
	pthread_mutex_lock((((lockedQ*)qp)->lock));
	sleep(10);
	void* element = qsearch((((lockedQ*)qp) -> queue), bool(*searchfn)(void* elementp. const void* keyp), const void* skeyp);
	pthread_mutex_unlock((((lockedQ*)qp) -> lock));
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

