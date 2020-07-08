//
//  RectanglePhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#include "../../include/Physics/RectanglePhysicsBody.hpp"

namespace mz {

RectanglePhysicsBody::RectanglePhysicsBody(float width, float height, sf::Vector2f const& origin, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld) :
PhysicsBody(origin + sf::Vector2f{width, height} / 2.f, categoryBitMask, parentWorld),
m_width(width),
m_heigth(height)
{
    m_frame.width =  m_width;
    m_frame.height = m_heigth;
    RectanglePhysicsBody::updateFrame();
}

RectanglePhysicsBody::RectanglePhysicsBody(sf::Vector2f const& firstCorner, sf::Vector2f const& secondCorner, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld) : RectanglePhysicsBody(std::abs(firstCorner.x - secondCorner.x), std::abs(firstCorner.y - secondCorner.y), firstCorner, categoryBitMask, parentWorld)
{
    m_width = std::abs(firstCorner.x - secondCorner.x);
    m_heigth = std::abs(firstCorner.y - secondCorner.y);
}

RectanglePhysicsBody::RectanglePhysicsBody(RectanglePhysicsBody const& body) :
PhysicsBody(body),
m_width(body.m_width),
m_heigth(body.m_heigth)
{
}

RectanglePhysicsBody::~RectanglePhysicsBody() {
    if (getParentWorld())
        getParentWorld()->removeBody(this);
}

PhysicsBody* RectanglePhysicsBody::clone() const {
    return new RectanglePhysicsBody(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> RectanglePhysicsBody::collideWith(PhysicsBody* body) const {
    return body->collideWith(*this);
}

std::unique_ptr<std::vector<sf::Vector2f>> RectanglePhysicsBody::collideWith(CirclePhysicsBody const& circle) const {
    SegmentPhysicsBody seg1 {getTopLeftCorner(), getTopRightCorner()};
    SegmentPhysicsBody seg2 {getTopRightCorner(), getBottomRightCorner()};
    SegmentPhysicsBody seg3 {getBottomRightCorner(), getBottomLeftCorner()};
    SegmentPhysicsBody seg4 {getBottomLeftCorner(), getTopLeftCorner()};
    
    std::array<std::unique_ptr<std::vector<sf::Vector2f>>, 4> inters {
        circle.collideWith(seg1),
        circle.collideWith(seg2),
        circle.collideWith(seg3),
        circle.collideWith(seg4)
    };
    
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    for (auto & array : inters) {
        for (int i = 0; i < array->size(); ++i) {
            intersections->emplace_back(std::move((*array)[i]));
        }
    }
    
    return intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> RectanglePhysicsBody::collideWith(RectanglePhysicsBody const& rectangle) const {
    SegmentPhysicsBody seg1 {getTopLeftCorner(), getTopRightCorner()};
    SegmentPhysicsBody seg2 {getTopRightCorner(), getBottomRightCorner()};
    SegmentPhysicsBody seg3 {getBottomRightCorner(), getBottomLeftCorner()};
    SegmentPhysicsBody seg4 {getBottomLeftCorner(), getTopLeftCorner()};
    
    std::array<std::unique_ptr<std::vector<sf::Vector2f>>, 4> inters {
        rectangle.collideWith(seg1),
        rectangle.collideWith(seg2),
        rectangle.collideWith(seg3),
        rectangle.collideWith(seg4)
    };
    
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    for (auto & array : inters) {
        for (int i = 0; i < array->size(); ++i) {
            intersections->emplace_back(std::move((*array)[i]));
        }
    }
    
    return intersections;
}

std::unique_ptr<std::vector<sf::Vector2f>> RectanglePhysicsBody::collideWith(SegmentPhysicsBody const& segment) const {
    SegmentPhysicsBody seg1 {getTopLeftCorner(), getTopRightCorner()};
    SegmentPhysicsBody seg2 {getTopRightCorner(), getBottomRightCorner()};
    SegmentPhysicsBody seg3 {getBottomRightCorner(), getBottomLeftCorner()};
    SegmentPhysicsBody seg4 {getBottomLeftCorner(), getTopLeftCorner()};
    
    std::array<std::unique_ptr<std::vector<sf::Vector2f>>, 4> inters {
        segment.collideWith(seg1),
        segment.collideWith(seg2),
        segment.collideWith(seg3),
        segment.collideWith(seg4)
    };
    
    auto intersections {std::make_unique<std::vector<sf::Vector2f>>()};
    for (auto & array : inters) {
        for (int i = 0; i < array->size(); ++i) {
            intersections->emplace_back(std::move((*array)[i]));
        }
    }
    
    return intersections;
}

sf::Sprite const RectanglePhysicsBody::getBodySprite(sf::Vector2f const& anchor) const {
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
    
    bodySprite.setColor(m_didCollide ? sf::Color(255, 0, 0, 255) : sf::Color::White);
    m_didCollide = false;
    
    return bodySprite;
}

sf::Vector2f RectanglePhysicsBody::getBottomLeftCorner() const {
    return sf::Vector2f {
        getCenter().x - m_width / 2.f,
        getCenter().y + m_heigth / 2.f
    };
}

sf::Vector2f RectanglePhysicsBody::getBottomRightCorner() const {
    return sf::Vector2f {
        getCenter().x + m_width / 2.f,
        getCenter().y + m_heigth / 2.f
    };
}

sf::Vector2f RectanglePhysicsBody::getTopLeftCorner() const {
    return sf::Vector2f {
        getCenter().x - m_width / 2.f,
        getCenter().y - m_heigth / 2.f
    };
}

sf::Vector2f RectanglePhysicsBody::getTopRightCorner() const {
    return sf::Vector2f {
        getCenter().x + m_width / 2.f,
        getCenter().y - m_heigth / 2.f
    };
}

bool RectanglePhysicsBody::isCollidingWithAABB(AABB const& box) const {
    return !(getTopLeftCorner().x > box.origin.x + box.width ||
             getTopLeftCorner().y > box.origin.y + box.height ||
             getBottomRightCorner().x < box.origin.x ||
             getBottomRightCorner().y < box.origin.y);
}

bool RectanglePhysicsBody::isInsideAABB(AABB const& box) const {
    return (isPositionInsideAABB(box, getTopLeftCorner()) &&
            isPositionInsideAABB(box, getTopRightCorner()) &&
            isPositionInsideAABB(box, getBottomRightCorner()) &&
            isPositionInsideAABB(box, getBottomLeftCorner()));
}

bool RectanglePhysicsBody::isPositionInside(sf::Vector2f const& position) const {
    return !(position.x < getTopLeftCorner().x ||
             position.y < getTopLeftCorner().y ||
             position.x > getBottomRightCorner().x ||
             position.y > getBottomRightCorner().y);
}

void RectanglePhysicsBody::updateFrame() {
    sf::Vector2f origin {
        getCenter().x - m_width / 2.f,
        getCenter().y - m_heigth / 2.f
    };
    if (m_frame.origin != origin)
        m_frame.origin = origin;
}

std::optional<sf::RenderTexture>    RectanglePhysicsBody::bodyTexture   = std::nullopt;

}
