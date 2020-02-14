//
//  CirclePhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef CirclePhysicsBody_hpp
# define CirclePhysicsBody_hpp

# include "PhysicsBody.hpp"
# include "RectanglePhysicsBody.hpp"
# include "SegmentPhysicsBody.hpp"
# include "../Utils/ExtraMaths.hpp"

namespace mz {

class CirclePhysicsBody : public PhysicsBody {
public:
    CirclePhysicsBody(float radius, sf::Vector2f const& center, PhysicsWorld* parentWorld = nullptr);
    
    CirclePhysicsBody(CirclePhysicsBody const& body) = default;
    
    virtual void updateFrame();
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body);
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithSegment(SegmentPhysicsBody* segment);
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithCircle(CirclePhysicsBody* circle);
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithRectangle(RectanglePhysicsBody* rectangle);
    
    virtual bool isPositionInside(sf::Vector2f const& position) const;
    
    virtual PhysicsBody* clone() const;
    
protected:
    virtual void generateDebugTexture();
    
private:
    float m_radius;
};

}

#endif /* CirclePhysicsBody_hpp */
