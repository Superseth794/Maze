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

# include "PhysicsWorld.hpp"

namespace mz {

class PhysicsWorld;

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
    PhysicsBody(sf::Vector2f const& center = sf::Vector2f{0.f, 0.f}, PhysicsWorld* parentWorld = nullptr);
    PhysicsBody(PhysicsBody && body) = default;
    virtual ~PhysicsBody();
    
    PhysicsBody(PhysicsBody const& body) = default;
    PhysicsBody& operator=(PhysicsBody const& body) = default;
    
    AABB const& getFrame() const;
    virtual void updateFrame() = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body) = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithSegment(SegmentPhysicsBody* segment) = 0;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithCircle(CirclePhysicsBody* circle) = 0;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithRectangle(RectanglePhysicsBody* rectangle) = 0;
    
    virtual bool isPositionInside(sf::Vector2f const& position) const = 0;
    
    virtual PhysicsBody* clone() const = 0;
    
    sf::Vector2f const& getCenter() const;
    void move(sf::Vector2f const& delta);
    void setCenter(sf::Vector2f const& center);
    
    PhysicsWorld* getParentWorld() const;
    
    std::shared_ptr<sf::RenderTexture> const getDebugTexture();
    
protected:
    virtual void generateDebugTexture() = 0;
    
protected:
    AABB m_frame;
    
    std::shared_ptr<sf::RenderTexture> m_debugTexture;
    bool m_debugTextureLoaded = false;
    
    static const sf::Color DEBUG_PHYSICS_FILL_COLOR;
    static const sf::Color DEBUG_PHYSICS_OUTLINE_COLOR;
    
private:
    sf::Vector2f m_center;
    PhysicsWorld* m_parentWorld;
};


}

#endif /* PhysicsBody_hpp */
