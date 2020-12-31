CC = g++
CFLAGS = -g -Wall
TARGET = main

output: main.o listener.o menus.o global.o listenerManager.o
	$(CC) -pthread main.o listener.o menus.o global.o listenerManager.o -o output
	rm *.o

main.o: main.cpp 
	g++ -pthread -c main.cpp 

listener.o: listener.cpp listener.h
	g++ -pthread -c listener.cpp

menus.o: menus.cpp menus.h
	g++ -pthread -c menus.cpp

global.o: global.cpp global.h
	g++ -c global.cpp

listenerManager.o: listenerManager.cpp listenerManager.h
	g++ -c listenerManager.cpp

clean:
	rm output
