CC = gcc
LDLIBS = -lpthread -g -lm 
CFLAGS = -Wall -O -I./atomic_ops
#CFLAGS = -Wall -O -I./atomic_ops -g -pg -DDEBUG

SRCS = linkedlist.c util.c bench.c
OBJS = linkedlist.o util.o bench.o

BOBJS=util.o b_linked.o b_bench.o
NBOBJS=util.o nb_linked.o bench.o


all: b_linked nb_linked 

b_linked: $(BOBJS)
	$(CC) -o $@ $(BOBJS) $(LDLIBS)

nb_linked: $(NBOBJS)
	$(CC) -o $@ $(NBOBJS) $(LDLIBS)

util.o: util.c util.h
	$(CC) $(CFLAGS) -c $< -o $@

bench.o: bench.c bench.h linkedlist.h
	$(CC) $(CFLAGS) -c $< -o $@

b_bench.o: bench.c bench.h linkedlist.h
	$(CC) $(CFLAGS) -DBLINKED_LIST -c $< -o $@

nb_linked.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c $< -o $@

b_linked.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -DBLINKED_LIST -c $< -o $@


install:
	cp ./bin/* /usr/bin
clean:
	rm -rf *.o b_linked nb_linked *.gch
	rm -rf *.dSYM

