CFLAGS=-Wall -pedantic -std=c11 -I ~/engs50/tse/utils -L ~/engs50/tse/lib -g
LIBS=-lutils -lcurl

all: crawler crawler5

%o: %.c %.h
						gcc $(CFLAGS) -c $<

crawler:		crawler.c
						gcc $(CFLAGS) crawler.c $(LIBS) -o $@
crawler5:		crawler5.c
						gcc $(CFLAGS) crawler5.c $(LIBS) -o $@

clean:
						rm -f *.o crawler crawler5
