CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=queue.o hash.o webpage.o pageio.o

all: test1 pagetest

%.o:					%.c %.h
							gcc $(CFLAGS) -c $<

all:					$(OFILES)
							ar cr ~/engs50/tse/lib/libutils.a $(OFILES)

pagetest.o:		pagetest.c webpage.c webpage.h pageio.h
							gcc $(CFLAGS) -c pagetest.c

test1:				test1.o hash.o queue.o
							gcc $(CFLAGS) test1.o hash.o queue.o -o test1

pagetest:			pagetest.o pageio.o webpage.o webpage.c
							gcc $(CFLAGS) pagetest.o pageio.o webpage.o -o pagetest 

clean:
							rm -f *.o test1 pagetest

