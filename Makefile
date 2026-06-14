CC=g++
CFLAGS=-c -Wall -Werror 
INCLUDE=-Iinclude/

all: main
	echo "Built successfully"

main: main.o chip8.o
	echo "Linking main.o and chip8.o"
	$(CC) main.o chip8.o -o main

main.o:
	echo "Compiling main.cpp..."
	$(CC) $(CFLAGS) $(INCLUDE) src/main.cpp

chip8.o:
	echo "Compiling chip8.cpp..."
	$(CC) $(CFLAGS) $(INCLUDE) src/chip8.cpp

clean:
	echo "Cleaning..."
	rm *.o main
