//
//  AABB.hpp
//  Maze
//
//  Created by Jaraxus on 15/02/2020.
//

#ifndef AABB_hpp
#define AABB_hpp

#include <SFML/Graphics.hpp>

namespace mz {

struct AABB {
    float           height;
    sf::Vector2f    origin;
    float           width;
    
    AABB(sf::Vector2f const& _origin, float _width, float _height) : height(_height), origin(_origin), width(_width) {};
    
    bool operator==(AABB const& box) {
        return (origin == box.origin && width == box.width && height == box.height);
    }
    
    sf::Vector2f getBottomLeftCorner() const {
        return sf::Vector2f{origin.x, origin.y + height};
    }
    
    sf::Vector2f getBottomRightCorner() const {
        return sf::Vector2f{origin.x + width, origin.y + height};
    }
    
    sf::Vector2f getTopLeftCorner() const {
        return sf::Vector2f{origin.x, origin.y};
    }
    
    sf::Vector2f getTopRightCorner() const {
        return sf::Vector2f{origin.x + width, origin.y};
    }
};


inline bool isCollisionBetweenAABB(AABB const& b1, AABB const& b2) {
    return !(b1.origin.x > b2.getBottomRightCorner().x ||
             b1.origin.y > b2.getBottomRightCorner().y ||
             b1.getBottomRightCorner().x < b2.origin.x ||
             b1.getBottomRightCorner().y < b2.origin.y);
}

inline bool isPositionInsideAABB(AABB const& box, sf::Vector2f const& position) {
    return !(position.x < box.getTopLeftCorner().x ||
             position.x > box.getBottomRightCorner().x ||
             position.y < box.getTopLeftCorner().y ||
             position.y > box.getBottomRightCorner().y);
}

}

#endif /* AABB_hpp */
