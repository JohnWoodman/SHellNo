CC = g++
CFLAGS = -g -Wall
TARGET = main

output: main.o listener.o
	$(CC) -pthread main.o listener.o -o output
	rm *.o

main.o: main.cpp 
	g++ -pthread -c main.cpp

listener.o: listener.cpp listener.h
	g++ -pthread -c listener.cpp

clean:
	rm *.o output
