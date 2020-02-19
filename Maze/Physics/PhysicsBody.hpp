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
# include "AABB.hpp"

namespace mz {

class PhysicsWorld;

class SegmentPhysicsBody;
class CirclePhysicsBody;
class RectanglePhysicsBody;

struct QuadtreeNode;

class PhysicsBody {
public:
    PhysicsBody(sf::Vector2f const& center = sf::Vector2f{0.f, 0.f}, PhysicsWorld* parentWorld = nullptr);
    PhysicsBody(PhysicsBody && body) = default;
    
    virtual ~PhysicsBody() = 0;
    
    bool operator==(PhysicsBody* body) const;
    bool operator==(PhysicsBody const& body) const;
    bool operator!=(PhysicsBody* body) const;
    bool operator!=(PhysicsBody const& body) const;
    
    PhysicsBody(PhysicsBody const& body);
    PhysicsBody& operator=(PhysicsBody const& body) = default;
    
    AABB const& getFrame() const;
    virtual void updateFrame() = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body) = 0;
    
    virtual bool isInsideAABB(AABB const& box) const = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithSegment(SegmentPhysicsBody* segment) = 0;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithCircle(CirclePhysicsBody* circle) = 0;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWithRectangle(RectanglePhysicsBody* rectangle) = 0;
    
    virtual bool isPositionInside(sf::Vector2f const& position) const = 0;
    
    virtual PhysicsBody* clone() const = 0;
    
    sf::Vector2f const& getCenter() const;
    void move(sf::Vector2f const& delta);
    void setCenter(sf::Vector2f const& center);
    
    PhysicsWorld* getParentWorld() const;
    void updateInWorld();
    std::uint64_t getId() const;
    
    QuadtreeNode* getParentNode();
    void setParentNode(QuadtreeNode* parentNode);
    
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
    std::uint64_t m_id = 0;
    sf::Vector2f m_center;
    
    PhysicsWorld* m_parentWorld;
    QuadtreeNode* m_parentNode = nullptr;
};


}

#endif /* PhysicsBody_hpp */