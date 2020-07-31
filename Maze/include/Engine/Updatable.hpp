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
    Updatable() noexcept = default;
    Updatable(Updatable const& updatable) noexcept = default;
    Updatable(Updatable && updatable) noexcept = default;
    
    virtual ~Updatable() noexcept = default;
    
    Updatable& operator=(Updatable const& updatable) noexcept = default;
    Updatable& operator=(Updatable && updatable) noexcept = default;
    
    virtual void update(std::uint64_t timeElapsed) = 0;
};

}

#endif /* Updatable_hpp */
