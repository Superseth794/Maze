//
//  PhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "PhysicsBody.hpp"

namespace mz {

bool isCollisionBetweenAABB(AABB const& b1, AABB const& b2) {
    return !(b1.origin.x > b2.getTopLeftCorner().x ||
             b1.getBottomRightCorner().x < b2.origin.x ||
             b1.origin.y > b2.getTopLeftCorner().y ||
             b1.getTopRightCorner().y < b2.origin.y);
}

PhysicsBody::PhysicsBody(sf::Vector2f const& center, PhysicsWorld* parentWorld) :
m_parentWorld(parentWorld),
m_center(center),
m_frame(AABB{{0.f, 0.f}, 0.f, 0.f})
{
}

AABB const& PhysicsBody::getFrame() const {
    return m_frame;
}

PhysicsBody::~PhysicsBody() {
    if (m_parentWorld)
        m_parentWorld->removeBody(this);
}

sf::Vector2f const& PhysicsBody::getCenter() const {
    return m_center;
}

void PhysicsBody::move(sf::Vector2f const& delta) {
    m_center.x += delta.x;
    m_center.y += delta.y;
}

void PhysicsBody::setCenter(sf::Vector2f const& center) {
    m_center = center;
}

PhysicsWorld* PhysicsBody::getParentWorld() const {
    return m_parentWorld;
}

std::shared_ptr<sf::RenderTexture> const PhysicsBody::getDebugTexture() {
    if (!m_debugTextureLoaded)
        generateDebugTexture();
    m_debugTextureLoaded = true;
    return m_debugTexture;
}

const sf::Color PhysicsBody::DEBUG_PHYSICS_FILL_COLOR = sf::Color(101, 184, 127, 125);
const sf::Color PhysicsBody::DEBUG_PHYSICS_OUTLINE_COLOR = sf::Color{58, 158, 65, 187};

}
