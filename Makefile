CC=gcc
CFLAGS=-Wall -Wextra
SRCS=main.c
OBJS=$(SRCS:.c=.o)
MAIN=main
HASSRCS=hash.c
HASHOBJS=$(HASSRCS:.c=.o)
HASH=hash

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

$(HASH): $(HASHOBJS)
	$(CC) $(CFLAGS) -o $(HASH) $(HASHOBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -g

clean:
	$(RM) *.o *~ $(MAIN) $(HASH)
