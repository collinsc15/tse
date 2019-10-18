/* queue1.c --- 
 * 
 * 
 * Author: Vlado Vojdanovski
 * Created: Sat Oct 12 15:51:47 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */


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

void doublePrice(void* elementp)
{
	((car_t*)elementp)->price*=2;
}

bool findYear(void* elementp, const void* keyp)
{
	printf("%d", *(int *)keyp);
	printf("\n");
	printf("%d", ((car_t*)elementp)->year);
	printf("\n");
	fflush(stdout);
  if (((car_t*)elementp)->year == *(int *)keyp)
		{
			//			printf("found match at %d", ((car_t*)elementp)->year); 
			return true;
		}
	else
		{
			return false;
		}														 
}

int main(void)
{
	char plate[10];
	strcpy(plate,"123");
	car_t *c=make_car(plate, 19.0, 77);

	char plate1[10];
	strcpy(plate1,"1243");
	car_t *c3=make_car(plate1, 29.0, 73);

	char plate2[10];
	strcpy(plate2,"1123");
	car_t *c5=make_car(plate2, 39.0, 277);
	
	queue_t *queue1 = qopen();
	int32_t z = qput(queue1, (void*)c);
	int32_t z3 = qput(queue1, (void*)c3);
	int32_t z5 = qput(queue1, (void*)c5);

	qapply(queue1, doublePrice);


	//	printf("Size of queue is %d '\n'", size(queue1));
	int val = 77;
	const void* key = &val; 
	void* myEl = qremove(queue1, findYear,key);
	car_t *cs = (car_t*)(myEl);
	if(myEl != NULL)
		{
			printf("Found year %d", cs->year);
			printf("\n");
		}
	//	printf("Size of queue is %d '\n'", size(queue1));
	car_t *c2 = (car_t*)qget(queue1);
	car_t *c4 = (car_t*)qget(queue1);
	car_t *c6 = (car_t*)qget(queue1);
	printf("%lf", c2->price);

	//  free(c);
	//	free(c3);
	//	free(c5);
	free(cs);
	free(c2);
	free(c4);
 	free(c6);
	qclose(queue1);
	exit(EXIT_SUCCESS);
}
