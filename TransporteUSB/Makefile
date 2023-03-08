CC = gcc
CFLAGS = -ansi -Wall -g -pedantic

all: TransporteUSB

TransporteUSB: main.o Service_List_Functions.o Sch_List_Functions.o
	$(CC) $(CFLAGS) -o TransporteUSB main.o Service_List_Functions.o Sch_List_Functions.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

Service_List_Functions.o: Service_List_Functions.c
	$(CC) $(CFLAGS) -c Service_List_Functions.c

Sch_List_Functions.o: Sch_List_Functions.c
	$(CC) $(CFLAGS) -c Sch_List_Functions.c

clean:
	rm *.o TransporteUSB
