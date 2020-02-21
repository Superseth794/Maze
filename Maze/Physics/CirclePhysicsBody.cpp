//
//  CirclePhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#include "CirclePhysicsBody.hpp"

namespace mz {

CirclePhysicsBody::CirclePhysicsBody(float radius, sf::Vector2f const& center, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld) :
PhysicsBody(center, categoryBitMask, parentWorld),
m_radius(radius)
{
    CirclePhysicsBody::updateFrame();
    m_frame.height = 2 * m_radius;
    m_frame.width = 2 * m_radius;
}

CirclePhysicsBody::CirclePhysicsBody(CirclePhysicsBody const& body) :
PhysicsBody(body),
m_radius(body.m_radius)
{    
}

CirclePhysicsBody::~CirclePhysicsBody() {
    if (getParentWorld())
        getParentWorld()->removeBody(this);
}

void CirclePhysicsBody::updateFrame() {
    sf::Vector2f boxOrigin {getCenter().x - m_radius, getCenter().y - m_radius};
    if (boxOrigin != m_frame.origin)
        m_frame.origin = boxOrigin;
}

std::unique_ptr<std::vector<sf::Vector2f>> CirclePhysicsBody::collideWith(PhysicsBody* body) {
    return body->collideWithCircle(this);
}

bool CirclePhysicsBody::isInsideAABB(AABB const& box) const {
    return (isPositionInsideAABB(box, getCenter()) &&
            isPositionInsideAABB(box, sf::Vector2f{getCenter().x + m_radius, getCenter().y}) &&
            isPositionInsideAABB(box, sf::Vector2f{getCenter().x, getCenter().y + m_radius}) &&
            isPositionInsideAABB(box, sf::Vector2f{getCenter().x - m_radius, getCenter().y}) &&
            isPositionInsideAABB(box, sf::Vector2f{getCenter().x, getCenter().y - m_radius}));
}

std::unique_ptr<std::vector<sf::Vector2f>> CirclePhysicsBody::collideWithSegment(SegmentPhysicsBody* segment) {
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    
    sf::Vector2f AC {getCenter().x - segment->getStartPos().x, getCenter().y - segment->getStartPos().y};
    float num = segment->getVector().x * AC.y - segment->getVector().y * AC.x;
    if (num < 0)
        num = -num;
    float div = std::sqrt(segment->getVector().x * segment->getVector().x + segment->getVector().y * segment->getVector().y);
    float CI = num / div;
    
    if (CI >= m_radius)
        return intersections;
    
    sf::Vector2f BC {getCenter().x - segment->getEndPos().x, getCenter().y - segment->getEndPos().y};
    float scal1 = segment->getVector().x * AC.x + segment->getVector().y * AC.y;
    float scal2 = -segment->getVector().x * BC.x + -segment->getVector().y * BC.y;
    
    if ((scal1 < 0 || scal2 < 0) && !(isPositionInside(segment->getStartPos()) || isPositionInside(segment->getEndPos())))
        return intersections;
    
    float length2 = segment->getLength2();
    if (length2 == 0)
        return intersections;
    
    float scal = (segment->getVector().x * AC.x + segment->getVector().y * AC.y) / length2;
    intersections->push_back({segment->getStartPos().x + scal * segment->getVector().x, segment->getStartPos().y + scal * segment->getVector().y});
    
    return intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> CirclePhysicsBody::collideWithCircle(CirclePhysicsBody* circle) {
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    
    float dist2 = (circle->getCenter().x - getCenter().x) * (circle->getCenter().x - getCenter().x) + (circle->getCenter().y - getCenter().y) * (circle->getCenter().y - getCenter().y);
    
    if (dist2 > m_radius * m_radius + circle->m_radius * circle->m_radius)
        return intersections;
    
    if (circle->getCenter() == getCenter())
        return intersections;
    
    float a = ((circle->getCenter().x * circle->getCenter().x) + (circle->getCenter().y * circle->getCenter().y) - (getCenter().x * getCenter().x) - (getCenter().y * getCenter().y) + m_radius * m_radius - circle->m_radius * circle->m_radius) / (2 * (circle->getCenter().y - getCenter().y));
    float d = (circle->getCenter().x - getCenter().x) / (circle->getCenter().y - getCenter().y);
    
    float A = d * d + 1;
    float B = -2 * getCenter().x + 2 * getCenter().y * d - 2 * a * d;
    float C = getCenter().x * getCenter().x + getCenter().y * getCenter().y - 2 * getCenter().y * a + a * a - m_radius * m_radius;
    
    float deltaSqrt = std::sqrt(B * B - 4 * A * C);
    
    intersections->push_back({
        (-B + deltaSqrt) / (2 * A),
        a - d * (-B + deltaSqrt) / (2 * A)
    });
    intersections->push_back({
        (-B - deltaSqrt) / (2 * A),
        a - d * (-B - deltaSqrt) / (2 * A)
    });
    
    return intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> CirclePhysicsBody::collideWithRectangle(RectanglePhysicsBody* rectangle) {
    return rectangle->collideWithCircle(this);
}

bool CirclePhysicsBody::isPositionInside(sf::Vector2f const& position) const {
    float distance2 = (getCenter().x - position.x) * (getCenter().x - position.x) + (getCenter().y - position.y) * (getCenter().y - position.y);
    return distance2 <= m_radius * m_radius;
}

PhysicsBody* CirclePhysicsBody::clone() const {
    return new CirclePhysicsBody(*this);
}

void CirclePhysicsBody::generateDebugTexture() {
    m_debugTexture = std::make_shared<sf::RenderTexture>();
    m_debugTexture->create(m_radius * 2.f, m_radius * 2.f);
    m_debugTexture->clear(sf::Color::Transparent);
    
    sf::CircleShape debugShape{m_radius};
    debugShape.setFillColor(m_debugCollisionTriggered ? DEBUG_DID_COLLIDE_BODY_FILL_COLOR : DEBUG_PHYSICS_FILL_COLOR);
    debugShape.setPosition(0.f, 0.f);
    debugShape.setOutlineThickness(-3.f);
    debugShape.setOutlineColor(DEBUG_PHYSICS_OUTLINE_COLOR);
    m_debugTexture->draw(debugShape);
    
    m_debugTexture->display();
}

}
