/* queue6.c --- 
 * 
 * 
 * Author: Vlado Vojdanovski
 * Created: Mon Oct 14 17:33:57 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "queue.h"
//#include "hash.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct person_t{
  char name[15];
  int age;
} person_t;

struct person_t *make_person(char *name, int age){
  struct person_t *p;
  if(!(p = (struct person_t *)malloc(sizeof(person_t)))){
    printf("error creating person");
    return NULL;
  }
  strcpy(p->name, name);
  p->age = age;
  return p;
}


bool searchByName(void *p, const void *n){
  struct person_t *person = (struct person_t *)p;
  char *nameLookingFor = (char *)n;
  char *name = person->name;
  if(strcmp(name, nameLookingFor) == 0)
    return true;
  else
    return false;
}

int main(void){
  // Make hashtable
  queue_t* hp = qopen();

  // Make peoples, me and my amazing friends :)
  person_t *person1 = make_person("Sudharsan", 18); // Awesome Person
  person_t *person2 = make_person("Taylor", 19); // Awesome Person
  person_t *person3 = make_person("James", 200); // Sometimes Awesom Person
  person_t *person4 = make_person("Ben Liu", 27); // Miss you!

	// hput(hp, (void *)person1, (const char *)person1->name, sizeof(person1->name));
  //hput(hp, (void *)person2, (const char *)person2->name, sizeof(person2->name));
  //hput(hp, (void *)person3, (const char *)person3->name, sizeof(person3->name));
  //hput(hp, (void *)person4, (const char *)person4->name, sizeof(person4->name));
	//	qput(hp, (void*)person1);
	//qput(hp, (void*)person2);
	//qput(hp, (void*)person3);
	//qput(hp, (void*)person4);
  //xc Make sure we are returning the right values when we search for it

  char *returnedName = qremove(hp, searchByName, (const char *)person1->name);
  if(strcmp(returnedName,"Sudharsan") != 0){
    printf("Failure: Someone returned, but not who we think!\n");
    exit(EXIT_FAILURE);
  }

	qclose(hp);
	free(person1);
	free(person2);
	free(person3);
	free(person4);
	free(returnedName);
	
  printf("testhRemoveValidity Succeeded!\n");
  exit(EXIT_SUCCESS);


}
