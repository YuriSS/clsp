CC=gcc
CFLAGS=-Wall -Wextra
SRCS=main.c
OBJS=$(SRCS:.c=.o)
MAIN=main

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -g

clean:
	$(RM) *.o *~ $(MAIN)
