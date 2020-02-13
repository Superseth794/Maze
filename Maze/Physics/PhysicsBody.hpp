//
//  PhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#ifndef PhysicsBody_hpp
# define PhysicsBody_hpp

# include <cmath>

# include <SFML/Graphics.hpp>

namespace mz {

class SegmentPhysicsBody;
class CirclePhysicsBody;
class RectanglePhysicsBody;

struct AABB {
    sf::Vector2f origin;
    float width;
    float height;
    
    sf::Vector2f getTopLeftCorner() const {
        return sf::Vector2f{origin.x, origin.y};
    }
    
    sf::Vector2f getTopRightCorner() const {
        return sf::Vector2f{origin.x + width, origin.y};
    }
    
    sf::Vector2f getBottomRightCorner() const {
        return sf::Vector2f{origin.x + width, origin.y + height};
    }
    
    sf::Vector2f getBottomLeftCorner() const {
        return sf::Vector2f{origin.x, origin.y + height};
    }
};

bool isCollisionBetweenAABB(AABB const& b1, AABB const& b2);

class PhysicsBody {
public:
    PhysicsBody(sf::Vector2f const& center = sf::Vector2f{0.f, 0.f});
    PhysicsBody(PhysicsBody && body) = default;
    virtual ~PhysicsBody() = default;
    
    PhysicsBody(PhysicsBody const& body) = delete;
    PhysicsBody& operator=(PhysicsBody const& body) = delete;
    
    AABB const& getFrame();
    virtual void updateFrame() = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body) = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithSegment(SegmentPhysicsBody* segment) = 0;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithCircle(CirclePhysicsBody* circle) = 0;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithRectangle(RectanglePhysicsBody* rectangle) = 0;
    
    virtual bool isPositionInside(sf::Vector2f const& position) const = 0;
    
    sf::Vector2f const& getCenter() const;
    void move(sf::Vector2f const& delta);
    void setCenter(sf::Vector2f const& center);
    
protected:
    AABB m_frame;
    
private:
    sf::Vector2f m_center;
};


}

#endif /* PhysicsBody_hpp */
