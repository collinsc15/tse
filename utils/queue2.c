#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>                                                           
#include <malloc.h>
#include  <stdbool.h>
#include "queue.h"                                                             


typedef struct car
{
	char plate[10];
	double price;
	int year;
}	car_t;


void doublePrice(void* elementp)
{
	((car_t*)elementp)->price*=2;
}

car_t *make_car( char *plate, double price, int year)
{
	car_t *cp;
	if(!(cp = (car_t*)malloc(sizeof(car_t))))
		{
			printf("[Error: malloc failed allocating car]\n");
			return NULL;
		}
	strcpy(cp->plate,plate);
	cp->price=price;
	cp->year=year;
	return cp;
}

int main(void)
{
	char plate[10];
	strcpy(plate,"123");
	car_t *c=make_car(plate, 19.0, 77);

	char plate1[10];
	strcpy(plate1,"1243");
	car_t *c3=make_car(plate, 29.0, 73);
	//
	char plate2[10];
	strcpy(plate2,"1123");
		car_t *c5=make_car(plate2, 39.0, 277);
	
	queue_t *queue1 = qopen();
		int32_t z = qput(queue1, (void*)c);
		int32_t z3 = qput(queue1, (void*)c3);
 	int32_t z5 = qput(queue1, (void*)c5);

 	qapply(queue1, doublePrice);
	
	strcpy(plate,"123");
	car_t *c7=make_car(plate, 19.0, 77);

	
	strcpy(plate1,"1243");
	car_t *c9=make_car(plate, 29.0, 73);

	
	strcpy(plate2,"1123");
	car_t *c11=make_car(plate2, 39.0, 277);

	queue_t *queue2 = qopen();
	//		int32_t z7 = qput(queue2, (void*)c7);
	//	int32_t z9 = qput(queue2, (void*)c9);
	//	int32_t z11 = qput(queue2, (void*)c11);
	
	qconcat(queue1, queue2);
	void* get = qget(queue1);
	if (get != NULL)
	{
		free(get);
		car_t *c2 = (car_t*)qget(queue1);
		printf("%lf", c2->price);
		printf("\n");
		free(c2);
		car_t *c4 = (car_t*)qget(queue1);
		printf("%lf", c4->price);
		printf("\n");
		free(c4);
		//	car_t *c6 = (car_t*)qget(queue1);
		//	printf("%lf", c6->price);
		//	printf("\n");
		//	free(c6);
		///	car_t *c8 = (car_t*)qget(queue1);
		///	printf("%lf", c8->price);
		///	printf("\n");
		///	free(c8);
		///car_t *c10 = (car_t*)qget(queue1);
		//printf("%lf", c10->price);
		//printf("\n");
		//free(c10);
		//		car_t *c12 = (car_t*)qget(queue1);
		//	printf("%lf", c12->price);
		//printf("\n");
		//	qclose(queue1);
	}
	free(c);
	free(c3);
	free(c5);
	free(c7);
	free(c9);
	free(c11);
	qclose(queue1);
	exit(EXIT_SUCCESS);
}
