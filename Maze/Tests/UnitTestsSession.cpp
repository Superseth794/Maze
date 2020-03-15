//
//  UnitTestsSession.cpp
//  Maze
//
//  Created by Jaraxus on 15/03/2020.
//

#define CATCH_CONFIG_RUNNER
#include "UnitTestsSession.hpp"

UnitTestsSession::UnitTestsSession(std::string const& sessionName) :
m_sessionName(sessionName)
{
}

int UnitTestsSession::run() {
    int argc = 1;
    auto argv = m_sessionName.c_str();
    
    int result = Catch::Session().run(argc, &argv);
    
    return result;
}
