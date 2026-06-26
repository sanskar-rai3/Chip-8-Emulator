# CHIP-8 Emulator

A CHIP-8 emulator written in C++ using SDL2.

This project emulates the classic CHIP-8 virtual machine and can run many CHIP-8 games, demos, and test ROMs.

---

## Features

* Classic CHIP-8 instruction set
* SDL2-based graphics rendering and audio
* Keyboard input support
* Delay timer support
* ROM loading from file
* Adjustable display scaling
* Configurable CPU frequency

---

## Requirements

* C++17 compatible compiler
* SDL2
* GNU Make

---

## Building

```bash
git clone https://github.com/sanskar-rai3/Chip-8-Emulator
cd Chip-8-Emulator
make
```

This produces:

```bash
./main
```

---

## Usage

```bash
./main <Scale> <CPU_HZ> <ROM_Path>
```

### Arguments

| Argument | Description                       |
| -------- | --------------------------------- |
| Scale    | Window scale factor               |
| CPU_HZ   | CPU frequency (cycles per second) |
| ROM_Path | Path to a CHIP-8 ROM              |

---

### Example

```bash
./main 15 700 "roms/games/Pong [Paul Vervalin, 1990].ch8"
```

---

## Controls

```text
CHIP-8        Keyboard

1 2 3 C       1 2 3 4
4 5 6 D       Q W E R
7 8 9 E       A S D F
A 0 B F       Z X C V
```

---

## Emulation Design

* Fixed-frequency CPU loop using accumulator timing
* CPU execution is decoupled from rendering
* SDL2 handles framebuffer presentation

---

## Supported Variant

* CHIP-8 (classic with audio)

---

## Not Supported

* SCHIP
* XO-CHIP

---

## Tested ROMs

* IBM Logo
* Pong
* Tetris
* Space Invaders
* Brix
* Maze
* Particle Demo

---

## License

This project is licensed under the MIT License.

Full license text is available in the `LICENSE` file.

---

### ROMs Notice

The ROM files in `roms/` are sourced from:
https://github.com/kripod/chip8-roms

That repository explicitly allows redistribution of the ROM package in its original form.

These ROMs are not part of this emulator’s MIT license.
All rights remain with their original authors.