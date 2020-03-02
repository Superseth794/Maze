//
//  SegmentPhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef SegmentPhysicsBody_hpp
# define SegmentPhysicsBody_hpp

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
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body);
    
    virtual bool isInsideAABB(AABB const& box) const;
    virtual bool isCollidingWithAABB(AABB const& box) const;
    virtual bool isPositionInside(sf::Vector2f const& position) const;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithSegment(SegmentPhysicsBody* segment);
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithCircle(CirclePhysicsBody* circle);
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithRectangle(RectanglePhysicsBody* rectangle);
    
    virtual PhysicsBody* clone() const;
    
    sf::Vector2f const& getStartPos() const;
    sf::Vector2f const& getEndPos() const;
    sf::Vector2f getVector() const;
    
    float getLength() const;
    float getLength2() const;
    
protected:
    virtual void generateDebugTexture();
    
private:
    sf::Vector2f m_startPos;
    sf::Vector2f m_endPos;
};

}

#endif /* SegmentPhysicsBody_hpp */
