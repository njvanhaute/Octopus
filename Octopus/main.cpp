//
//  main.cpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 4/30/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#include <iostream>
#include <SDL.h>

const int SCREEN_X     = 100;
const int SCREEN_Y     = 100;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT= 480;
const char *WINDOW_TITLE = "Octopus";

void logSDLError(std::ostream &os, const std::string &msg) {
    os << msg << " error: " << SDL_GetError() << std::endl;
}

void performSDLCleanup(SDL_Window *window, SDL_Renderer *renderer) {
    if (window) {
        SDL_DestroyWindow(window);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
}

SDL_Texture *loadTexture(const std::string &filepath, SDL_Renderer *renderer) {
    SDL_Texture *texture = nullptr;
    SDL_Surface *loadedImage = SDL_LoadBMP(filepath.c_str());
    
    if (loadedImage != nullptr) {
        texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
        SDL_FreeSurface(loadedImage);
        if (texture == nullptr) {
            logSDLError(std::cerr, "SDL_CreateTextureFromSurface");
        }
    } else {
        logSDLError(std::cerr, "SDL_LoadBMP");
    }
    return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, tex, NULL, &dst);
}

int main(int argc, const char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        logSDLError(std::cerr, "SDL_Init");
        return 1;
    }
    
    SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, SCREEN_X, SCREEN_Y, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (window == nullptr) {
        logSDLError(std::cerr, "SDL_CreateWindow");
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (renderer == nullptr) {
        logSDLError(std::cerr, "SDL_CreateRenderer");
        performSDLCleanup(window, renderer);
        SDL_Quit();
        return 1;
    }
    
    performSDLCleanup(window, renderer);
    return 0;
}
