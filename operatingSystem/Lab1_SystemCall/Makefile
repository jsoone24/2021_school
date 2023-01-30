
CC = gcc
SRCS = fork.c exec.c cat.c pipe.c
TARG = fork exec cat pipe
OBJS = $(SRCS:.c=.o)

all: $(TARG)

fork:
	$(CC) -o fork fork.c

exec:
	$(CC) -o exec exec.c

cat:
	$(CC) -o cat cat.c

pipe:
	$(CC) -o pipe pipe.c

clean:
	rm -f $(OBJS) $(TARG)