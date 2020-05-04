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
    
    try {
        graphics = new Graphics();
        core = new OctopusCore();
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
}
