CFLAGS=-Wall -pedantic -std=c11 -I ~/engs50/tse/utils -L ~/engs50/tse/lib -g -pthread
LIBS=-lutils -lcurl

all: indetest indexer pagetest threadedIndexer

%o: %.c %.h
						gcc $(CFLAGS) -c $<

pagetest: 		pagetest.c
							gcc $(CFLAGS) pagetest.c $(LIBS) -o $@

indexer:		indexer.c
						gcc $(CFLAGS) indexer.c $(LIBS) -o $@

threadedIndexer:		threadedIndexer.c
						gcc $(CFLAGS) threadedIndexer.c $(LIBS) -o $@

indetest:		indetest.c 
						gcc $(CFLAGS) indetest.c $(LIBS) -o $@

clean:
						rm -f *.o indexer indexerST2 indetest threadedIndexer
