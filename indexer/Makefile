CFLAGS=-Wall -pedantic -std=c11 -I ~/engs50/tse/utils -L ~/engs50/tse/lib -g
LIBS=-lutils -lcurl

all: indexer pagetest

%o: %.c %.h
						gcc $(CFLAGS) -c $<

indexer:		indexerStep2.c
						gcc $(CFLAGS) indexerStep2.c $(LIBS) -o $@

pagetest: 		pagetest.c
						gcc $(CFLAGS) pagetest.c $(LIBS) -o $@

clean:
						rm -f *.o indexer
