CC = gcc
PROGRAM = txtcp
CFLAGS = -Wall -O2

OBJS = kadai2.o kadai2_sub.o
$(PROGRAM) : $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM)

main.o: kadai2.c includes.h
	$(CC) -c $(CFLAGS) main.c

sub.o: kadai2_sub.c includes.h
	$(CC) -c $(CFLAGS) sub.c

clean:
	rm $(PROGRAM) $(OBJS)


