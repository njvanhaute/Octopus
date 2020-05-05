//
//  OctopusCore.hpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/3/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#ifndef OctopusCore_hpp
#define OctopusCore_hpp

#include <chrono>
#include <random>

class OctopusCore {
    typedef void (OctopusCore::*MethodPointer)();
    
    static const int MEM_SIZE = 4096;
    static const int NUM_REGISTERS = 16;
    static const int NUM_PIXEL_ROWS = 32;
    static const int NUM_PIXEL_COLS = 64;
    static const int STACK_SIZE = 16;
    static const int NUM_KEYS = 16;
    
    unsigned short opcode;
    unsigned char memory[MEM_SIZE];
    unsigned char V[NUM_REGISTERS];
    unsigned short I;
    unsigned short pc;
    unsigned char gfx[NUM_PIXEL_COLS * NUM_PIXEL_ROWS];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[STACK_SIZE];
    unsigned short sp;
    unsigned char key[NUM_KEYS];
    
    MethodPointer methodTable[16];
    MethodPointer arithmeticTable[16];
    
    std::default_random_engine RandomNumberGenerator;
    std::uniform_int_distribution<int> RNGDistribution;
    
    bool drawFlag;
    
    void fetchOpcode();
    void execute();
    void clearDisplay();
    void NOP();
    void cpuSys();
    void cpuJmp();
    void cpuCall();
    void cpuSEA();
    void cpuSEB();
    void cpuSNEA();
    void cpuLoadA();
    void cpuAddA();
    void cpuLoadB();
    void cpuOr();
    void cpuAnd();
    void cpuXor();
    void cpuAddB();
    void cpuSub();
    void cpuSHR();
    void cpuSubN();
    void cpuSHL();
    void cpuSNEB();
    void cpuLoadIndex();
    void cpuJmpReg();
    void cpuRand();
    void cpuDraw();
    void cpuSKP();
    void cpuLoadRegFromDelay();
    void cpuWaitForKeyPress();
    void cpuSetDelayTimer();
    void cpuSetSoundTimer();
    void cpuAddIndex();
    void cpuSetSpriteLoc();
    void cpuBCD();
    void cpuMoveRegToMem();
    void cpuMoveMemToReg();
    void cpuArithmetic();
    void cpuMisc();
    
public:
    OctopusCore();
    ~OctopusCore();
    void emulateCycle();
    int loadROM(char *, int);
    bool drawFlagIsSet();
};

typedef void (OctopusCore::*MethodPointer)();

#endif /* OctopusCore_hpp */
