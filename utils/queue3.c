/* queue3.c --- 
 * 
 * 
 * Author: Vlado Vojdanovski
 * Created: Mon Oct 14 15:55:54 2019 (-0400)
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
#include <stdbool.h>                                                                                                                                                 
#include <unistd.h>
#include <queue.h>


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
	qput(queue1, (void*)c);
	qput(queue1, (void*)c3);
	qput(queue1, (void*)c5);

	car_t *c2 = (car_t*)qget(queue1);
	printf("%d \n", c2->year);
	car_t *c4 = (car_t*)qget(queue1);
	printf("%d \n", c4->year);
	car_t *c6 = (car_t*)qget(queue1);
	printf("%d \n", c6->year);

	car_t *c7 = (car_t*)qget(queue1);
	if (c7 != NULL)
		{
			printf("Failure");
		}

	exit(EXIT_SUCCESS);
}
