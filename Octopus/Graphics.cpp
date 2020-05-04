//
//  Graphics.cpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/3/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#include <iostream>
#include "Graphics.hpp"

const int SCREEN_X               = 100;
const int SCREEN_Y               = 100;
const int SCREEN_WIDTH           = 640;
const int SCREEN_HEIGHT          = 320;
const int RENDERER_INDEX_DEFAULT =  -1;

const char *WINDOW_TITLE = "Octopus";

void logSDLError(std::ostream &os, const std::string &msg) {
    os << msg << " error: " << SDL_GetError() << std::endl;
}

Graphics::Graphics() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        logSDLError(std::cerr, "SDL_Init");
        throw std::exception();
    }
    
    window = SDL_CreateWindow(WINDOW_TITLE, SCREEN_X, SCREEN_Y, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (window == nullptr) {
        logSDLError(std::cerr, "SDL_CreateWindow");
        throw std::exception();
    }

    renderer = SDL_CreateRenderer(window, RENDERER_INDEX_DEFAULT, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (renderer == nullptr) {
        logSDLError(std::cerr, "SDL_CreateRenderer");
        throw std::exception();
    }
}

Graphics::~Graphics() {
    if (window) {
        SDL_DestroyWindow(window);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    SDL_Quit();
}
