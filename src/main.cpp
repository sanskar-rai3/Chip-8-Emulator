#include "chip8.hpp"

#include <cstdlib>
#include <iostream>

void handle_argc(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "error: No ROM inputed\n";

        std::exit(EXIT_FAILURE);
    }
    else if (argc > 2) {
        std::cout << "error: Too many arguments: expected one argument"
                     "usage:\n"
                     "main {ROM path}\n";
        
        std::exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    handle_argc(argc, argv);

    Chip8 chip8;

    char *rom_path = argv[1];
    std::cout << "rom_path: " << rom_path << std::endl;
    chip8.loadROM(rom_path);


	return 0;
}
