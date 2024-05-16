CC = gcc
PROGRAM = txtcp
CFLAGS = -Wall -O2

OBJS = main.o sub.o
$(PROGRAM) : $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM)

main.o: main.c includes.h
	$(CC) -c $(CFLAGS) main.c

sub.o: sub.c includes.h
	$(CC) -c $(CFLAGS) sub.c

clean:
	rm $(PROGRAM) $(OBJS)


