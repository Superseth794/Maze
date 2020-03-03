//
//  SegmentPhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#include "SegmentPhysicsBody.hpp"

namespace mz {

SegmentPhysicsBody::SegmentPhysicsBody(sf::Vector2f const& startPos, sf::Vector2f const& endPos, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld) :
PhysicsBody(sf::Vector2f{(endPos.x + startPos.x) / 2.f, (endPos.y + startPos.y) / 2.f}, categoryBitMask, parentWorld),
m_startPos(startPos),
m_endPos(endPos)
{
    m_frame.width = std::abs(m_endPos.x - m_startPos.x);
    m_frame.height = std::abs(m_endPos.y - m_startPos.y);
    SegmentPhysicsBody::updateFrame();
}

SegmentPhysicsBody::SegmentPhysicsBody(sf::Vector2f const& startPos, sf::Vector2f const& direction, float length, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld) :
SegmentPhysicsBody(startPos, sf::Vector2f{startPos.x + normalize(direction).x * length, startPos.y + normalize(direction).y * length}, categoryBitMask, parentWorld)
{
}

SegmentPhysicsBody::SegmentPhysicsBody(SegmentPhysicsBody const& body) :
PhysicsBody(body),
m_startPos(body.m_startPos),
m_endPos(body.m_endPos)
{
}

SegmentPhysicsBody::~SegmentPhysicsBody() {
    if (getParentWorld())
        getParentWorld()->removeBody(this);
}

void SegmentPhysicsBody::updateFrame() {
    sf::Vector2f origin {std::min(m_startPos.x, m_endPos.x), std::min(m_startPos.y, m_endPos.y)};
    if (m_frame.origin != origin)
        m_frame.origin = origin;
}

bool SegmentPhysicsBody::isInsideAABB(AABB const& box) const {
    return (isPositionInsideAABB(box, m_startPos) &&
            isPositionInsideAABB(box, m_endPos));
}

bool SegmentPhysicsBody::isCollidingWithAABB(AABB const& box) const {
    std::cerr << "Warning !! bad collision computation !\n";
    return isInsideAABB(box); // TOFIX - TODO
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

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWith(PhysicsBody* body) const {
    return body->collideWith(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWith(SegmentPhysicsBody const& segment) const {
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    sf::Vector2f AO {segment.getStartPos().x - m_startPos.x, segment.getStartPos().y - m_startPos.y};
    sf::Vector2f AP {segment.getEndPos().x - m_startPos.x, segment.getEndPos().y - m_startPos.y};
    
    if ((getVector().x * AP.y - getVector().y - AP.x) * (getVector().x * AO.y - getVector().y * AO.x) >= 0)
        return intersections;
    
    float div = getVector().x * segment.getVector().y - getVector().y * segment.getVector().x;
    if (div == 0.f) // Prevents division per 0
        return intersections;
    
    float k = -(m_startPos.x * segment.getVector().y - segment.m_startPos.x * segment.getVector().y - segment.getVector().x * m_startPos.y * segment.getVector().x * segment.m_startPos.y) / div;
    if (k < 0 || k > 1)
        return intersections;
    
    float l = (-getVector().x * m_startPos.y + getVector().x * segment.m_startPos.y + getVector().y * m_startPos.x - getVector().y * segment.m_startPos.x) / div;
    auto normalizedVect {normalize(getVector())};
    intersections->push_back({m_startPos.x + normalizedVect.x * l, m_startPos.y + normalizedVect.y * l});
    return intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWith(CirclePhysicsBody const& circle) const {
    return circle.collideWith(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWith(RectanglePhysicsBody const& rectangle) const {
    return rectangle.collideWith(*this);
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

sf::Sprite const SegmentPhysicsBody::getBodySprite(sf::Vector2f const& anchor) const {
    if (!bodyTexture.has_value()) {
        bodyTexture.emplace();
        
        auto& texture = bodyTexture.value();
        texture.create(100.f, 100.f);
        texture.clear(sf::Color::Transparent);
        
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f{100.f, 10.f});
        shape.setFillColor(DEBUG_PHYSICS_OUTLINE_COLOR);
        shape.setPosition(0.f, 0.f);
        
        texture.draw(shape);
        texture.display();
    }
    
    float angle = std::acos((100.f * getVector().x) / (getLength() * 100 * 100));
    float scaleFactor = getLength2() / (100 * 100);
    
    sf::Sprite bodySprite;
    bodySprite.setTexture(bodyTexture.value().getTexture());
    bodySprite.setScale(scaleFactor, 1.f);
    bodySprite.setPosition(m_frame.origin.x - 5.f + anchor.x, m_frame.origin.y - 5.f + anchor.y);
    bodySprite.setRotation(angle);
    
    bodySprite.setColor(m_didCollide ? sf::Color::Red : sf::Color::White);
    m_didCollide = false;
    
    return bodySprite;
}

std::optional<sf::RenderTexture> SegmentPhysicsBody::bodyTexture = std::nullopt;

}
