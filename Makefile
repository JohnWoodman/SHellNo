CC = g++
CFLAGS = -g -Wall
TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
