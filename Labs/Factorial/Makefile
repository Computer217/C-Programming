# This is a comment line

#define variables so it is easier to make changes
CC=gcc
CFLAGS=-g -Wall -std=c99
TARGETS=ex-factorial catalan

all: $(TARGETS)

ex-factorial: ex-factorial.c
	cc $(CFLAGS) -o ex-factorial ex-factorial.c

catalan: catalan.c 
	cc $(CFLAGS) -o catalan catalan.c

clean:
	rm -rf *.o *~ $(TARGETS) a.out
