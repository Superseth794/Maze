//
//  main.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include <iostream>

#include "Maze.hpp"

int main(int argc, const char * argv[]) {
    
    constexpr unsigned int windowWidth = 3200;
    constexpr unsigned int windowHeight = 1800;
    
    mz::Maze maze(windowWidth, windowHeight);
    
    maze.lauch();
    
    return 0;
}
