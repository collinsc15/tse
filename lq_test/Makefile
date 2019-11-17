CFLAGS=-Wall -pedantic -std=c11 -I ~/engs50/tse/utils -L ~/engs50/tse/lib -g -lpthread
LIBS=-lutils -lcurl

all: lqueue_test lhash_test working_queue_test 

%o : %.c %.h
		gcc $(CFLAGS) -c $<

lqueue_test: 	lqueue_test.c
							gcc $(CFLAGS) lqueue_test.c $(LIBS) -o $@

lhash_test: 	lhash_test.c
							gcc $(CFLAGS) lhash_test.c $(LIBS) -o $@

working_queue_test: 	working_queue_test.c
											gcc $(CFLAGS) working_queue_test.c $(LIBS) -o $@

clean:
		rm -f *.o lqueue_test working_queue_test
