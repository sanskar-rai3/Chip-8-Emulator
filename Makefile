CC=g++
CFLAGS=-c -Wall -Werror 
INCLUDE=-Iinclude/
SDL=$(shell pkg-config --cflags --libs sdl2)
EXE_NAME=main

all: main
	@echo "Built successfully"

main: main.o chip8.o render.o
	@echo "Linking main.o, chip8.o and render.o"
	$(CC) main.o chip8.o render.o $(SDL) -o $(EXE_NAME)
	@echo

main.o:
	@echo "Compiling main.cpp..."
	$(CC) $(CFLAGS) $(INCLUDE) src/main.cpp
	@echo

chip8.o:
	@echo "Compiling chip8.cpp..."
	$(CC) $(CFLAGS) $(INCLUDE) src/chip8.cpp
	@echo

render.o:
	@echo "Compiling chip8.cpp..."
	$(CC) $(CFLAGS) $(INCLUDE) src/render.cpp
	@echo

clean:
	@echo "Cleaning..."
	rm *.o $(EXE_NAME)
