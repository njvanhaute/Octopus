//
//  main.cpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/3/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#include <fstream>
#include <iostream>

#include "FileLoader.hpp"
#include "Graphics.hpp"
#include "OctopusCore.hpp"

#define BUF_SIZE 4096

int main(int argc, char **argv) {
    Graphics *graphics;
    OctopusCore *core;
    SDL_Event event;
    bool quit = false;
    char *buf = new char[BUF_SIZE];
    
    try {
        graphics = new Graphics();
        core = new OctopusCore();
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    int romLen = readFileIntoBuffer("/Users/njvanhaute/Desktop/pong.ch8", buf, BUF_SIZE);
    if (core->loadROM(buf, romLen) != 0) {
        std::cerr << "Could not load ROM into core memory." << std::endl;
        return 1;
    }
    
    delete[] buf;
    
    while (!quit) {
        
        
        core->emulateCycle();
        
        
        // Process user input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
    
    delete graphics;
    delete core;
    return 0;
}
