//
//  Drawable.hpp
//  Maze
//
//  Created by Jaraxus on 31/07/2020.
//

#ifndef Drawable_hpp
#define Drawable_hpp

#include <SFML/Graphics.hpp>

namespace mz {

class Drawable {
public:
    Drawable() noexcept = default;
    Drawable(Drawable const& drawable) noexcept = default;
    Drawable(Drawable && drawable) noexcept = default;
    
    virtual ~Drawable() noexcept = default;
    
    Drawable& operator=(Drawable const& drawable) noexcept = default;
    Drawable& operator=(Drawable && drawable) noexcept = default;
    
    virtual void draw(sf::RenderTarget & target) = 0;
};

}

#endif /* Drawable_hpp */
