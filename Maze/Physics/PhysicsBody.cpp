//
//  PhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "PhysicsBody.hpp"

namespace mz {

PhysicsBody::PhysicsBody(sf::Vector2f const& center, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld) :
m_center(center),
m_frame(AABB{{0.f, 0.f}, 0.f, 0.f}),
m_categoryBitMask(categoryBitMask),
m_parentWorld(parentWorld),
m_collisionCallback([](Collision const& Collision){})
{
    if (parentWorld)
        m_id = parentWorld->generateBodyId();
}

bool PhysicsBody::operator==(PhysicsBody* body) const {
    return m_id == body->m_id;
}

bool PhysicsBody::operator==(PhysicsBody const& body) const {
    return m_id == body.m_id;
}

bool PhysicsBody::operator!=(PhysicsBody* body) const {
    return m_id != body->m_id;
}

bool PhysicsBody::operator!=(PhysicsBody const& body) const {
    return m_id != body.m_id;
}

PhysicsBody::PhysicsBody(PhysicsBody const& body) :
m_frame(body.m_frame),
m_center(body.m_center),
m_id(0),
m_categoryBitMask(body.m_categoryBitMask),
m_contactTestBitMask(body.m_contactTestBitMask),
m_parentWorld(body.m_parentWorld),
m_collisionCallback([](Collision const& Collision){})
{
    if (m_parentWorld)
        m_id = m_parentWorld->generateBodyId();
}

PhysicsBody::~PhysicsBody() {
    m_parentWorld = nullptr;
}

AABB const& PhysicsBody::getFrame() const {
    return m_frame;
}

void PhysicsBody::didCollide(Collision const& collision) {
    m_didCollide = true;
    m_collisionCallback(collision);
}

sf::Vector2f const& PhysicsBody::getCenter() const {
    return m_center;
}

void PhysicsBody::move(sf::Vector2f const& delta) {
    m_center.x += delta.x;
    m_center.y += delta.y;
    updateFrame();
}

void PhysicsBody::setCenter(sf::Vector2f const& center) {
    m_center = center;
    updateFrame();
}

PhysicsWorld* PhysicsBody::getParentWorld() const {
    return m_parentWorld;
}

std::uint64_t PhysicsBody::getId() const {
    return m_id;
}

QuadtreeNode* PhysicsBody::getParentNode() const {
    return m_parentNode;
}

void PhysicsBody::updateInWorld() {
    if (m_parentWorld)
        m_parentWorld->updateBody(this);
}

void PhysicsBody::setParentNode(QuadtreeNode* parentNode) {
    assert(parentNode);
    m_parentNode = parentNode;
}

void PhysicsBody::setCollisionCallback(CollisionCallback const& callback) {
    m_collisionCallback = callback;
}

void PhysicsBody::setCollisionCallback(CollisionCallback && callback) {
    m_collisionCallback = std::move(callback);
}

std::uint32_t PhysicsBody::getCategoryBitMask() const {
    return m_categoryBitMask;
}

void PhysicsBody::addContactTestBitMask(std::uint32_t bitMask) {
    m_contactTestBitMask |= bitMask;
}

void PhysicsBody::removeContactTestBitMask(std::uint32_t bitMask) {
    m_contactTestBitMask &= ~bitMask;
}

std::uint32_t PhysicsBody::getContactTestBitMasks() const {
    return m_contactTestBitMask;
}

bool PhysicsBody::shouldTestCollisionWithBitMask(std::uint32_t bitMask) const {
    return (m_contactTestBitMask & bitMask) != 0b0;
}

sf::RectangleShape const& PhysicsBody::getAABBShape(sf::Vector2f const& anchor) const {
    if (!AABBShape.has_value()) {
        AABBShape.emplace();
        
        auto& shape = AABBShape.value();
        
        shape.setSize(sf::Vector2f{100.f, 100.f});
        shape.setFillColor(DEBUG_AABB_FILL_COLOR);
        shape.setOutlineColor(DEBUG_AABB_OUTLINE_COLOR);
        shape.setOutlineThickness(-2.f);
        shape.setPosition(0.f, 0.f);
    }
    
    sf::Vector2f scaleFactors {
        m_frame.width / 100.f,
        m_frame.height / 100.f
    };
    
    AABBShape.value().setScale(scaleFactors);
    AABBShape.value().setPosition(m_frame.origin.x + anchor.x, m_frame.origin.y + anchor.y);
    
    return AABBShape.value();
}

std::optional<sf::RectangleShape> PhysicsBody::AABBShape = std::nullopt;

const sf::Color PhysicsBody::DEBUG_PHYSICS_FILL_COLOR = sf::Color(56, 128, 78, 175); // 18, 217, 4
const sf::Color PhysicsBody::DEBUG_PHYSICS_OUTLINE_COLOR = sf::Color{22, 64, 25, 187};
const sf::Color PhysicsBody::DEBUG_DID_COLLIDE_BODY_FILL_COLOR = sf::Color{255, 0, 0, 135};

const sf::Color PhysicsBody::DEBUG_AABB_FILL_COLOR = sf::Color(3, 28, 252, 160);
const sf::Color PhysicsBody::DEBUG_AABB_OUTLINE_COLOR = sf::Color(52, 216, 235, 200);

}
