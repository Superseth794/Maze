//
//  SegmentPhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#include "SegmentPhysicsBody.hpp"

namespace mz {

SegmentPhysicsBody::SegmentPhysicsBody(sf::Vector2f const& startPos, sf::Vector2f const& endPos, PhysicsWorld* parentWorld) :
PhysicsBody(sf::Vector2f{(endPos.x + startPos.x) / 2.f, (endPos.y + startPos.y) / 2.f}, parentWorld),
m_startPos(startPos),
m_endPos(endPos)
{
    m_frame.width = std::abs(m_endPos.x - m_startPos.x);
    m_frame.height = std::abs(m_endPos.y - m_startPos.y);
    SegmentPhysicsBody::updateFrame();
}

SegmentPhysicsBody::SegmentPhysicsBody(sf::Vector2f const& startPos, sf::Vector2f const& direction, float length, PhysicsWorld* parentWorld) :
SegmentPhysicsBody(startPos, sf::Vector2f{startPos.x + normalize(direction).x * length, startPos.y + normalize(direction).y * length}, parentWorld)
{
}

void SegmentPhysicsBody::updateFrame() {
    sf::Vector2f origin {std::min(m_startPos.x, m_endPos.x), std::min(m_startPos.y, m_endPos.y)};
    if (m_frame.origin != origin)
        m_frame.origin = origin;
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWith(PhysicsBody* body) {
    return body->collideWithSegment(this);
}

bool SegmentPhysicsBody::isInsideAABB(AABB const& box) const {
    return (isPositionInsideAABB(box, m_startPos) &&
            isPositionInsideAABB(box, m_endPos));
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWithSegment(SegmentPhysicsBody* segment) {
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    sf::Vector2f AO {segment->getStartPos().x - m_startPos.x, segment->getStartPos().y - m_startPos.y};
    sf::Vector2f AP {segment->getEndPos().x - m_startPos.x, segment->getEndPos().y - m_startPos.y};
    
    if ((getVector().x * AP.y - getVector().y - AP.x) * (getVector().x * AO.y - getVector().y * AO.x) >= 0)
        return intersections;
    
    float div = getVector().x * segment->getVector().y - getVector().y * segment->getVector().x;
    if (div == 0.f) // Prevents division per 0
        return intersections;
    
    float k = -(m_startPos.x * segment->getVector().y - segment->m_startPos.x * segment->getVector().y - segment->getVector().x * m_startPos.y * segment->getVector().x * segment->m_startPos.y) / div;
    if (k < 0 || k > 1)
        return intersections;
    
    float l = (-getVector().x * m_startPos.y + getVector().x * segment->m_startPos.y + getVector().y * m_startPos.x - getVector().y * segment->m_startPos.x) / div;
    auto normalizedVect {normalize(getVector())};
    intersections->push_back({m_startPos.x + normalizedVect.x * l, m_startPos.y + normalizedVect.y * l});
    return intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWithCircle(CirclePhysicsBody* circle) {
    return circle->collideWithSegment(this);
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWithRectangle(RectanglePhysicsBody* rectangle) {
    return rectangle->collideWithSegment(this);
}

bool SegmentPhysicsBody::isPositionInside(sf::Vector2f const& position) const {
    float a = getVector().y;
    float b = -getVector().x;
    if (std::abs(position.y - (position.x * a + b)) > std::numeric_limits<float>::epsilon())
        return false;
    sf::Vector2f vect {position.x - m_startPos.x, position.y - m_startPos.y};
    float scalar = getVector().x * vect.x + getVector().y * vect.y;
    return (scalar < 0 || scalar * scalar > getLength2());
}

PhysicsBody* SegmentPhysicsBody::clone() const {
    return new SegmentPhysicsBody(*this);
}

sf::Vector2f const& SegmentPhysicsBody::getStartPos() const {
    return m_startPos;
}

sf::Vector2f const& SegmentPhysicsBody::getEndPos() const {
    return m_endPos;
}

sf::Vector2f SegmentPhysicsBody::getVector() const {
    return sf::Vector2f{m_endPos.x - m_startPos.x, m_endPos.y - m_startPos.y};
}

float SegmentPhysicsBody::getLength() const {
    return std::sqrt(getLength2());
}

float SegmentPhysicsBody::getLength2() const {
    return (m_endPos.x - m_startPos.x) * (m_endPos.x - m_startPos.x) + (m_endPos.y - m_startPos.y) * (m_endPos.y - m_startPos.y);
}

void SegmentPhysicsBody::generateDebugTexture() {
    m_debugTexture = std::make_shared<sf::RenderTexture>();
    
    float width = std::abs(m_endPos.x - m_startPos.x);
    float height = std::abs(m_endPos.y - m_startPos.y);
    sf::Vector2f anchor {std::min(m_startPos.x, m_endPos.x), std::min(m_startPos.y, m_endPos.y)};
    
    m_debugTexture->create(width, height);
    m_debugTexture->clear(sf::Color::Transparent);
    
    sf::Vertex vertexes[2] = {};
    vertexes[0].position = sf::Vector2f{m_startPos.x - anchor.x, m_startPos.y - anchor.y};
    vertexes[0].color = DEBUG_PHYSICS_OUTLINE_COLOR;
    vertexes[1].position = sf::Vector2f{m_endPos.x - anchor.x, m_endPos.y - anchor.y};
    vertexes[1].color = DEBUG_PHYSICS_OUTLINE_COLOR;
    m_debugTexture->draw(vertexes, 2, sf::Lines);
    
    m_debugTexture->display();
}

}
