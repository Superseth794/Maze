//
//  RectanglePhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef RectanglePhysicsBody_hpp
# define RectanglePhysicsBody_hpp

# include <array>
# include <optional>

# include "PhysicsBody.hpp"
# include "CirclePhysicsBody.hpp"
# include "SegmentPhysicsBody.hpp"
# include "AABB.hpp"
# include "../Utils/ExtraMaths.hpp"

namespace mz {

class RectanglePhysicsBody : public PhysicsBody {
public:
    RectanglePhysicsBody(float width, float height, float rotation, sf::Vector2f const& origin, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    RectanglePhysicsBody(sf::Vector2f const& firstCorner, sf::Vector2f const& secondCorner, float rotation, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    
    RectanglePhysicsBody(RectanglePhysicsBody const& body);
    
    virtual ~RectanglePhysicsBody();
    
    virtual void updateFrame();
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body);
    
    virtual bool isInsideAABB(AABB const& box) const;
    virtual bool isCollidingWithAABB(AABB const& box) const;
    virtual bool isPositionInside(sf::Vector2f const& position) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithSegment(SegmentPhysicsBody* segment);
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithCircle(CirclePhysicsBody* circle);
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithRectangle(RectanglePhysicsBody* rectangle);
    
    virtual PhysicsBody* clone() const;
    
    sf::Vector2f getTopLeftCorner() const;
    sf::Vector2f getTopRightCorner() const;
    sf::Vector2f getBottomRightCorner() const;
    sf::Vector2f getBottomLeftCorner() const;
    
    float getRotation() const;
    
    virtual sf::Sprite const getBodySprite(sf::Vector2f const& anchor) const;
    
private:
    float m_width;
    float m_heigth;
    float m_rotation;
    
    static std::optional<sf::RenderTexture> bodyTexture;
};

}

#endif /* RectanglePhysicsBody_hpp */
