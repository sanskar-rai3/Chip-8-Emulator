#include "chip8.hpp"

#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <cstdint>

Chip8::Chip8() {
    pc = START_ADDRESS;

    for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
        memory[FONT_START_ADDRESS + i] = fontset[i];
    }

    randGen.seed(
        std::chrono::system_clock::now()
        .time_since_epoch()
        .count()
    );

    randByte = std::uniform_int_distribution<uint8_t>(0, 255u);
}

void Chip8::LoadROM(const char *filename) {
    std::ifstream file(filename,
         std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "error: No such file or directory\n";
        std::exit(EXIT_FAILURE);
    }

    std::streampos size = file.tellg();
    char *buffer        = new char[size];

    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    for (unsigned int i = 0; i < size; ++i) {
        memory[START_ADDRESS + i] = buffer[i];
    }

    delete[] buffer;
}

void Chip8::Clock() {
    opcode = (memory[pc] << 8u) | memory[pc + 1];
    pc    += 2;

    switch (opcode & 0xF000u) {
        case 0x0000u: 
            switch (opcode & 0x000Fu) {
                case 0x0000u: OP_00E0(); break;
                case 0x000Eu: OP_00EE(); break;
            }
        break;

        case 0x1000u: OP_1nnn(); break;
        case 0x2000u: OP_2nnn(); break;
        case 0x3000u: OP_3xkk(); break;
        case 0x4000u: OP_4xkk(); break;
        case 0x5000u: OP_5xy0(); break;
        case 0x6000u: OP_6xkk(); break;
        case 0x7000u: OP_7xkk(); break;
        case 0x9000u: OP_9xy0(); break;
        case 0xA000u: OP_Annn(); break;
        case 0xB000u: OP_Bnnn(); break;
        case 0xC000u: OP_Cxkk(); break;
        case 0xD000u: OP_Dxyn(); break;

        case 0x8000u:
            switch (opcode & 0x000Fu) {
                case 0x0000u: OP_8xy0(); break;
                case 0x0001u: OP_8xy1(); break;
                case 0x0002u: OP_8xy2(); break;
                case 0x0003u: OP_8xy3(); break;
                case 0x0004u: OP_8xy4(); break;
                case 0x0005u: OP_8xy5(); break;
                case 0x0006u: OP_8xy6(); break;
                case 0x0007u: OP_8xy7(); break;
                case 0x000Eu: OP_8xyE(); break;
            }
            break;

        case 0xE000u:
            switch (opcode & 0x00FFu) {
                case 0x009E: OP_Ex9E(); break;
                case 0x00A1: OP_ExA1(); break;
            }
            break;

        case 0xF000u:
            switch (opcode & 0x00FFu) {
                case 0x0007u: OP_Fx07(); break;
                case 0x000Au: OP_Fx0A(); break;
                case 0x0015u: OP_Fx15(); break;
                case 0x0018u: OP_Fx18(); break;
                case 0x001Eu: OP_Fx1E(); break;
                case 0x0029u: OP_Fx29(); break;
                case 0x0033u: OP_Fx33(); break;
                case 0x0055u: OP_Fx55(); break;
                case 0x0065u: OP_Fx65(); break;
            }
            break;
    }

    if (delayTimer > 0) {
        --delayTimer;
    }

    if (soundTimer > 0) {
        --soundTimer;
    }
}


// 00E0 
// CLS - clear the display
void Chip8::OP_00E0() {
    std::memset(video, 0, sizeof(video));
}


// 00EE
// RET - return from a subroutine
void Chip8::OP_00EE() {
    --sp;
    pc = stack[sp];
}


// 1nnn
// JP addr - jump to location nnn
void Chip8::OP_1nnn() {
    uint16_t address = opcode & 0x0FFFu;

    pc = address;
}


// 2nnn
// CALL addr - call subroutine at nnn
void Chip8::OP_2nnn() {
    uint16_t address = opcode & 0x0FFFu;

    stack[sp] = pc;
    ++sp;
    pc = address;
}


// 3xkk
// SE Vx, byte - skip next instruction if Vx = kk
void Chip8::OP_3xkk() {
    uint8_t Vx   = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (V[Vx] == byte) {
        pc += 2;
    }
}


// 4xkk
// SNE Vx, byte - skip next instruction if  Vx = Vy
void Chip8::OP_4xkk() {
    uint8_t Vx   = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (V[Vx] != byte) {
        pc += 2;
    }
}


// 5xy0
// SE Vx, Vy - skip next instruction if Vx = Vy
void Chip8::OP_5xy0() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (V[Vx] == V[Vy]) {
        pc += 2;
    }
}


// 6xkk
// LD Vx, byte - set Vx = kk
void Chip8::OP_6xkk() {
    uint8_t Vx   = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    V[Vx] = byte;
}


// 7xkk
// ADD Vx, byte - set Vx = Vx + kk
void Chip8::OP_7xkk() {
    uint8_t Vx   = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    V[Vx] += byte;
}


// 9xy0
// SNE Vx, Vy - skip next instruction if Vx != Vy
void Chip8::OP_9xy0() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (V[Vx] != V[Vy]) {
        pc += 2;
    }
}


// Annn
// LD I, addr - set index = addr
void Chip8::OP_Annn() {
    uint16_t address = opcode & 0x0FFFu;

    index = address;
}


// Bnnn
// JP V0, addr - jump to nnn + V0
void Chip8::OP_Bnnn() {
    uint16_t address = opcode & 0x0FFFu;

    pc = V[0] + address;
}


// Cxkk
// RND Vx, byte - set Vx = random byte & kk
void Chip8::OP_Cxkk() {
    uint8_t Vx   = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    V[Vx] = randByte(randGen) & byte;
}


// Chip8 uses XOR drawing
// Dxyn
// DRW Vx, Vy, nibble - display n-byte sprite starting at memory location I at (Vx, Vy), set VF(collision) = 1 if any pixel changes 1 -> 0 else 0
void Chip8::OP_Dxyn() {
    uint8_t xPos = V[(opcode & 0x0F00) >> 8] % VIDEO_WIDTH;
    uint8_t yPos = V[(opcode & 0x00F0) >> 4] % VIDEO_HEIGHT;
    uint8_t height = opcode & 0x000F;

    V[0xF] = 0;

    for (int row = 0; row < height; row++) {
        uint8_t spriteByte = memory[index + row];

        for (int col = 0; col < 8; col++) {
            bool spritePixel = spriteByte & (0x80 >> col);

            if (spritePixel) {
                uint8_t x = (xPos + col) % VIDEO_WIDTH;
                uint8_t y = (yPos + row) % VIDEO_HEIGHT;

                uint32_t *pixel = &video[y * VIDEO_WIDTH + x];

                if (*pixel != 0)
                    V[0xF] = 1;

                *pixel ^= 0xFFFFFFFF;
            }
        }
    }
}


// 8xy0
// LD Vx, Vy - set Vx = Vy
void Chip8::OP_8xy0() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    V[Vx] = V[Vy];
}


// 8xy1
// OR Vx, Vy - set Vx = Vx | Vy
void Chip8::OP_8xy1() {
    uint8_t Vx = (opcode & 0x0F00) >> 8u;
    uint8_t Vy = (opcode & 0x00F0) >> 4u;

    V[Vx] |= V[Vy];
}


// 8xy2
// AND Vx, Vy - set Vx = Vx & Vy
void Chip8::OP_8xy2() {
    uint8_t Vx = (opcode & 0x0F00) >> 8u;
    uint8_t Vy = (opcode & 0x00F0) >> 4u;

    V[Vx] &= V[Vy];
}


// 8xy3
// XOR Vx, Vy - set Vx = Vx ^ Vy
void Chip8::OP_8xy3() {
    uint8_t Vx = (opcode & 0x0F00) >> 8u;
    uint8_t Vy = (opcode & 0x00F0) >> 4u;

    V[Vx] ^= V[Vy];
}


// 8xy4
// ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry
void Chip8::OP_8xy4() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = V[Vx] + V[Vy];

    V[0xF] = (sum > 255u) ? 1 : 0;

	V[Vx] = sum & 0xFFu;
}


// 8xy5
// SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow
void Chip8::OP_8xy5() {
 	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    V[0xF] = V[Vx] > V[Vy] ? 1 : 0;

    V[Vx] -= V[Vy];
}


// 8xy6
// SHR Vx - set Vx = Vx >> 1
void Chip8::OP_8xy6() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	// Save LSB in VF
	V[0xF] = (V[Vx] & 0x1u);

    V[Vx] >>= 1;
}


// 8xy7
// SUBN Vx, Vy - set Vx = Vy - Vx, set VF = NOT borrow
void Chip8::OP_8xy7() {
 	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    V[0xF] = V[Vy] > V[Vx] ? 1 : 0;

    V[Vx] = V[Vy] - V[Vx];
}


// 8xyE
// SHL Vx {, Vy} - set Vx = Vx << 1
void Chip8::OP_8xyE() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    // Save MSB in VF
    V[0xF] = (V[Vx] & 0x80u) >> 7u;

    V[Vx] <<= 1;
}


// Ex9E
// SKP Vx - skip next instruction if key with value of Vx is pressed
void Chip8::OP_Ex9E() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    uint8_t key = V[Vx];

    if (keypad[key]) {
        pc += 2;
    }
}


// ExA1
// SKNP Vx - skip next instruction if key with value of Vx is not pressed
void Chip8::OP_ExA1() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    uint8_t key = V[Vx];

    if (!keypad[key]) {
        pc += 2;
    }
}


// Fx07
// LD Vx, DT - set Vx = delay timer value
void Chip8::OP_Fx07() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    V[Vx] = delayTimer;
}


// Fx0A
// LD Vx, K - wait for a key press, store the value of the key in Vx
void Chip8::OP_Fx0A() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    bool keyPressed = false;

    for (uint8_t i = 0; i <= 0xFu; ++i) {
        if (keypad[i]) {
            V[Vx] = i;
            keyPressed = true;
            break;
        }
    }

    if (!keyPressed) {
        pc -= 2;
    }
}


// Fx15
// LD DT, Vx - set delay timer = Vx
void Chip8::OP_Fx15() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    delayTimer = V[Vx];
}


// Fx18
// LD ST, Vx - set sound timer = Vx
void Chip8::OP_Fx18() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    soundTimer = V[Vx];
}


// Fx1E
// ADD I, Vx - set I = I + Vx
void Chip8::OP_Fx1E() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    index += V[Vx];
}


// Fx29
// LD F, Vx - set I = location of sprite for digit Vx
void Chip8::OP_Fx29() {
    uint8_t Vx    = (opcode & 0x0F00u) >> 8u;
    uint8_t digit = V[Vx];

    index = FONT_START_ADDRESS + (5 * digit);
}


// Fx33
// LD B, Bx - store decimal value of Vx in memory location I, I + 1 and I + 2
void Chip8::OP_Fx33() {
    uint8_t Vx    = (opcode & 0x0F00u) >> 8u;
    uint8_t value = V[Vx];   

    // ones place
    memory[index + 2] = value % 10;
    value /= 10;

     // tens place
    memory[index + 1] = value % 10;
    value /= 10;   

    // hundreds place
    memory[index] = value % 10;
}


// Fx55
// LD [I], Vx - store registers V0 - Vx in memory starting at location I
void Chip8::OP_Fx55() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    for (uint8_t i = 0; i <= Vx; ++i) {
        memory[index + i] = V[i];
    }
}


// Fx65
// LD Vx, [I] - read registers V0 - Vx from memory starting at location I
void Chip8::OP_Fx65() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    for (uint8_t i = 0; i <= Vx; ++i) {
        V[i] = memory[index + i];
    }
}
