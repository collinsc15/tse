CFLAGS=-Wall -pedantic -std=c11 -I ~/engs50/tse/utils -L ~/engs50/tse/lib -g
LIBS=-lutils -lcurl

all: crawler

%o: %.c %.h
						gcc $(CFLAGS) -c $<

crawler:		
						gcc $(CFLAGS) crawler.c $(LIBS) -o $@
