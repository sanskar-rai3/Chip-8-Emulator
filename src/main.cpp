#include "chip8.hpp"
#include "render.hpp"

#include <cstdlib>
#include <iostream>
#include <chrono>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "error: Usage: " << argv[0] << " <Scale> <Delay> <ROM Path>\n";
        std::exit(EXIT_FAILURE);
    }

    int videoScale       = std::stoi(argv[1]);
    int cycleDelay       = std::stoi(argv[2]);
    const char *ROM_PATH = argv[3];

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
    auto lastCycleTime = std::chrono::high_resolution_clock::now();

    bool quit = false;
    while (!quit) {
        quit = platform.ProcessInput(chip8.keypad);

        auto currentTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double, std::chrono::microseconds::period>(currentTime - lastCycleTime).count();

        if (deltaTime > cycleDelay) {
            lastCycleTime = currentTime;

            chip8.Clock();

            platform.Update(chip8.video, videoPitch);
        }
    }

	return 0;
}
