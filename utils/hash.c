/*Claire Collins
 * hash.c -- implements a generic hash table as an indexed set of queues.
 *
 */
#include <stdint.h>
#include <queue.h>
#include <stdlib.h>
/* 
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 * 
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */
#define get16bits(d) (*((const uint16_t *) (d)))

typedef struct hashtable_t{
	uint32_t size;
	queue_t **table;
} hashtable_t;
	

static uint32_t SuperFastHash (const char *data,int len,uint32_t tablesize) {
  uint32_t hash = len, tmp;
  int rem;
  
  if (len <= 0 || data == NULL)
		return 0;
  rem = len & 3;
  len >>= 2;
  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }
  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (data);
    hash ^= hash << 16;
    hash ^= data[sizeof (uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += *data;
    hash ^= hash << 10;
    hash += hash >> 1;
  }
	//m  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;
  return hash % tablesize;
}

hashtable_t *hopen(uint32_t hsize){
	hashtable_t *tp = (hashtable_t*)malloc(sizeof(hashtable_t));
	queue_t **ap=(queue_t**) malloc(hsize*sizeof(queue_t*));

 	tp->size=hsize;
	tp->table=ap;
	//queue_t *p=tp->table;
	for(int i=0;i<hsize;i++)
		{
			ap[i]=qopen();
			//queue_t *q=NULL;
		}
	return tp;
}

void hclose(hashtable_t *htp){
	queue_t **tp = htp->table;
	for (int i=0;i<htp->size;i++){
		queue_t *p=tp[i];
		qclose(p);
		}
	free(htp->table);
	free(htp);
}

int32_t hput(hashtable_t *htp, void *ep, const char *key, int keylen){
	uint32_t p=SuperFastHash(key, keylen, htp->size);
	queue_t **tp=htp->table;
	qput(tp[p], ep);
	return 0;
}

void happly(hashtable_t *htp, void (*fn)(void* ep)){
	for(int i=0;i<htp->size;i++) {
		if(htp->table[i]!=NULL){
			qapply(htp->table[i], fn);
		}
	}
}

void *hsearch(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
	uint32_t p=SuperFastHash(key, keylen, htp->size);
	queue_t *q=htp->table[p];
	return qsearch(q, searchfn, key);
 	}

void *hremove(hashtable_t *htp,                                                
        bool (*searchfn)(void* elementp, const void* searchkeyp),              
        const char *key,                                                       
							int32_t keylen){
	uint32_t p=SuperFastHash(key, keylen, htp->size);
	return qremove(htp->table[p], searchfn, key);
}

