//
//  FileLoader.cpp
//  Octopus
//
//  Created by Nicholas Vanhaute on 5/4/20.
//  Copyright © 2020 Nicholas Vanhaute. All rights reserved.
//

#include <fstream>
#include "FileLoader.hpp"

// Returns size of useful buffer
int readFileIntoBuffer(char const *filepath, char *buf, int buf_size) {
    std::ifstream fin(filepath, std::ios::in | std::ios::binary);
    fin.read(buf, buf_size);
    return (int) fin.gcount();
}
