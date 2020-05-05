//
//  OctopusCore.cpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/3/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#include <chrono>
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
        &OctopusCore::cpuSys, &OctopusCore::cpuJmp, &OctopusCore::cpuCall, &OctopusCore::cpuSEA,
        &OctopusCore::cpuSNEA, &OctopusCore::cpuSEB, &OctopusCore::cpuLoadA, &OctopusCore::cpuAddA,
        &OctopusCore::cpuArithmetic, &OctopusCore::cpuSNEB, &OctopusCore::cpuLoadIndex, &OctopusCore::cpuJmpReg,
        &OctopusCore::cpuRand, &OctopusCore::cpuDraw, &OctopusCore::cpuSKP, &OctopusCore::cpuMisc
    };
    
    MethodPointer at[VLT_SIZE] = {
        &OctopusCore::cpuLoadB, &OctopusCore::cpuOr, &OctopusCore::cpuAnd, &OctopusCore::cpuXor,
        &OctopusCore::cpuAddB, &OctopusCore::cpuSub, &OctopusCore::cpuSHR, &OctopusCore::cpuSubN,
        &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::NOP,
        &OctopusCore::NOP, &OctopusCore::NOP, &OctopusCore::cpuSHL, &OctopusCore::NOP
    };
    
    for (int i = 0; i < FONTSET_SIZE; i++) {
        memory[i] = chip8_fontset[i];
    }
    
    for (int i = 0; i < VLT_SIZE; i++) {
        methodTable[i] = vt[i];
        arithmeticTable[i] = at[i];
    }
    
    for (int i = 0; i < NUM_REGISTERS; i++) {
        V[i] = 0;
    }
    
    for (int i = 0; i < STACK_SIZE; i++) {
        stack[i] = 0;
    }
    
    for (int i = 0; i < NUM_KEYS; i++) {
        key[i] = 0;
    }
    
    clearDisplay();
    
    unsigned seed = (int) std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine RNG(seed);
    std::uniform_int_distribution<int> dist(0, 255);
    
    RandomNumberGenerator = RNG;
    RNGDistribution = dist;
}

OctopusCore::~OctopusCore() {}

void OctopusCore::emulateCycle() {
    drawFlag = false;
    fetchOpcode();
    execute();
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
}

void OctopusCore::execute() {
    MethodPointer method = methodTable[(opcode & 0xF000) >> 12];
    CALL_METHOD(this, method)();
}
                            
void OctopusCore::clearDisplay() {
    for (int i = 0; i < NUM_PIXEL_ROWS * NUM_PIXEL_COLS; i++) {
        gfx[i] = 0;
    }
}

void OctopusCore::NOP() {}

void OctopusCore::cpuSys() {
    short test = opcode & 0x00FF;
    if (test == 0xE0) {
        // CLS - Clear display
        clearDisplay();
        drawFlag = true;
        pc += 2;
    } else if (test == 0xEE) {
        // RET - Return from subroutine
        pc = stack[sp--];
    } else {
        // SYS (unused)
        
    }
}

void OctopusCore::cpuJmp() {
    pc = opcode & 0xFFF;
}

void OctopusCore::cpuCall() {
    stack[++sp] = pc + 2;
    pc = opcode & 0xFFF;
}

void OctopusCore::cpuSEA() {
    short reg = (opcode & 0xF00) >> 8;
    if (V[reg] == (opcode & 0xFF)) {
        pc += 2;
    }
    pc += 2;
}

void OctopusCore::cpuSEB() {
    short x = (opcode & 0xF00) >> 8;
    short y = (opcode &  0xF0) >> 4;
    if (V[x] == V[y]) {
        pc += 2;
    }
    pc += 2;
}

void OctopusCore::cpuSNEA() {
    short reg = (opcode & 0xF00) >> 8;
    if (V[reg] != (opcode & 0xFF)) {
        pc += 2;
    }
    pc += 2;
}

void OctopusCore::cpuLoadA() {
    short reg = (opcode & 0xF00) >> 8;
    V[reg] = (opcode & 0xFF);
    pc += 2;
}

void OctopusCore::cpuAddA() {
    short reg = (opcode & 0xF00) >> 8;
    V[reg] += (opcode & 0xFF);
    pc += 2;
}

void OctopusCore::cpuArithmetic() {
    MethodPointer method = arithmeticTable[opcode & 0xF];
    CALL_METHOD(this, method)();
}

void OctopusCore::cpuLoadB() {
    short x = (opcode & 0xF00) >> 8;
    short y = (opcode &  0xF0) >> 4;
    V[x] = V[y];
    pc += 2;
}

void OctopusCore::cpuOr() {
    short x = (opcode & 0xF00) >> 8;
    short y = (opcode &  0xF0) >> 4;
    V[x] |= V[y];
    pc += 2;
}

void OctopusCore::cpuAnd() {
    short x = (opcode & 0xF00) >> 8;
    short y = (opcode &  0xF0) >> 4;
    V[x] &= V[y];
    pc += 2;
}

void OctopusCore::cpuXor() {
    short x = (opcode & 0xF00) >> 8;
    short y = (opcode &  0xF0) >> 4;
    V[x] ^= V[y];
    pc += 2;
}

void OctopusCore::cpuAddB() {
    short x = (opcode & 0xF00) >> 8;
    short y = (opcode &  0xF0) >> 4;
    short total = V[x] + V[y];
    
    V[0xF] = (total > 0xFF) ? 1 : 0;
    V[x] = total & 0xFF;
    pc += 2;
}

void OctopusCore::cpuSub() {
    short x = (opcode & 0xF00) >> 8;
    short y = (opcode &  0xF0) >> 4;
    
    V[0xF] = V[x] > V[y] ? 1 : 0;
    V[x] = V[x] - V[y];
    pc += 2;
}

void OctopusCore::cpuSHR() {
    short x = (opcode & 0xF00) >> 8;
    V[0xF] = (V[x] & 0x1) ? 1 : 0;
    V[x] >>= 1;
    pc += 2;
}

void OctopusCore::cpuSubN() {
    short x = (opcode & 0xF00) >> 8;
    short y = (opcode &  0xF0) >> 4;
    
    V[0xF] = V[x] < V[y] ? 1 : 0;
    V[x] = V[y] - V[x];
    pc += 2;
}

void OctopusCore::cpuSHL() {
    short x = (opcode & 0xF00) >> 8;
    V[0xF] = (V[x] & 0x1) ? 1 : 0;
    V[x] <<= 1;
    pc += 2;
}

void OctopusCore::cpuSNEB() {
    short x = (opcode & 0xF00) >> 8;
    short y = (opcode &  0xF0) >> 4;
    
    if (V[x] != V[y]) {
        pc += 2;
    }
    pc += 2;
}

void OctopusCore::cpuLoadIndex() {
    I = opcode & 0xFFF;
    pc += 2;
}

void OctopusCore::cpuJmpReg() {
    pc = (opcode & 0xFFF) + V[0];
}

void OctopusCore::cpuRand() {
    short rand = RNGDistribution(RandomNumberGenerator);
    short kk = opcode & 0xFF;
    short x = (opcode & 0xF00) >> 8;
    V[x] = rand & kk;
    pc += 2;
}

void OctopusCore::cpuDraw() {
    
}

void OctopusCore::cpuSKP() {
    short x = (opcode & 0xF00) >> 8;
    if ((opcode & 0xF) == 0xE && key[V[x]]) {
        pc += 2;
    } else if ((opcode & 0xF) == 0x1 && !key[V[x]]) {
        pc += 2;
    }
    pc += 2;
}

void OctopusCore::cpuMisc() {
    short x = (opcode & 0xF00) >> 8;
    short tail = (opcode & 0xFF);
    switch (tail) {
        case 0x07:
            V[x] = delay_timer;
            break;
            
        case 0x0A:
            
            break;
        case 0x15:
            delay_timer = V[x];
            break;
        case 0x18:
            sound_timer = V[x];
            break;
        case 0x1E:
            I += V[x];
            break;
        case 0x29:
            break;
        case 0x33:
            break;
        case 0x55:
            for (int i = 0; i < x + 1; i++) {
                memory[I + i] = V[i];
            }
            break;
        case 0x65:
            for (int i = 0; i < x + 1; i++) {
                V[i] = memory[I + i];
            }
            break;
        default:
            throw std::exception();
            break;
    }
    pc += 2;
}
