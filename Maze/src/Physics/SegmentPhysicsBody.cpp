//
//  SegmentPhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#include "../../include/Physics/SegmentPhysicsBody.hpp"

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
SegmentPhysicsBody(startPos, sf::Vector2f{startPos.x + normalizeVector(direction).x * length, startPos.y + normalizeVector(direction).y * length}, categoryBitMask, parentWorld)
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

PhysicsBody* SegmentPhysicsBody::clone() const {
    return new SegmentPhysicsBody(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWith(PhysicsBody* body) const {
    return body->collideWith(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWith(CirclePhysicsBody const& circle) const {
    return circle.collideWith(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWith(RectanglePhysicsBody const& rectangle) const {
    return rectangle.collideWith(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collideWith(SegmentPhysicsBody const& segment) const {
    return collisionBetweenSegments(m_startPos, m_endPos, segment.m_startPos, segment.m_endPos);
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collisionBetweenSegments(sf::Vector2f const& pA, sf::Vector2f const& pB, sf::Vector2f const& p1, sf::Vector2f const& p2) {
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    
    if (pA.x == pB.x || p1.x == p2.x)
        return intersections;
    
    sf::Vector2f AB {pB - pA};
    
    float aAB = (pB.y - pA.y) / (pB.x - pA.x);
    float bAB = pA.y - aAB * pA.x;
    float aOP = (p2.y - p1.y) / (p2.x - p1.x);
    float bOP = p1.y - aOP * p1.x;
    
    if (aAB == aOP)
        return intersections;
    
    float x = (bOP - bAB) / (aAB - aOP);
    float y = aAB * x + bAB;
    
    sf::Vector2f AX {x - pA.x, y - pA.y};
    float scalar = AX.x * AB.y - AX.y * AB.x;
    float length = getVectorLength(AB);
    
    if (scalar < 0 || scalar > length)
        return intersections;
    
    intersections->emplace_back(x, y);
    return  intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> SegmentPhysicsBody::collisionBetweenSegments(sf::Vector2f pA, float lengthA, float angleA, sf::Vector2f p1, float length1, float angle1) {
    return collisionBetweenSegments(pA, sf::Vector2f{pA.x + std::cos(lengthA), pA.y + std::sin(lengthA)}, p1, sf::Vector2f{p1.x + std::cos(length1), p1.y + std::sin(length1)});
}

bool SegmentPhysicsBody::isCollidingWithAABB(AABB const& box) const {
    // TODO update computation : here -> bad segment - AABB collision computation
    return isInsideAABB(box); // TOFIX - TODO
}

bool SegmentPhysicsBody::isCollisionBetweenSegments(sf::Vector2f const& pA, sf::Vector2f const& pB, sf::Vector2f const& p1, sf::Vector2f const& p2) {
    sf::Vector2f AB {pB - pA};
    sf::Vector2f OP {p2 - p1};
    sf::Vector2f AO {p1 - pA};
    
    float scalar1 = AO.x * AB.x + AO.y * AB.y;
    float scalar2 = -AO.x * OP.x + -AO.y * OP.y;
    
    float lengthAB2 = getVectorLength2(AB);
    float lengthOP2 = getVectorLength2(OP);
    
//    return (scalar1 >= 0 && scalar2 >= 0);
    std::cout << "isCollisionBetweenSegments --> TOFIX\n";
    return (true); // TOFIX
}

bool SegmentPhysicsBody::isCollisionBetweenSegments(sf::Vector2f const& pA, float lengthA, float angleA, sf::Vector2f const& p1, float length1, float angle1) {
    return isCollisionBetweenSegments(pA, sf::Vector2f{pA.x + std::cos(angleA), pA.y + std::sin(angleA)}, p1, sf::Vector2f{p1.x + std::cos(angle1), p1.y + std::sin(angle1)});
}

bool SegmentPhysicsBody::isInsideAABB(AABB const& box) const {
    return (isPositionInsideAABB(box, m_startPos) &&
            isPositionInsideAABB(box, m_endPos));
}

bool SegmentPhysicsBody::isPositionInside(sf::Vector2f const& position) const {
    const sf::Vector2f vect {getVector()};
    
    const float coef1 = (position.x - m_startPos.x) / vect.x;
    const float coef2 = (position.y - m_startPos.y) / vect.y;
    
    if (!nearlyEquals(coef1, coef2))
        return false;
    
    float scalar = getVector().x * vect.x + getVector().y * vect.y;
    return (scalar < 0 || scalar * scalar > getLength2());
    
    const sf::Vector2f vect1 {position - m_startPos};
    const sf::Vector2f vect2 {position - m_endPos};
    
    const float scalar1 = vect.x * vect1.x + vect.y * vect1.y;
    const float scalar2 = -vect.x * vect2.x + -vect.y * vect2.y;
    
    return (scalar1 >= 0 && scalar2 >= 0);
}

sf::Sprite const SegmentPhysicsBody::getBodySprite(sf::Vector2f const& anchor) const {
    if (!s_bodyTexture.has_value()) {
        s_bodyTexture.emplace();
        
        auto& texture = s_bodyTexture.value();
        texture.create(100.f, s_debugShapeWidth);
        texture.clear(sf::Color::Yellow);
        
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f{100.f, s_debugShapeWidth});
        shape.setFillColor(s_debugPhysicsOutlineColor);
        shape.setPosition(0.f, 0.f);
        
        texture.draw(shape);
        texture.display();
    }
    
    float length = getLength();
    float angle = std::acos(getVector().x / length);
    float scaleFactor = length / 100;
    
    sf::Sprite bodySprite;
    bodySprite.setTexture(s_bodyTexture.value().getTexture());
    bodySprite.setScale(scaleFactor, 1.f);
    bodySprite.setPosition(m_startPos.x + anchor.x + s_debugShapeWidth / 2.f, m_startPos.y + anchor.y + s_debugShapeWidth / 2.f);
    bodySprite.setRotation(angle * 180.f / M_PI);
    
    bodySprite.setColor(m_didCollide ? sf::Color::Red : sf::Color::White);
    m_didCollide = false;
    
    return bodySprite;
}

sf::Vector2f const& SegmentPhysicsBody::getEndPos() const {
    return m_endPos;
}

float SegmentPhysicsBody::getLength() const {
    return mz::getVectorLength(getVector());
}

float SegmentPhysicsBody::getLength2() const {
    return mz::getVectorLength2(getVector());
}

sf::Vector2f const& SegmentPhysicsBody::getStartPos() const {
    return m_startPos;
}

sf::Vector2f SegmentPhysicsBody::getVector() const {
    return sf::Vector2f{m_endPos.x - m_startPos.x, m_endPos.y - m_startPos.y};
}

void SegmentPhysicsBody::updateFrame() {
    sf::Vector2f origin {std::min(m_startPos.x, m_endPos.x), std::min(m_startPos.y, m_endPos.y)};
    if (m_frame.origin != origin)
        m_frame.origin = origin;
}

std::optional<sf::RenderTexture>    SegmentPhysicsBody::s_bodyTexture = std::nullopt;

}
