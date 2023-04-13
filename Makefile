CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS = -g 
CC = gcc

PROGRAMS = List
PROGRAMS = List Work Commit

all : $(PROGRAMS)

List : List.o
	$(CC) -o $@ $(CFLAGS) $^

Work : Work.o List.o
	$(CC) -o $@ $(CFLAGS) $^

Commit : Commit.o Work.o List.o
	$(CC) -o $@ $(CFLAGS) $^

clean :
	rm -f *.o *~ $(PROGRAMS)
