#include "chip8.hpp"
#include "platform.hpp"

#include <cstdlib>
#include <iostream>
#include <chrono>


int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "error: Usage: " << argv[0] << " <Scale> <CPU_HZ> <ROM Path>\n";
        std::exit(EXIT_FAILURE);
    }

    int videoScale          = std::stoi(argv[1]);
    const double CHIP8_HZ   = std::stoi(argv[2]);
    const double cycleDelay = 1.0 / CHIP8_HZ;
    const char *ROM_PATH    = argv[3];

    Platform platform(
        "CHIP-8 Emulator",
        VIDEO_WIDTH  * videoScale,
        VIDEO_HEIGHT * videoScale,
        VIDEO_WIDTH,
        VIDEO_HEIGHT
    );

    Chip8 chip8;
    chip8.LoadROM(ROM_PATH);

    int videoPitch     = sizeof(chip8.video[0]) * VIDEO_WIDTH;

    double accumulator = 0.0;
    auto lastTime      = std::chrono::high_resolution_clock::now();

    bool quit = false;
    while (!quit) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();

        lastTime     = currentTime;
        accumulator += deltaTime;

        quit = platform.ProcessInput(chip8.keypad);

        platform.SetSound(chip8.soundTimer > 0);

        while (accumulator >= cycleDelay) {
            chip8.Clock(); 
            accumulator -= cycleDelay;
        }

        platform.Update(chip8.video, videoPitch);
    }

	return 0;
}
