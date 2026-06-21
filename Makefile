CC=g++
CFLAGS=-Wall -Werror 
INCLUDE=-Iinclude/
SDL=$(shell pkg-config --cflags --libs sdl2)
EXE_NAME=main

all: main
	@echo "Built successfully"

main: main.o chip8.o platform.o
	@echo "Linking main.o, chip8.o and platform.o"
	$(CC) $(CFLAGS) $(SDL) main.o chip8.o platform.o -o $(EXE_NAME)
	@echo

main.o:
	@echo "Compiling main.cpp..."
	$(CC) $(CFLAGS) $(INCLUDE) -c src/main.cpp
	@echo

chip8.o:
	@echo "Compiling chip8.cpp..."
	$(CC) $(CFLAGS) $(INCLUDE) -c src/chip8.cpp
	@echo

platform.o:
	@echo "Compiling platform.cpp..."
	$(CC) $(CFLAGS) $(INCLUDE) -c src/platform.cpp
	@echo

clean:
	@echo "Cleaning..."
	rm *.o $(EXE_NAME)
