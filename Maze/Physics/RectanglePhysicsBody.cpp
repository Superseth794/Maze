//
//  RectanglePhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#include "RectanglePhysicsBody.hpp"

namespace mz {

RectanglePhysicsBody::RectanglePhysicsBody(sf::Vector2f const& firstCorner, sf::Vector2f const& secondCorner, PhysicsWorld* parentWorld) :
PhysicsBody(sf::Vector2f{(firstCorner.x + secondCorner.x) / 2.f, (firstCorner.y + secondCorner.y) / 2.f}, parentWorld),
m_firstCorner(firstCorner),
m_secondCorner(secondCorner)
{
    m_frame.width = std::abs(secondCorner.x - firstCorner.x);
    m_frame.height = std::abs(secondCorner.y - firstCorner.y);
    RectanglePhysicsBody::updateFrame();
}

RectanglePhysicsBody::RectanglePhysicsBody(float width, float height, float rotation, sf::Vector2f const& origin, PhysicsWorld* parentWorld) :
RectanglePhysicsBody(origin, sf::Vector2f{origin.x + std::cos(rotation) * width + std::sin(rotation) * height, origin.y + std::cos(rotation) * height + std::sin(rotation) * width}, parentWorld)
{
}

void RectanglePhysicsBody::updateFrame() {
    sf::Vector2f origin {
        std::min(m_firstCorner.x, m_secondCorner.x),
        std::min(m_firstCorner.y, m_secondCorner.y)
    };
    if (m_frame.origin != origin)
        m_frame.origin = origin;
}

std::unique_ptr<std::vector<sf::Vector2f>> RectanglePhysicsBody::collideWith(PhysicsBody* body) {
    return body->collideWithRectangle(this);
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

bool RectanglePhysicsBody::isPositionInside(sf::Vector2f const& position) const {
    return !(position.x <= getTopLeftCorner().x ||
             position.x >= getTopRightCorner().x ||
             position.y <= getTopLeftCorner().y ||
             position.y >= getBottomRightCorner().y);
}

PhysicsBody* RectanglePhysicsBody::clone() const {
    return new RectanglePhysicsBody(*this);
}

sf::Vector2f RectanglePhysicsBody::getTopLeftCorner() const {
    return sf::Vector2f{std::min(m_firstCorner.x, m_secondCorner.x), std::min(m_firstCorner.y, m_secondCorner.y)};
}

sf::Vector2f RectanglePhysicsBody::getTopRightCorner() const {
    return sf::Vector2f{std::max(m_firstCorner.x, m_secondCorner.x), std::min(m_firstCorner.y, m_secondCorner.y)};
}

sf::Vector2f RectanglePhysicsBody::getBottomRightCorner() const {
    return sf::Vector2f{std::max(m_firstCorner.x, m_secondCorner.x), std::max(m_firstCorner.y, m_secondCorner.y)};
}

sf::Vector2f RectanglePhysicsBody::getBottomLeftCorner() const {
    return sf::Vector2f{std::min(m_firstCorner.x, m_secondCorner.x), std::max(m_firstCorner.y, m_secondCorner.y)};
}

}
