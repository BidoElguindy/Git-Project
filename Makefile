CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS = -g 
CC = gcc

PROGRAMS = List Work Commit

all : $(PROGRAMS)

List : List.o
	$(CC) -o $@ $(CFLAGS) $^

Work : Work.o List.o
	$(CC) -o $@ $(CFLAGS) $^

Commit : Commit.o Work.o List.o
	$(CC) -o $@ $(CFLAGS) $^

List.o : List.c List.h
	$(CC) $(CFLAGS) -c $^

Work.o : Work.c Work.h
	$(CC) $(CFLAGS) -c $^

Commit.o : Commit.c Commit.h
	$(CC) $(CFLAGS) -c $^

clean :
	rm -f *.o *~ $(PROGRAMS)
