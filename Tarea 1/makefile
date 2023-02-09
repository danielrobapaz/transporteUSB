CC = gcc
CFLAGS = -ansi -Wall -pedantic

all: Twitter

Twitter: main.o LinkedListTweets.o
	$(CC) $(CFLAGS) -o Twitter main.o LinkedListTweets.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

LinkedListTweets.o: LinkedListTweets.c
	$(CC) $(CFLAGS) -c LinkedListTweets.c

clean:
	rm *.o Twitter
