CXX      = g++
CXXFLAGS = -Wall -Werror 
INCLUDE  = -Iinclude/
SDL      = $(shell pkg-config --cflags --libs sdl2)
TARGET   = chip8emu

all: main
	@echo "Built successfully"

main: main.o chip8.o platform.o
	@echo "Linking main.o, chip8.o, platform.o, SDL2"
	$(CXX) $(CXXFLAGS) main.o chip8.o platform.o $(SDL) -o $(TARGET)
	@echo

main.o:
	@echo "Compiling main.cpp..."
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c src/main.cpp
	@echo

chip8.o:
	@echo "Compiling chip8.cpp..."
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c src/chip8.cpp
	@echo

platform.o:
	@echo "Compiling platform.cpp..."
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c src/platform.cpp
	@echo

clean:
	@echo "Cleaning..."
	rm *.o $(TARGET)
