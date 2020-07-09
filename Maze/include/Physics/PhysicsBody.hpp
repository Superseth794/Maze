//
//  PhysicsBody.hpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#ifndef PhysicsBody_hpp
# define PhysicsBody_hpp

# include <algorithm>
# include <cmath>
# include <optional>

# include <SFML/Graphics.hpp>

# include "AABB.hpp"
# include "PhysicsWorld.hpp"

namespace mz {

struct QuadtreeNode;

class CirclePhysicsBody;
class PhysicsWorld;
class RectanglePhysicsBody;
class SegmentPhysicsBody;

class PhysicsBody {
public:
    
    using Collision = std::pair<PhysicsBody*, std::unique_ptr<std::vector<sf::Vector2f>>>;
    using CollisionCallback = std::function<void(Collision const& collision)>;
    
public:
    PhysicsBody(sf::Vector2f const& center = sf::Vector2f{0.f, 0.f}, std::uint32_t categoryBitMask = 0, PhysicsWorld* parentWorld = nullptr);
    
    PhysicsBody(PhysicsBody const& body);
    
    PhysicsBody(PhysicsBody && body) = default;
    
    virtual ~PhysicsBody() = 0;
    
    bool operator==(PhysicsBody* body) const;
    
    bool operator==(PhysicsBody const& body) const;
    
    bool operator!=(PhysicsBody* body) const;
    
    bool operator!=(PhysicsBody const& body) const;
    
    PhysicsBody& operator=(PhysicsBody const& body) = default;
    
    void addContactTestBitMask(std::uint32_t bitMask);
    
    virtual PhysicsBody* clone() const = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(CirclePhysicsBody const& circle) const = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(PhysicsBody* body) const = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(RectanglePhysicsBody const& rectangle) const = 0;
    
    virtual std::unique_ptr<std::vector<sf::Vector2f>> collideWith(SegmentPhysicsBody const& segment) const = 0;
    
    void didCollide(Collision const& collision);
    
    sf::RectangleShape const& getAABBShape(sf::Vector2f const& anchor) const;
    
    virtual sf::Sprite const getBodySprite(sf::Vector2f const& anchor) const = 0;
    
    std::uint32_t getCategoryBitMask() const;
    
    sf::Vector2f const& getCenter() const;
    
    std::uint32_t getContactTestBitMasks() const;
    
    AABB const& getFrame() const;
    
    std::uint64_t getId() const;
    
//    sf::RectangleShape const& getOBBShape(); TODO
    
    QuadtreeNode* getParentNode() const;
    
    PhysicsWorld* getParentWorld() const;
    
    virtual bool isCollidingWithAABB(AABB const& box) const = 0;
    
    virtual bool isInsideAABB(AABB const& box) const = 0;
    
    virtual bool isPositionInside(sf::Vector2f const& position) const = 0;
    
    void move(sf::Vector2f const& delta);
    
    void removeContactTestBitMask(std::uint32_t bitMask);
    
    void setCenter(sf::Vector2f const& center);
    
    void setCollisionCallback(CollisionCallback const& callback);
    
    void setCollisionCallback(CollisionCallback && callback);
    
    void setParentNode(QuadtreeNode* parentNode);
    
    bool shouldTestCollisionWithBitMask(std::uint32_t bitMask) const;
    
    virtual void updateFrame() = 0;
    
    void updateInWorld();
    
protected:
    mutable bool                                m_didCollide                        = false;
    AABB                                        m_frame;
    
    static const sf::Color                      s_debugPhysicsFillColor;
    static const sf::Color                      s_debugPhysicsOutlineColor;
    static const sf::Color                      s_debugDidCollideFillColor;
    
private:
    std::uint32_t                               m_categoryBitMask;
    sf::Vector2f                                m_center;
    CollisionCallback                           m_collisionCallback;
    std::uint32_t                               m_contactTestBitMask;
    std::uint64_t                               m_id = 0;
    QuadtreeNode*                               m_parentNode                        = nullptr;
    PhysicsWorld*                               m_parentWorld;
    
    static std::optional<sf::RectangleShape>    s_AABBShape;
    static const sf::Color                      s_debugAABBFillColor;
    static const sf::Color                      s_debugAABBOutlineColor;
};

}

#endif /* PhysicsBody_hpp */
