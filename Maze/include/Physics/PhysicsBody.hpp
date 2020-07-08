//
//  PhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#ifndef PhysicsBody_hpp
# define PhysicsBody_hpp

# include <cmath>
# include <algorithm>
# include <optional>

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
    
    using Collision = std::pair<PhysicsBody*, std::unique_ptr<std::vector<sf::Vector2f>>>;
    using CollisionCallback = std::function<void(Collision const& collision)>;
    
public:
    PhysicsBody(sf::Vector2f const& center = sf::Vector2f{0.f, 0.f}, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
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
    
    virtual bool isInsideAABB(AABB const& box) const = 0;
    virtual bool isCollidingWithAABB(AABB const& box) const = 0;
    virtual bool isPositionInside(sf::Vector2f const& position) const = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body) const = 0;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(SegmentPhysicsBody const& segment) const = 0;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(CirclePhysicsBody const& circle) const = 0;
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(RectanglePhysicsBody const& rectangle) const = 0;
    
    virtual PhysicsBody* clone() const = 0;
    
    void didCollide(Collision const& collision);
    
    sf::Vector2f const& getCenter() const;
    void move(sf::Vector2f const& delta);
    void setCenter(sf::Vector2f const& center);
    
    PhysicsWorld* getParentWorld() const;
    void updateInWorld();
    std::uint64_t getId() const;
    
    QuadtreeNode* getParentNode() const;
    void setParentNode(QuadtreeNode* parentNode);
    
    void setCollisionCallback(CollisionCallback const& callback);
    void setCollisionCallback(CollisionCallback && callback);
    
    std::uint32_t getCategoryBitMask() const;
    void addContactTestBitMask(std::uint32_t bitMask);
    void removeContactTestBitMask(std::uint32_t bitMask);
    std::uint32_t getContactTestBitMasks() const;
    bool shouldTestCollisionWithBitMask(std::uint32_t bitMask) const;
    
    sf::RectangleShape const& getAABBShape(sf::Vector2f const& anchor) const;
//    sf::RectangleShape const& getOBBSprite(); TODO
    virtual sf::Sprite const getBodySprite(sf::Vector2f const& anchor) const = 0;
    
protected:
    AABB m_frame;
    
    mutable bool m_didCollide = false;
    
    static const sf::Color DEBUG_PHYSICS_FILL_COLOR;
    static const sf::Color DEBUG_PHYSICS_OUTLINE_COLOR;
    static const sf::Color DEBUG_DID_COLLIDE_BODY_FILL_COLOR;
    
private:
    std::uint64_t m_id = 0;
    sf::Vector2f m_center;
    
    PhysicsWorld* m_parentWorld;
    QuadtreeNode* m_parentNode = nullptr;
    
    std::uint32_t m_categoryBitMask;
    std::uint32_t m_contactTestBitMask;
    
    CollisionCallback m_collisionCallback;
    
    static std::optional<sf::RectangleShape> AABBShape;
    
    static const sf::Color DEBUG_AABB_FILL_COLOR;
    static const sf::Color DEBUG_AABB_OUTLINE_COLOR;
    
};


}

#endif /* PhysicsBody_hpp */
