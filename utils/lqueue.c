/* lqueue.c --- 
0;136;0c * 
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
#include <unistd.h>

typedef struct locked_queue
{
	queue_t* queue;
	pthread_mutex_t lock;
} lockedQ;
	
void* lqopen(void)
{
	lockedQ* newLockedQ = (lockedQ*)malloc(sizeof(lockedQ));
	if(!(newLockedQ))
		{
			return NULL;
		}
	(newLockedQ -> queue) = qopen();
	pthread_mutex_init(&(newLockedQ->lock), NULL);
	return (void*)newLockedQ;
}

void lqclose(void *qp)
{
	pthread_mutex_lock(&(((lockedQ*)qp) -> lock));
	
	qclose((((lockedQ*)qp)->queue));
	//	sleep(2);
	pthread_mutex_unlock(&(((lockedQ*)qp) -> lock));
	pthread_mutex_destroy(&(((lockedQ*)qp) -> lock));
	free(((lockedQ*)qp));
}

int32_t lqput(void *qp, void *elementp)
{
	pthread_mutex_lock(&(((lockedQ*)qp) -> lock));
	if(!qput((((lockedQ*)qp) -> queue), elementp))
		{
			pthread_mutex_unlock(&(((lockedQ*)qp) -> lock));
			return 1;
		}
	//sleep(2);
	pthread_mutex_unlock(&(((lockedQ*)qp) -> lock));
	return 0;
}

void *lqget(void *qp)
{
	pthread_mutex_lock(&(((lockedQ*)qp) -> lock));
	void* element = qget(&(((lockedQ*)qp) -> queue));
	//sleep(2);
	pthread_mutex_unlock(&(((lockedQ*)qp) -> lock));
	return element;
}

void lqapply(void *qp, void(*fn)(void* elementp))
{
	pthread_mutex_lock(&(((lockedQ*)qp) -> lock));
	
	qapply((((lockedQ*)qp)->queue), fn);
	//sleep(2);
	pthread_mutex_unlock(&(((lockedQ*)qp) ->lock));
}

void* lqsearch(void *qp, bool(*searchfn)(void* elementp, const void* keyp), const void* skeyp)
{
	pthread_mutex_lock(&(((lockedQ*)qp) -> lock));
	void* element = qsearch((((lockedQ*)qp) -> queue),searchfn, skeyp);
	//sleep(2);
	
	pthread_mutex_unlock(&(((lockedQ*)qp) -> lock));
	return element;
}

void *lqadd(void *qp, bool(*searchfn)(void* elementp, const void* keyp), const void* skeyp, void *initElement) {
	pthread_mutex_lock(&(((lockedQ*)qp) -> lock));
	void* element = qsearch((((lockedQ*)qp) -> queue),searchfn, skeyp);
	//sleep(2);
	void *stored=initElement;
	if(element){
		free(initElement);
		pthread_mutex_unlock(&(((lockedQ*)qp) -> lock));
		return element;
	}
	//printf("%s",initElement);
	//fflush(stdout);
	qput(((lockedQ*)qp)->queue, initElement);
	pthread_mutex_unlock(&(((lockedQ*)qp) -> lock));
	return stored;
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

