/* locked-hash.c --- 
 * 
 * 
 * Author: Claire C. Collins
 * Created: Thu Nov 14 18:16:41 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <hash.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct lockedhash_t{
	hashtable_t *hash;
	pthread_mutex_t lock;
} lockedhash_t;

lockedhash_t *lhopen(int32_t hsize){
	lockedhash_t *lhtp= (lockedhash_t *)malloc(sizeof(lockedhash_t));
	if (!(lhtp)){
		return NULL;
	}
	lhtp->hash=hopen(hsize);
	pthread_mutex_init(&(lhtp->lock),NULL);
	return lhtp;
}

void lhclose(lockedhash_t *lhtp){
	pthread_mutex_lock(&(lhtp->lock));
	hclose(lhtp->hash);
	//pthread_mutex_unlock(&(lhtp->lock));
	pthread_mutex_destroy(&(lhtp->lock));
	free(lhtp);
	
}

int32_t lhput(lockedhash_t *lhtp, void *ep, const char *key, int keylen){
	pthread_mutex_lock(&(lhtp->lock));
	hput(lhtp->hash, ep, key, keylen);
	pthread_mutex_unlock(&(lhtp->lock));
	return 0;
}

void lhapply(lockedhash_t *lhtp, void (*fn)(void* ep)){
	pthread_mutex_lock(&(lhtp->lock));
	happly(lhtp->hash, fn);
	pthread_mutex_unlock(&(lhtp->lock));
}
void *lhsearch(lockedhash_t *lhtp,                                                                                                  
        bool (*searchfn)(void* elementp, const void* searchkeyp),                                                                
        const char *key,                                                                                                         
							int32_t keylen){
	pthread_mutex_lock(&(lhtp->lock));
	void *ele=hsearch(lhtp->hash, searchfn, key, keylen);
	pthread_mutex_unlock(&(lhtp->lock));
	return ele;
}

void *lhremove(lockedhash_t *lhtp,                                                                                                  
							bool (*searchfn)(void* elementp, const void* searchkeyp),                                                                
							const char *key,                                                                                                         
							int32_t keylen){
	pthread_mutex_lock(&(lhtp->lock));
	void *ele=hremove(lhtp->hash, searchfn, key, keylen);
	pthread_mutex_unlock(&(lhtp->lock));
	return ele;
}
