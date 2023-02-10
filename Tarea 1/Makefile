CC = gcc
CFLAGS = -ansi -Wall -pedantic

all: Twitter

Twitter: main.o Tweets_List_Functions.o Hash_Table_Functions.o User_List_Functions.o
	$(CC) $(CFLAGS) -o Twitter main.o Tweets_List_Functions.o Hash_Table_Functions.o User_List_Functions.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

Tweets_List_Functions.o: Tweets_List_Functions.c
	$(CC) $(CFLAGS) -c Tweets_List_Functions.c

Hash_Table_Functions.o: Hash_Table_Functions.c
	$(CC) $(CFLAGS) -c Hash_Table_Functions.c

User_List_Functions.o: User_List_Functions.c
	$(CC) $(CFLAGS) -c User_List_Functions.c

clean:
	rm *.o Twitter