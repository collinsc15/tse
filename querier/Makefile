CFLAGS=-Wall -pedantic -std=c11 -I ~/engs50/tse/utils -L ~/engs50/tse/lib -g
LIBS=-lutils -lcurl

all:  querier

%o: %.c %.h
						gcc $(CFLAGS) -c $<
querier:	
						gcc $(CFLAGS) query.c $(LIBS) -o $@

clean:
						rm -f *.o querier
