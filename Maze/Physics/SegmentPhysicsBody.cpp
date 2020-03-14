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
    assert(m_frame.width == m_frame.height);
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
    std::cerr << "Warning !! bad segment - AABB collision computation !\n";
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
    return collisionBetweenSegments(m_startPos, m_endPos, segment.m_startPos, segment.m_endPos);
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
        texture.create(100.f, 10.f);
        texture.clear(sf::Color::Yellow);
        
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f{100.f, 10.f});
        shape.setFillColor(DEBUG_PHYSICS_OUTLINE_COLOR);
        shape.setPosition(0.f, 0.f);
        
        texture.draw(shape);
        texture.display();
    }
    
    float length = getLength();
    float angle = std::acos(getVector().x / length);
    float scaleFactor = length / 100;
    
    sf::Sprite bodySprite;
    bodySprite.setTexture(bodyTexture.value().getTexture());
    bodySprite.setScale(scaleFactor, 1.f);
    bodySprite.setPosition(m_startPos.x + anchor.x, m_startPos.y + anchor.y);
    bodySprite.setRotation(angle * 180.f / M_PI);
    
    bodySprite.setColor(m_didCollide ? sf::Color::Red : sf::Color::White);
    m_didCollide = false;
    
    return bodySprite;
}

bool SegmentPhysicsBody::isCollisionBetweenSegments(sf::Vector2f const& pA, sf::Vector2f const& pB, sf::Vector2f const& p1, sf::Vector2f const& p2) {
    sf::Vector2f AB {pB.x - pA.x, pB.y - pA.y};
    sf::Vector2f OP {p2.x - p1.x, p2.y - p1.y};
    sf::Vector2f AO {p1.x - pA.x, p1.y - pA.y};
    
    float scalar1 = AO.x * AB.x + AO.y * AB.y;
    float scalar2 = -AO.x * OP.x + -AO.y * OP.y;
    
    float lengthAB2 = AB.x * AB.x + AB.y * AB.y;
    float lengthOP2 = OP.x * OP.x + OP.y * OP.y;
    
    return ((scalar1 >= 0 && scalar1 * scalar1 <= lengthOP2) || (scalar2 >= 0 && scalar2 * scalar2 <= lengthAB2));
}

bool SegmentPhysicsBody::isCollisionBetweenSegments(sf::Vector2f const& pA, float lengthA, float angleA, sf::Vector2f const& p1, float length1, float angle1) {
    return isCollisionBetweenSegments(pA, sf::Vector2f{pA.x + std::cos(angleA), pA.y + std::sin(angleA)}, p1, sf::Vector2f{p1.x + std::cos(angle1), p1.y + std::sin(angle1)});
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collisionBetweenSegments(sf::Vector2f const& pA, sf::Vector2f const& pB, sf::Vector2f const& p1, sf::Vector2f const& p2) {
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    
    sf::Vector2f AO {p1.x - pA.x, p1.y - pA.y};
    sf::Vector2f AP {p1.x - pA.x, p1.y - pA.y};
    sf::Vector2f vectAB {pB.x - pA.x, pB.y - pA.y};
    sf::Vector2f vect12 {p2.x - p1.x, p2.y - p1.y};
    
    if ((vectAB.x * AP.y - vectAB.y - AP.x) * (vectAB.x * AO.y - vectAB.y * AO.x) >= 0)
        return intersections;
    
    float div = vectAB.x * vect12.y - vectAB.y * vect12.x;
    if (div == 0.f) // Prevents division per 0
        return intersections;
    
    float k = -(pA.x * vect12.y - p1.x * vect12.y - vect12.x * pA.y * vect12.x * p1.y) / div;
    if (k < 0 || k > 1)
        return intersections;
    
    float l = (-vectAB.x * pA.y + vectAB.x * p1.y + vectAB.y * pA.x - vectAB.y * p1.x) / div;
    auto normalizedVect {normalize(vectAB)};
    intersections->push_back({pA.x + normalizedVect.x * l, pA.y + normalizedVect.y * l});
    return intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collisionBetweenSegments(sf::Vector2f pA, float lengthA, float angleA, sf::Vector2f p1, float length1, float angle1) {
    return collisionBetweenSegments(pA, sf::Vector2f{pA.x + std::cos(lengthA), pA.y + std::sin(lengthA)}, p1, sf::Vector2f{p1.x + std::cos(length1), p1.y + std::sin(length1)});
}

std::optional<sf::RenderTexture> SegmentPhysicsBody::bodyTexture = std::nullopt;

}
