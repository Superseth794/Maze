//
//  VisualTestsSession.cpp
//  Maze
//
//  Created by Jaraxus on 01/07/2020.
//

#include "VisualTestsSession.hpp"

mz::Test::VisualTestsSession::VisualTestsSession(std::string && sessionName) :
m_sessionName(std::forward<std::string>(sessionName))
{
//    std::filesystem::create_directory(""); TODO: create cross-platform system
}

int mz::Test::VisualTestsSession::run() {
    bool totalSucces = true;
    
    auto runSubSession = [&totalSucces](std::function<bool()> const& subSessionFunc) {
        std::cout << "------------------------------------------------------------\n";
        
        bool result = subSessionFunc();
        
        std::cout << "\nsubsession " << (result ? "passed" : "failed") << "\n";
        std::cout << "------------------------------------------------------------\n" << std::endl;
        
        totalSucces &= result;
    };
    
    std::cout << "-------------------------------------------------------------------------------------------------\n\n";
    std::cout << m_sessionName << " :\n\n";
    std::cout << "-------------------------------------------------------------------------------------------------\n\n" << std::endl;
    
    runSubSession(VisualTestsSession::runPositionsInsideShapesTests);
    
    if (totalSucces)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

inline void mz::Test::VisualTestsSession::displayTestTitle(std::string const& title) {
    std::cout << title << "\n------------------------\n\n";
}

bool mz::Test::VisualTestsSession::runTest(std::function<void()> const& testFunction) {
    try {
        testFunction();
    }
    catch(std::runtime_error const& e) {
        std::cout << "error: " << e.what();
        return false;
    }
    return true;
}

bool mz::Test::VisualTestsSession::runPositionsInsideShapesTests() {
    displayTestTitle("Positions inside shapes tests");
    return (
        runTest(positionInsideSegmentTest) &&
        runTest(positionInsideRayTest) &&
        runTest(positionInsideLineTest) &&
        runTest(positionInsideCircleTest) &&
        runTest(positionInsideTriangleTest) &&
        runTest(positionInsideAABBTest) &&
        runTest(positionInsideOOBBTest)
    );
}
