CC = g++
CFLAGS = -g -Wall
TARGET = main

output: main.o listener.o menus.o global.o
	$(CC) -pthread main.o listener.o menus.o global.o -o output
	rm *.o

main.o: main.cpp 
	g++ -pthread -c main.cpp 

listener.o: listener.cpp listener.h
	g++ -pthread -c listener.cpp

menus.o: menus.cpp menus.h
	g++ -pthread -c menus.cpp

global.o: global.cpp global.h
	g++ -c global.cpp

clean:
	rm *.o output
