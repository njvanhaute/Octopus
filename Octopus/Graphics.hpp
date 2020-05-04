//
//  Graphics.hpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/3/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#ifndef Graphics_hpp
#define Graphics_hpp

#include <SDL.h>

class Graphics {
    SDL_Window *window;
    SDL_Renderer *renderer;
    
public:
    Graphics();
    ~Graphics();
};

#endif /* Graphics_hpp */
