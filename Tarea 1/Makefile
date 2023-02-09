CC = gcc
CFLAGS = -ansi -Wall -pedantic

all: Twitter

Twitter: main.o LinkedListTweets.o LinkedListUsers.o
	$(CC) $(CFLAGS) -o Twitter main.o LinkedListTweets.o LinkedListUsers.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

LinkedListTweets.o: LinkedListTweets.c
	$(CC) $(CFLAGS) -c LinkedListTweets.c

LinkedListUsers.o: LinkedListUsers.c
	$(CC) $(CFLAGS) -c LinkedListUsers.c

clean:
	rm *.o Twitter
