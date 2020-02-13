//
//  RectanglePhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef RectanglePhysicsBody_hpp
# define RectanglePhysicsBody_hpp

# include <array>

# include "PhysicsBody.hpp"
# include "CirclePhysicsBody.hpp"
# include "SegmentPhysicsBody.hpp"

namespace mz {

class RectanglePhysicsBody : public PhysicsBody {
public:
    RectanglePhysicsBody(sf::Vector2f const& firstCorner, sf::Vector2f const& secondCorner);
    RectanglePhysicsBody(float width, float height, float rotation, sf::Vector2f const& origin);
    
    virtual void updateFrame();
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body);
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithSegment(SegmentPhysicsBody* segment);
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithCircle(CirclePhysicsBody* circle);
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithRectangle(RectanglePhysicsBody* rectangle);
    
    virtual bool isPositionInside(sf::Vector2f const& position) const;
    
    sf::Vector2f getTopLeftCorner() const;
    sf::Vector2f getTopRightCorner() const;
    sf::Vector2f getBottomRightCorner() const;
    sf::Vector2f getBottomLeftCorner() const;
    
private:
    sf::Vector2f m_firstCorner;
    sf::Vector2f m_secondCorner;
};

}

#endif /* RectanglePhysicsBody_hpp */
