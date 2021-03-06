//
//  CirclePhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef CirclePhysicsBody_hpp
#define CirclePhysicsBody_hpp

#include <optional>

#include "AABB.hpp"
#include "PhysicsBody.hpp"
#include "RectanglePhysicsBody.hpp"
#include "SegmentPhysicsBody.hpp"
#include "../System/MathsExt.hpp"

namespace mz {

class CirclePhysicsBody : public PhysicsBody {
public:
    CirclePhysicsBody(float radius, sf::Vector2f const& center, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    
    CirclePhysicsBody(CirclePhysicsBody const& body);
    
    virtual ~CirclePhysicsBody();
    
    virtual PhysicsBody* clone() const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(CirclePhysicsBody const& circle) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(RectanglePhysicsBody const& rectangle) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(SegmentPhysicsBody const& segment) const;
    
    virtual sf::Sprite const getBodySprite(sf::Vector2f const& anchor) const;
    
    virtual bool isCollidingWithAABB(AABB const& box) const;
    
    virtual bool isInsideAABB(AABB const& box) const;
    
    virtual bool isPositionInside(sf::Vector2f const& position) const;
    
    virtual void updateFrame();
    
private:
    float                                   m_radius;
    
    static std::optional<sf::RenderTexture> s_bodyTexture;
};

}

#endif /* CirclePhysicsBody_hpp */
