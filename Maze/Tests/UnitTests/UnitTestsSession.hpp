//
//  UnitTestsSession.hpp
//  Maze
//
//  Created by Jaraxus on 15/03/2020.
//

#ifndef UnitTestsSession_hpp
# define UnitTestsSession_hpp

# include "../External/Catch/Catch.hpp"

class UnitTestsSession {
public:
    UnitTestsSession(std::string const& sessionName);
    
    int run();
    
private:
    std::string m_sessionName;
};

#endif /* UnitTestsSession_hpp */
