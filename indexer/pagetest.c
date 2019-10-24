/* pagetest.c --- 
 * 
 * 
 * Author: Claire C. Collins
 * Created: Wed Oct 23 21:13:59 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <webpage.h>
#include <pageio.h>
void main(void){
	webpage_t *w= pageload(1, "pages");
	pagesave(w,101, "pages");
}
