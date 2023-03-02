CC = gcc
CFLAGS = -ansi -Wall -g -pedantic

all: TransporteUSB

TransporteUSB: main.o List_Functions.o
	$(CC) $(CFLAGS) -o TransporteUSB main.o List_Functions.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

List_Functions.o: List_Functions.c
	$(CC) $(CFLAGS) -c List_Functions.c

clean:
	rm *.o TransporteUSB