//
//  VisualTestsSession.hpp
//  Maze
//
//  Created by Jaraxus on 01/07/2020.
//

#ifndef VisualTestsSession_hpp
# define VisualTestsSession_hpp

# include <iostream>
# include <stdexcept>
# include <stdio.h>
# include <functional>
# include <string>

# include "Physics/PositionInsideShapes.hpp"

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

namespace mz::Test {

class VisualTestsSession {
public:
    VisualTestsSession(std::string && sessionName);
    int run();
    
private:
    static inline void displayTestTitle(std::string const& title);
    
    static bool runTest(std::function<void()> const& testFunction);
    
    static bool runPositionsInsideShapesTests(); // TODO: add other tests
    
private:
    std::string m_sessionName;
};

}

#endif /* VisualTestsSession_hpp */
