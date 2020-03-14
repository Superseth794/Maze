//
//  main.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include <iostream>
#include <time.h>

#include "Maze.hpp"

#include "UnitTestsSession.hpp"

#define RUN_UNIT_TESTS

#ifndef RUN_UNIT_TESTS

int main(int argc, const char * argv[]) {
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    constexpr unsigned int windowWidth = 2800;
    constexpr unsigned int windowHeight = 1600;
    
    mz::Maze maze(windowWidth, windowHeight);
    
    maze.lauch();
    
    return 0;
}

#else

int main(int argc, const char * argv[]) {
    
    UnitTestsSession session {"My first unit tests session!"};
    
    int result = session.run();
    
    return result;
}

#endif
