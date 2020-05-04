//
//  OctopusCore.cpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/3/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#include <functional>
#include "OctopusCore.hpp"

#define CALL_METHOD(object,ptrToMember) ((object)->*(ptrToMember))
#define FONTSET_SIZE 80
#define VLT_SIZE 16

OctopusCore::OctopusCore() {
    pc          = 0x200;
    opcode      = 0;
    I           = 0;
    sp          = 0;
    delay_timer = 0;
    sound_timer = 0;
    drawFlag    = false;
    
    unsigned char chip8_fontset[FONTSET_SIZE] =
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
    
    // ALL NOPs for now; Will be implemented later
    MethodPointer vt[VLT_SIZE] = {
        &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP,
        &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP,
        &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP,
        &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP,
    };
    
    for (int i = 0; i < FONTSET_SIZE; i++) {
        memory[i] = chip8_fontset[i];
    }
    
    for (int i = 0; i < VLT_SIZE; i++) {
        vtable[i] = vt[i];
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

void OctopusCore::emulateCycle() {
    fetchOpcode();
    execute();
    drawFlag = false;
}

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

bool OctopusCore::drawFlagIsSet() {
    return drawFlag;
}

void OctopusCore::fetchOpcode() {
    opcode = memory[pc] << 8 | memory[pc + 1];
    pc += 2;
}

void OctopusCore::execute() {
    MethodPointer method = vtable[(opcode & 0xF000) >> 12];
    CALL_METHOD(this, method)();
}
                            

void OctopusCore::NOP() {}
