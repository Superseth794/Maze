//
//  CirclePhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#include "../../include/Physics/CirclePhysicsBody.hpp"

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

PhysicsBody* CirclePhysicsBody::clone() const {
    return new CirclePhysicsBody(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> CirclePhysicsBody::collideWith(PhysicsBody* body) const {
    return body->collideWith(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> CirclePhysicsBody::collideWith(CirclePhysicsBody const& circle) const {
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    
    float dist2 = (circle.getCenter().x - getCenter().x) * (circle.getCenter().x - getCenter().x) + (circle.getCenter().y - getCenter().y) * (circle.getCenter().y - getCenter().y);
    
    if (dist2 > m_radius * m_radius + circle.m_radius * circle.m_radius)
        return intersections;
    
    if (circle.getCenter() == getCenter())
        return intersections;
    
    float a = ((circle.getCenter().x * circle.getCenter().x) + (circle.getCenter().y * circle.getCenter().y) - (getCenter().x * getCenter().x) - (getCenter().y * getCenter().y) + m_radius * m_radius - circle.m_radius * circle.m_radius) / (2 * (circle.getCenter().y - getCenter().y));
    float d = (circle.getCenter().x - getCenter().x) / (circle.getCenter().y - getCenter().y);
    
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

std::unique_ptr<std::vector<sf::Vector2f>> CirclePhysicsBody::collideWith(RectanglePhysicsBody const& rectangle) const {
    return rectangle.collideWith(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> CirclePhysicsBody::collideWith(SegmentPhysicsBody const& segment) const {
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    
    sf::Vector2f AB {segment.getVector()};
    if (AB.x == 0)
        return intersections;
    
    const float a = AB.y / AB.x;
    const float b = segment.getStartPos().y - a * segment.getStartPos().x;
    
    const float coefA = 1 + a * a;
    const float coefB = 2 * a * b - 2 * getCenter().x - 2 * getCenter().y * a;
    const float coefC = getCenter().x * getCenter().x + getCenter().y * getCenter().y + b * b - 2 * getCenter().y * b - m_radius * m_radius;
    
    float delta = coefB * coefB - 4 * coefA * coefC;
    
    if (delta < 0)
        return intersections;
    
    if (delta == 0) {
        const float x = -coefB / (2 * coefA);
        const float y = a * x + b;
        
        if (segment.isPositionInside(sf::Vector2f{x, y}))
            intersections->emplace_back(x, y);
    } else {
        float deltaSqrt = std::sqrt(delta);
        
        const float x1 = (-coefB - deltaSqrt) / (2 * coefA);
        const float x2 = (-coefB + deltaSqrt) / (2 * coefA);
        const float y1 = a * x1 + b;
        const float y2 = a * x2 + b;
        
        if (segment.isPositionInside(sf::Vector2f{x1, y1}))
            intersections->emplace_back(x1, y1);
        if (segment.isPositionInside(sf::Vector2f{x2, y2}))
            intersections->emplace_back(x2, y2);
    }
    
    return intersections;
}

sf::Sprite const CirclePhysicsBody::getBodySprite(sf::Vector2f const& anchor) const {
    if (!s_bodyTexture.has_value()) {
        s_bodyTexture.emplace();
        
        auto& texture = s_bodyTexture.value();
        texture.create(100, 100);
        texture.clear(sf::Color::Transparent);
        
        sf::CircleShape shape;
        shape.setRadius(50.f);
        shape.setFillColor(s_debugPhysicsFillColor);
        shape.setOutlineColor(s_debugPhysicsOutlineColor);
        shape.setOutlineThickness(-10.f);
        shape.setPosition(0.f, 0.f);
        
        texture.draw(shape);
        texture.display();
    }
    
    float scaleFactor = m_radius / 50.f;
    
    sf::Sprite bodySprite;
    bodySprite.setTexture(s_bodyTexture.value().getTexture());
    bodySprite.setScale(scaleFactor, scaleFactor);
    bodySprite.setPosition(m_frame.origin.x + anchor.x, m_frame.origin.y + anchor.y);
    
    bodySprite.setColor(m_didCollide ? sf::Color::Red : sf::Color::White);
    m_didCollide = false;
    
    return bodySprite;
}

bool CirclePhysicsBody::isCollidingWithAABB(AABB const& box) const {
    for (auto corner : {box.getTopLeftCorner(), box.getTopRightCorner(), box.getBottomRightCorner(), box.getBottomLeftCorner()}) {
        if (isPositionInside(corner))
            return true;
    }
    
    if (isPositionInsideAABB(box, getCenter()))
        return true;
    
    const sf::Vector2f border1 = box.getTopRightCorner() - box.getTopLeftCorner();
    const sf::Vector2f segment1a = getCenter() - box.getTopLeftCorner();
    const sf::Vector2f segment1b = getCenter() - box.getTopRightCorner();
    const float scalar1a = border1.x * segment1a.x + border1.y * segment1a.y;
    const float scalar1b = -border1.x * segment1b.x + -border1.y * segment1b.y;
    
    if (scalar1a >= 0 && scalar1b >= 0)
        return true;
    
    sf::Vector2f border2 = box.getTopRightCorner() - box.getBottomRightCorner();
    sf::Vector2f segment2a = getCenter() - box.getTopRightCorner();
    sf::Vector2f segment2b = getCenter() - box.getBottomRightCorner();
    float scalar2a = border2.x * segment2a.x + border2.y * segment2a.y;
    float scalar2b = -border2.x * segment2b.x + -border2.y * segment2b.y;
    
    if (scalar2a >= 0 && scalar2b >= 0)
        return true;
    
    return false;
}

bool CirclePhysicsBody::isInsideAABB(AABB const& box) const {
    return (isPositionInsideAABB(box, getCenter()) &&
            isPositionInsideAABB(box, sf::Vector2f{getCenter().x + m_radius, getCenter().y}) &&
            isPositionInsideAABB(box, sf::Vector2f{getCenter().x, getCenter().y + m_radius}) &&
            isPositionInsideAABB(box, sf::Vector2f{getCenter().x - m_radius, getCenter().y}) &&
            isPositionInsideAABB(box, sf::Vector2f{getCenter().x, getCenter().y - m_radius}));
}

bool CirclePhysicsBody::isPositionInside(sf::Vector2f const& position) const {
    float distance2 = (getCenter().x - position.x) * (getCenter().x - position.x) + (getCenter().y - position.y) * (getCenter().y - position.y);
    return distance2 <= m_radius * m_radius;
}

void CirclePhysicsBody::updateFrame() {
    sf::Vector2f boxOrigin {getCenter().x - m_radius, getCenter().y - m_radius};
    if (boxOrigin != m_frame.origin)
        m_frame.origin = boxOrigin;
}

std::optional<sf::RenderTexture>    CirclePhysicsBody::s_bodyTexture = std::nullopt;

}
