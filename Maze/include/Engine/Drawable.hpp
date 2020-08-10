//
//  Drawable.hpp
//  Maze
//
//  Created by Jaraxus on 31/07/2020.
//

#ifndef Drawable_hpp
#define Drawable_hpp

#include <SFML/Graphics.hpp>

#include "Camera.hpp"

namespace mz {

class Camera;

class Drawable {
public:
    Drawable() noexcept = default;
    Drawable(Drawable const& drawable) noexcept = default;
    Drawable(Drawable && drawable) noexcept = default;
    
    virtual ~Drawable() noexcept = default;
    
    Drawable& operator=(Drawable const& drawable) noexcept = default;
    Drawable& operator=(Drawable && drawable) noexcept = default;
    
    virtual void draw(Camera const& camera) = 0;
};

}

#endif /* Drawable_hpp */
