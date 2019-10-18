CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=queue.o hash.o webpage.o

all: test1

hash.o: 			hash.c hash.h queue.h
							gcc $(CFLAGS) -c hash.c

test1.o:			test1.c hash.h queue.h
							gcc $(CFLAGS) -c test1.c

queue.o:			queue.c queue.h
							gcc $(CFLAGS) -c queue.c

all:					$(OFILES)
							ar cr ~/engs50/tse/lib/libutils.a $(OFILES)

test1:				test1.o hash.o queue.o
							gcc $(CFLAGS) test1.o hash.o queue.o -o test1

clean:
							rm -f *.o test1

