//
//  RectanglePhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#include "RectanglePhysicsBody.hpp"

namespace mz {

RectanglePhysicsBody::RectanglePhysicsBody(float width, float height, float rotation, sf::Vector2f const& origin, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld) :
PhysicsBody(origin + sf::Vector2f{std::cos(rotation) * width + std::sin(rotation) * height, std::sin(rotation) * width - std::cos(rotation) * height} / 2.f, categoryBitMask, parentWorld),
m_width(width),
m_heigth(height),
m_rotation(rotation)
{
    m_frame.width = std::cos(rotation) * m_width + std::sin(rotation) * m_heigth;
    m_frame.height = std::sin(rotation) * m_width + std::cos(rotation) * m_heigth;
    RectanglePhysicsBody::updateFrame();
}

RectanglePhysicsBody::RectanglePhysicsBody(sf::Vector2f const& firstCorner, sf::Vector2f const& secondCorner, float rotation, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld) :
PhysicsBody(sf::Vector2f{(firstCorner.x + secondCorner.x) / 2.f, (firstCorner.y + secondCorner.y) / 2.f}, categoryBitMask, parentWorld),
m_rotation(rotation)
{
    float dist = std::sqrt((firstCorner.x - secondCorner.x) * (firstCorner.x - secondCorner.x) + (firstCorner.y - secondCorner.y) * (firstCorner.y - secondCorner.y));
    float subAngle = std::acos((secondCorner.x - firstCorner.x) / dist);
    float complAngle = rotation - subAngle;
    m_width = std::cos(complAngle) * dist;
    m_heigth = std::cos(complAngle) * dist;
    
    m_frame.width = std::cos(rotation) * m_width + std::sin(rotation) * m_heigth;
    m_frame.height = std::sin(rotation) * m_width + std::cos(rotation) * m_heigth;
    RectanglePhysicsBody::updateFrame();
}

RectanglePhysicsBody::RectanglePhysicsBody(RectanglePhysicsBody const& body) :
PhysicsBody(body),
m_width(body.m_width),
m_heigth(body.m_heigth),
m_rotation(body.m_rotation)
{
}

RectanglePhysicsBody::~RectanglePhysicsBody() {
    if (getParentWorld())
        getParentWorld()->removeBody(this);
}

void RectanglePhysicsBody::updateFrame() {
//    sf::Vector2f origin {
//        getCenter().x - m_width / 2.f,
//        getCenter().y - m_heigth / 2.f
//    };
    sf::Vector2f origin {
        std::min(std::min(getTopLeftCorner().x, getTopRightCorner().x), std::min(getBottomLeftCorner().x, getBottomRightCorner().x)),
        std::min(std::min(getTopLeftCorner().y, getTopRightCorner().y), std::min(getBottomLeftCorner().y, getBottomRightCorner().y))
    };
    if (m_frame.origin != origin)
        m_frame.origin = origin;
}

std::unique_ptr<std::vector<sf::Vector2f>> RectanglePhysicsBody::collideWith(PhysicsBody* body) {
    return body->collideWithRectangle(this);
}

bool RectanglePhysicsBody::isInsideAABB(AABB const& box) const {
    return (isPositionInsideAABB(box, getTopLeftCorner()) &&
            isPositionInsideAABB(box, getTopRightCorner()) &&
            isPositionInsideAABB(box, getBottomRightCorner()) &&
            isPositionInsideAABB(box, getBottomLeftCorner()));
}

bool RectanglePhysicsBody::isCollidingWithAABB(AABB const& box) const {
    return (isPositionInside(box.getTopLeftCorner()) ||
            isPositionInside(box.getTopRightCorner()) ||
            isPositionInside(box.getBottomRightCorner()) ||
            isPositionInside(box.getBottomLeftCorner()) ||
            isPositionInsideAABB(box, getTopLeftCorner()) ||
            isPositionInsideAABB(box, getTopRightCorner()) ||
            isPositionInsideAABB(box, getBottomRightCorner()) ||
            isPositionInsideAABB(box, getBottomLeftCorner()));
}

bool RectanglePhysicsBody::isPositionInside(sf::Vector2f const& position) const {
    auto getAngle = [&position] (sf::Vector2f const& ref) -> float {
        return std::atan((ref.y - position.y) / (ref.x - position.x));
    };
    float anglesSum = getAngle(getTopLeftCorner()) + getAngle(getTopRightCorner()) + getAngle(getBottomRightCorner()) + getAngle(getBottomLeftCorner());
    return (2 * M_PI * 0.99f <= anglesSum && anglesSum <= 2 * M_PI * 1.01);
}

std::unique_ptr<std::vector<sf::Vector2f>> RectanglePhysicsBody::collideWithSegment(SegmentPhysicsBody* segment) {
    SegmentPhysicsBody seg1 {getTopLeftCorner(), getTopRightCorner()};
    SegmentPhysicsBody seg2 {getTopRightCorner(), getBottomRightCorner()};
    SegmentPhysicsBody seg3 {getBottomRightCorner(), getBottomLeftCorner()};
    SegmentPhysicsBody seg4 {getBottomLeftCorner(), getTopLeftCorner()};
    
    std::array<std::unique_ptr<std::vector<sf::Vector2f>>, 4> inters {
        segment->collideWithSegment(&seg1),
        segment->collideWithSegment(&seg2),
        segment->collideWithSegment(&seg3),
        segment->collideWithSegment(&seg4)
    };
    
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    for (auto & array : inters) {
        for (int i = 0; i < array->size(); ++i) {
            intersections->emplace_back(std::move((*array)[i]));
        }
    }
    
    return intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> RectanglePhysicsBody::collideWithCircle(CirclePhysicsBody* circle) {
    SegmentPhysicsBody seg1 {getTopLeftCorner(), getTopRightCorner()};
    SegmentPhysicsBody seg2 {getTopRightCorner(), getBottomRightCorner()};
    SegmentPhysicsBody seg3 {getBottomRightCorner(), getBottomLeftCorner()};
    SegmentPhysicsBody seg4 {getBottomLeftCorner(), getTopLeftCorner()};
    
    std::array<std::unique_ptr<std::vector<sf::Vector2f>>, 4> inters {
        circle->collideWithSegment(&seg1),
        circle->collideWithSegment(&seg2),
        circle->collideWithSegment(&seg3),
        circle->collideWithSegment(&seg4)
    };
    
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    for (auto & array : inters) {
        for (int i = 0; i < array->size(); ++i) {
            intersections->emplace_back(std::move((*array)[i]));
        }
    }
    
    return intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> RectanglePhysicsBody::collideWithRectangle(RectanglePhysicsBody* rectangle) {
    SegmentPhysicsBody seg1 {getTopLeftCorner(), getTopRightCorner()};
    SegmentPhysicsBody seg2 {getTopRightCorner(), getBottomRightCorner()};
    SegmentPhysicsBody seg3 {getBottomRightCorner(), getBottomLeftCorner()};
    SegmentPhysicsBody seg4 {getBottomLeftCorner(), getTopLeftCorner()};
    
    std::array<std::unique_ptr<std::vector<sf::Vector2f>>, 4> inters {
        rectangle->collideWithSegment(&seg1),
        rectangle->collideWithSegment(&seg2),
        rectangle->collideWithSegment(&seg3),
        rectangle->collideWithSegment(&seg4)
    };
    
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    for (auto & array : inters) {
        for (int i = 0; i < array->size(); ++i) {
            intersections->emplace_back(std::move((*array)[i]));
        }
    }
    
    return intersections;
}

PhysicsBody* RectanglePhysicsBody::clone() const {
    return new RectanglePhysicsBody(*this);
}

sf::Vector2f RectanglePhysicsBody::getTopLeftCorner() const {
    float angle = m_rotation - std::atan(m_heigth / m_width);
    float diagonal = std::sqrt(m_width * m_width + m_heigth * m_heigth);
    return sf::Vector2f {
        getCenter().x - std::cos(angle) * diagonal / 2.f,
        getCenter().y + std::sin(angle) * diagonal / 2.f
    };
}

sf::Vector2f RectanglePhysicsBody::getTopRightCorner() const {
    float angle = m_rotation - std::atan(m_heigth / m_width);
    float diagonal = std::sqrt(m_width * m_width + m_heigth * m_heigth);
    return sf::Vector2f {
        getCenter().x - std::sin(angle) * diagonal / 2.f,
        getCenter().y - std::cos(angle) * diagonal / 2.f
    };
}

sf::Vector2f RectanglePhysicsBody::getBottomRightCorner() const {
    float angle = m_rotation - std::atan(m_heigth / m_width);
    float diagonal = std::sqrt(m_width * m_width + m_heigth * m_heigth);
    return sf::Vector2f {
        getCenter().x + std::cos(angle) * diagonal / 2.f,
        getCenter().y - std::sin(angle) * diagonal / 2.f
    };
}

sf::Vector2f RectanglePhysicsBody::getBottomLeftCorner() const {
    float angle = m_rotation - std::atan(m_heigth / m_width);
    float diagonal = std::sqrt(m_width * m_width + m_heigth * m_heigth);
    return sf::Vector2f {
        getCenter().x + std::sin(angle) * diagonal / 2.f,
        getCenter().y + std::cos(angle) * diagonal / 2.f
    };
}

float RectanglePhysicsBody::getRotation() const {
    return m_rotation;
}

sf::Sprite const RectanglePhysicsBody::getBodySprite(sf::Vector2f const& anchor) {
    if (!bodyTexture.has_value()) {
        bodyTexture.emplace();
        
        auto& texture = bodyTexture.value();
        texture.create(100.f, 100.f);
        texture.clear(sf::Color::Transparent);
        
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f{100.f, 100.f});
        shape.setFillColor(DEBUG_PHYSICS_FILL_COLOR);
        shape.setOutlineColor(DEBUG_PHYSICS_OUTLINE_COLOR);
        shape.setOutlineThickness(-5.f);
        shape.setPosition(0.f, 0.f);
        
        texture.draw(shape);
        texture.display();
    }
    
    sf::Vector2f scaleFactors {
        m_width / 100.f,
        m_heigth / 100.f
    };
    
    sf::Sprite bodySprite;
    bodySprite.setTexture(bodyTexture.value().getTexture());
    bodySprite.setScale(scaleFactors);
    bodySprite.setPosition(m_frame.origin.x + anchor.x, m_frame.origin.y + anchor.y);
    bodySprite.setRotation(m_rotation);
    
    return bodySprite;
}

std::optional<sf::RenderTexture> RectanglePhysicsBody::bodyTexture = std::nullopt;

}
