CFLAGS = -g 
CC = gcc

all : maintemp

List.o : List.c List.h 
	$(CC) $(CFLAGS) -c $^

Work.o : Work.c List.c Work.h List.h
	$(CC) $(CFLAGS) -c $^

Commit.o : Commit.c Work.c List.c Commit.h Work.h List.h
	$(CC) $(CFLAGS) -c $^

maintemp : List.o Work.o Commit.o
	$(CC) $(CFLAGS) -o $@ maintemp.c $^

clean :
	rm -f *.o 


