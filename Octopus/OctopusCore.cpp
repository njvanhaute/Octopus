//
//  OctopusCore.cpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/3/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#include "OctopusCore.hpp"

OctopusCore::OctopusCore() {
    pc          = 0x200;
    opcode      = 0;
    I           = 0;
    sp          = 0;
    delay_timer = 0;
    sound_timer = 0;
    
    
    unsigned char chip8_fontset[80] =
    {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    
    for (int i = 0; i < 80; i++) {
        memory[i] = chip8_fontset[i];
    }
    
    for (int i = 0; i < NUM_REGISTERS; i++) {
        V[i] = 0;
    }
    
    for (int i = 0; i < NUM_PIXEL_ROWS * NUM_PIXEL_COLS; i++) {
        gfx[i] = 0;
    }
    
    for (int i = 0; i < STACK_SIZE; i++) {
        stack[i] = 0;
    }
    
    for (int i = 0; i < NUM_KEYS; i++) {
        key[i] = 0;
    }
}

OctopusCore::~OctopusCore() {}

void OctopusCore::emulateCycle() {}

// Returns 0 if successful, 1 otherwise
int OctopusCore::loadROM(char *buf, int bufSize) {
    for (int i = 0; i < bufSize; i++) {
        if (i + 512 >= MEM_SIZE) {
            // Overflowed memory
            return 1;
        }
        memory[i + 512] = buf[i];
    }
    
    return 0;
}
