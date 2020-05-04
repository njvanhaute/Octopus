//
//  OctopusCore.hpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/3/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#ifndef OctopusCore_hpp
#define OctopusCore_hpp

class OctopusCore {
    static const int MEM_SIZE = 4096;
    static const int NUM_REGISTERS = 16;
    static const int NUM_PIXEL_ROWS = 32;
    static const int NUM_PIXEL_COLS = 64;
    static const int STACK_SIZE = 16;
    static const int NUM_KEYS = 16;
    
    unsigned short opcode; //
    unsigned char memory[MEM_SIZE]; // 
    unsigned char V[NUM_REGISTERS]; //
    unsigned short I; //
    unsigned short pc; //
    unsigned char gfx[NUM_PIXEL_COLS * NUM_PIXEL_ROWS]; //
    unsigned char delay_timer; //
    unsigned char sound_timer; //
    unsigned short stack[STACK_SIZE]; //
    unsigned short sp; //
    unsigned char key[NUM_KEYS]; //
    
public:
    OctopusCore();
    ~OctopusCore();
    void emulateCycle();
    int loadROM(char *, int);
};

#endif /* OctopusCore_hpp */
