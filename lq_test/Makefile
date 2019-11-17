CFLAGS=-Wall -pedantic -std=c11 -I -g -lpthread

all: lqueue_test working_queue_test


%o : %.c %.h
		gcc $(CFLAGS) -c $<

lqueue_test: lqueue_test.c
				gcc $(CFLAGS) lqueue_test.c -o $@

working_queue_test: working_queue_test.c
				gcc $(CFLAGS) working_queue_test.c -o $@

clean:
		rm -f *.0 lqueue_test working_queue_test
