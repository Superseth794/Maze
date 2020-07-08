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
# include "../System/ExtraMaths.hpp"

namespace mz {

class RectanglePhysicsBody : public PhysicsBody {
public:
    RectanglePhysicsBody(float width, float height, sf::Vector2f const& origin, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    
    RectanglePhysicsBody(sf::Vector2f const& firstCorner, sf::Vector2f const& secondCorner, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    
    RectanglePhysicsBody(RectanglePhysicsBody const& body);
    
    virtual ~RectanglePhysicsBody();
    
    virtual PhysicsBody* clone() const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(CirclePhysicsBody const& circle) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(RectanglePhysicsBody const& rectangle) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(SegmentPhysicsBody const& segment) const;
    
    sf::Vector2f getBottomLeftCorner() const;
    
    sf::Vector2f getBottomRightCorner() const;
    
    virtual sf::Sprite const getBodySprite(sf::Vector2f const& anchor) const;
    
    sf::Vector2f getTopLeftCorner() const;
    
    sf::Vector2f getTopRightCorner() const;
    
    virtual bool isCollidingWithAABB(AABB const& box) const;
    
    virtual bool isInsideAABB(AABB const& box) const;
    
    virtual bool isPositionInside(sf::Vector2f const& position) const;
    
    virtual void updateFrame();
    
private:
    float                                   m_heigth;
    float                                   m_width;
    
    static std::optional<sf::RenderTexture> bodyTexture;
};

}

#endif /* RectanglePhysicsBody_hpp */
