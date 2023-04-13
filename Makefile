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

List.o : List.c 
	$(CC) $(CFLAGS) -c $^

Work.o : Work.c 
	$(CC) $(CFLAGS) -c $^

Commit.o : Commit.c 
	$(CC) $(CFLAGS) -c $^
	
clean :
	rm -f *.o *~ $(PROGRAMS)

