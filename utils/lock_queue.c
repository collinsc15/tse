#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct queue_head{
	struct queue_head *next;
};

struct queue_r{
	struct queue_head *head;
	pthread_mutex_t head_lock; //initialize lock

	struct queue_head *tail;
	pthread_mutex_t tail_lock; //lock on tail

	struct queue_head divider; //ensure at least one item is queued

};

struct queue_root *make_root(){
	
	struct queue_root *root = malloc(sizeof(struct queue_root));
	pthread_mutex_init(&root->head_lock, NULL);
	pthread_mutex_init(&root->tail_lock, NULL);

	root->divider.next = NULL;
	root->head = &root->divider;
	root->tail = &root->divider;
	return root;

}

void queue_put(struct queue_head *new, struct queue_root *root){
	new->next = NULL;
	pthread_mutex_lock(&root->tail_lock);
	root->tail->next = new;
	root->tail = new;
	pthread_mutex_unlock(&root->tail_lock);
}

struct queue_head *queue_get(struct queue_root *root){
	struct queue_head *head, *next;

	while(1){
		pthread_mutex_lock(&root->head_lock);
		head = root->head;
		next = head->next;
		if(next == NULL){
			pthread_mutex_unlock(&root->head_lock);
			return NULL;
		}
		root->head = next;
		pthread_mutex_unlock(&root->head_lock);

		if(head == &root->divider){
			queue_put(head, root);
			continue;
		}

		return head;
	}

}
