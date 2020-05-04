//
//  main.cpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/3/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#include <iostream>
#include "Graphics.hpp"
#include "OctopusCore.hpp"

int main(int argc, char **argv) {
    Graphics *graphics;
    OctopusCore *core;
    
    bool quit = false;
    
    try {
        graphics = new Graphics();
        core = new OctopusCore();
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    SDL_Event event;
    while (!quit) {
        
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
