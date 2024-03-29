//
//  main.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include <iostream>
#include <time.h>

//#define RUN_UNIT_TESTS
//#define RUN_VISUAL_TESTS

#include "include/Core/Logs.hpp"

mz::Logs mz::Logs::Global(std::cerr, true);

#if !defined(RUN_UNIT_TESTS)

#   include "include/Game/Maze.hpp"

#include "examples/ActionsDemo.hpp"

int main(int /*argc*/, const char ** /*argv*/) {
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    constexpr unsigned int windowWidth  = 2800;
    constexpr unsigned int windowHeight = 1600;
    
//    mz::Maze maze(windowWidth, windowHeight);
//    maze.lauch();
    
    ActionsDemo demonstration(windowWidth, windowHeight);
    demonstration.run();
    
    return 0;
}

#elif !defined(RUN_VISUAL_TESTS)

#   include "tests/UnitTests/UnitTestsSession.hpp"

int main(int argc, const char * argv[]) {
    UnitTestsSession session {"Unit tests session"};
    return session.run();
}

#else

#   include "tests/VisualTests/VisualTestsSession.hpp"

int main(int argc, const char * argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));
    mz::Test::VisualTestsSession session {"Visual tests session"};
    return session.run();
}

#endif // Visual Tests
