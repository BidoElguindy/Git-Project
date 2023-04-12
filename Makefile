CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS = -g 
CC = gcc

PROGRAMS = List

all : $(PROGRAMS)

List : List.o
	$(CC) -o $@ $(CFLAGS) $^

clean :
	rm -f *.o *~ $(PROGRAMS)
