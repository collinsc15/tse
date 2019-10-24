CFLAGS=-Wall -pedantic -std=c11 -I ~/engs50/tse/utils -L ~/engs50/tse/lib -g
LIBS=-lutils -lcurl

all: pagetest

%o: %.c %.h
						gcc $(CFLAGS) -c $<

pagetest:		pagetest.c
						gcc $(CFLAGS) pagetest.c $(LIBS) -o $@

clean:
						rm -f *.o pagetest
