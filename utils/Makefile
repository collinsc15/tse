CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=queue.o hash.o webpage.o pageio.o indexio.o lhash.o lqueue.o

%.o:					%.c %.h
							gcc $(CFLAGS) -c $<

all:					$(OFILES)
							ar cr ~/engs50/tse/lib/libutils.a $(OFILES)

clean:
							rm -f *.o 

