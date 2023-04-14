CFLAGS = -g 
CC = gcc

all : myGit

List.o : List.c List.h 
	$(CC) $(CFLAGS) -c $^

Work.o : Work.c List.c Work.h List.h
	$(CC) $(CFLAGS) -c $^

Commit.o : Commit.c Work.c List.c Commit.h Work.h List.h
	$(CC) $(CFLAGS) -c $^
	
Refs.o : Refs.c Commit.c Work.c List.c Refs.h Commit.h Work.h List.h
	$(CC) $(CFLAGS) -c $^

Branch.o : Branch.c Refs.c Commit.c Work.c List.c Branch.h Refs.h Commit.h Work.h List.h
	$(CC) $(CFLAGS) -c $^

myGit : List.o Work.o Commit.o Refs.o Branch.o
	$(CC) $(CFLAGS) -o $@ myGit.c $^

clean :
	rm -f *.o 
