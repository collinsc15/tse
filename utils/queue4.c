/* queue4.c --- 
 * 
 * 
 * Author: Vlado Vojdanovski
 * Created: Mon Oct 14 17:21:22 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void square(void *num){
  *(int *)num *= *(int *)num;
}

int main(void){
  queue_t *qt;
  qt = qopen();
  int x = 2;
  int y = 420;
  
  qput(qt,(void*)&x);
  qput(qt,(void*)&y);

  qapply(qt, square);

  int x2 = *(int *)(qget(qt));
  int y2 = *(int *)(qget(qt));
  if((4 != x2) && ((420*420) != y2)){
    printf("YOU HAVE FAILED THIS CITY!");
    exit(EXIT_FAILURE);
  } 
  
  
  printf("testApply succeeded\n");
  qclose(qt);   
  exit(EXIT_SUCCESS);
}
