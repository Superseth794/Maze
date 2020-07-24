//
//  SegmentPhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef SegmentPhysicsBody_hpp
# define SegmentPhysicsBody_hpp

# include <optional>

# include "AABB.hpp"
# include "CirclePhysicsBody.hpp"
# include "PhysicsBody.hpp"
# include "RectanglePhysicsBody.hpp"
# include "../System/MathsExtr.hpp"

namespace mz {

class SegmentPhysicsBody : public PhysicsBody {
public:
    SegmentPhysicsBody(sf::Vector2f const& startPos, sf::Vector2f const& endPos, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    
    SegmentPhysicsBody(sf::Vector2f const& startPos, sf::Vector2f const& direction, float length, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    
    SegmentPhysicsBody(SegmentPhysicsBody const& body);
    
    virtual ~SegmentPhysicsBody();
    
    virtual PhysicsBody* clone() const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(CirclePhysicsBody const& circle) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(RectanglePhysicsBody const& rectangle) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(SegmentPhysicsBody const& segment) const;
    
    virtual sf::Sprite const getBodySprite(sf::Vector2f const& anchor) const;
    
    sf::Vector2f const& getEndPos() const;
    
    float getLength() const;
    
    float getLength2() const;
    
    sf::Vector2f const& getStartPos() const;
    
    sf::Vector2f getVector() const;
    
    static bool isCollisionBetweenSegments(sf::Vector2f const& pA, sf::Vector2f const& pB, sf::Vector2f const& p1, sf::Vector2f const& p2);
    
    static bool isCollisionBetweenSegments(sf::Vector2f const& pA, float lengthA, float angleA, sf::Vector2f const& p1, float length1, float angle1);
    
    virtual bool isCollidingWithAABB(AABB const& box) const;
    
    virtual bool isInsideAABB(AABB const& box) const;
    
    virtual bool isPositionInside(sf::Vector2f const& position) const;
    
    virtual void updateFrame();
    
    static std::unique_ptr<std::vector<sf::Vector2f>> collisionBetweenSegments(sf::Vector2f const& pA, sf::Vector2f const& pB, sf::Vector2f const& p1, sf::Vector2f const& p2);
    
    static std::unique_ptr<std::vector<sf::Vector2f>> collisionBetweenSegments(sf::Vector2f pA, float lengthA, float angleA, sf::Vector2f p1, float length1, float angle1);
    
private:
    sf::Vector2f                            m_endPos;
    sf::Vector2f                            m_startPos;
    
    static std::optional<sf::RenderTexture> s_bodyTexture;
    static constexpr float                  s_debugShapeWidth = 6.f; // TODO: remove debug from name
};

}

#endif /* SegmentPhysicsBody_hpp */
