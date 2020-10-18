TARGETS=runseq runpipeline checkof
SRCS=$(patsubst %,%.c,$(TARGETS))
CC=gcc
CFLAGS= -Wall -g -pthread

all : $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean: 
	@rm -f $(TARGETS) *.o a.out seqOutput shOutput pipeOutput

ultraclean: clean
	@rm -f *.dat

sub: 
	@zip $${PWD##*/}.zip $(SRCS) Makefile
