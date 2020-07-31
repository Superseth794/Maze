//
//  Drawable.hpp
//  Maze
//
//  Created by Jaraxus on 31/07/2020.
//

#ifndef Drawable_h
#define Drawable_h

#include <SFML/Graphics.hpp>

namespace mz {

class Drawable {
public:
    Drawable() = default;
    Drawable(Drawable const& drawable) = default;
    Drawable(Drawable && drawable) = default;
    
    virtual ~Drawable() = default;
    
    Drawable& operator=(Drawable const& drawable) = default;
    Drawable& operator=(Drawable && drawable) = default;
    
    virtual void draw(sf::RenderTarget & target) = 0;
};

}

#endif /* Drawable_h */
