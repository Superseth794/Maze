//
//  Updatable.hpp
//  Maze
//
//  Created by Jaraxus on 31/07/2020.
//

#ifndef Updatable_hpp
#define Updatable_hpp

namespace mz {

class Updatable {
public:
    Updatable() = default;
    Updatable(Updatable const& updatable) = default;
    Updatable(Updatable && updatable) = default;
    
    virtual ~Updatable() = default;
    
    Updatable& operator=(Updatable const& updatable) = default;
    Updatable& operator=(Updatable && updatable) = default;
    
    virtual void update(std::uint64_t timeElapsed) = 0;
};

}

#endif /* Updatable_hpp */
