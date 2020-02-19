//
//  PhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "PhysicsBody.hpp"

namespace mz {

PhysicsBody::PhysicsBody(sf::Vector2f const& center, PhysicsWorld* parentWorld) :
m_parentWorld(parentWorld),
m_center(center),
m_frame(AABB{{0.f, 0.f}, 0.f, 0.f})
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
m_parentWorld(body.m_parentWorld),
m_debugTexture(body.m_debugTexture),
m_debugTextureLoaded(body.m_debugTextureLoaded)
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

QuadtreeNode* PhysicsBody::getParentNode() {
    return m_parentNode;
}

void PhysicsBody::setParentNode(QuadtreeNode* parentNode) {
    assert(parentNode);
    m_parentNode = parentNode;
}

void PhysicsBody::updateInWorld() {
    if (m_parentWorld)
        m_parentWorld->updateBody(this);
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