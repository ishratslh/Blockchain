#les variables
CC = gcc
CFLAGS = -g

#les methodes
all : p1 p2 p3 p4

p1 : test_part1.o partie1.o
	$(CC) $(CFLAGS) -o p1 test_part1.o partie1.o -lm

p2 : test_part2.o partie1.o partie2.o
	$(CC) $(CFLAGS) -o p2 test_part2.o partie1.o partie2.o -lm

p3 : test_part3.o partie2.o partie3.o
	$(CC) $(CFLAGS) -o p3 test_part3.o partie1.o partie2.o partie3.o -lm

p4 : test_part4.o partie3.o partie4.o
	$(CC) $(CFLAGS) -o p4 test_part4.o partie1.o partie2.o partie3.o partie4.o -lm -lssl -lcrypto

test_part1.o : test_part1.c
	$(CC) $(CFLAGS) -c test_part1.c

test_part2.o : test_part2.c 
	$(CC) $(CFLAGS) -c test_part2.c

test_part3.o : test_part3.c
	$(CC) $(CFLAGS) -c test_part3.c

test_part4.o : test_part4.c
	$(CC) $(CFLAGS) -c test_part4.c

partie1.o : partie1.c partie1.h
	$(CC) $(CFLAGS) -c partie1.c

partie2.o : partie2.c partie2.h
	$(CC) $(CFLAGS) -c partie2.c

partie3.o : partie3.c partie3.h
	$(CC) $(CFLAGS) -c partie3.c

partie4.o : partie4.c partie4.h
	$(CC) $(CFLAGS) -c partie4.c

clean:
	rm -f *.o p1 p2 p3 p4