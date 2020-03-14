//
//  SegmentPhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef SegmentPhysicsBody_hpp
# define SegmentPhysicsBody_hpp

# include <optional>

# include "PhysicsBody.hpp"
# include "RectanglePhysicsBody.hpp"
# include "CirclePhysicsBody.hpp"
# include "../Utils/ExtraMaths.hpp"
# include "AABB.hpp"

namespace mz {

class SegmentPhysicsBody : public PhysicsBody {
public:
    SegmentPhysicsBody(sf::Vector2f const& startPos, sf::Vector2f const& endPos, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    SegmentPhysicsBody(sf::Vector2f const& startPos, sf::Vector2f const& direction, float length, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    
    SegmentPhysicsBody(SegmentPhysicsBody const& body);
    
    virtual ~SegmentPhysicsBody();
    
    virtual void updateFrame();
    
    virtual bool isInsideAABB(AABB const& box) const;
    virtual bool isCollidingWithAABB(AABB const& box) const;
    virtual bool isPositionInside(sf::Vector2f const& position) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body) const;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(SegmentPhysicsBody const& segment) const;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(CirclePhysicsBody const& circle) const;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(RectanglePhysicsBody const& rectangle) const;
    
    virtual PhysicsBody* clone() const;
    
    sf::Vector2f const& getStartPos() const;
    sf::Vector2f const& getEndPos() const;
    sf::Vector2f getVector() const;
    
    float getLength() const;
    float getLength2() const;
    
    virtual sf::Sprite const getBodySprite(sf::Vector2f const& anchor) const;
    
    static bool isCollisionBetweenSegments(sf::Vector2f const& pA, sf::Vector2f const& pB, sf::Vector2f const& p1, sf::Vector2f const& p2);
    static bool isCollisionBetweenSegments(sf::Vector2f const& pA, float lengthA, float angleA, sf::Vector2f const& p1, float length1, float angle1);
    static std::unique_ptr<std::vector<sf::Vector2f>> collisionBetweenSegments(sf::Vector2f const& pA, sf::Vector2f const& pB, sf::Vector2f const& p1, sf::Vector2f const& p2);
    static std::unique_ptr<std::vector<sf::Vector2f>> collisionBetweenSegments(sf::Vector2f pA, float lengthA, float angleA, sf::Vector2f p1, float length1, float angle1);
    
private:
    sf::Vector2f m_startPos;
    sf::Vector2f m_endPos;
    
    static std::optional<sf::RenderTexture> bodyTexture;
    
    static constexpr float DEBUG_SHAPE_WIDTH = 1.f;
};

}

#endif /* SegmentPhysicsBody_hpp */
