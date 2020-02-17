//
//  RectanglePhysicsBody.cpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#include "RectanglePhysicsBody.hpp"

namespace mz {

RectanglePhysicsBody::RectanglePhysicsBody(float width, float height, float rotation, sf::Vector2f const& origin, PhysicsWorld* parentWorld) :
PhysicsBody(origin + sf::Vector2f{std::cos(rotation) * width + std::sin(rotation) * height, std::sin(rotation) * width - std::cos(rotation) * height} / 2.f, parentWorld),
m_width(width),
m_heigth(height),
m_rotation(rotation)
{
    m_frame.width = std::cos(rotation) * m_width + std::sin(rotation) * m_heigth;
    m_frame.height = std::sin(rotation) * m_width + std::cos(rotation) * m_heigth;
    RectanglePhysicsBody::updateFrame();
}

RectanglePhysicsBody::RectanglePhysicsBody(sf::Vector2f const& firstCorner, sf::Vector2f const& secondCorner, float rotation, PhysicsWorld* parentWorld) :
PhysicsBody(sf::Vector2f{(firstCorner.x + secondCorner.x) / 2.f, (firstCorner.y + secondCorner.y) / 2.f}, parentWorld),
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
    sf::Vector2f origin {
        getCenter().x - m_width / 2.f,
        getCenter().y - m_heigth / 2.f
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
    auto getAngle = [&position] (sf::Vector2f const& ref) -> float {
        return std::atan((ref.y - position.y) / (ref.x - position.x));
    };
    float anglesSum = getAngle(getTopLeftCorner()) + getAngle(getTopRightCorner()) + getAngle(getBottomRightCorner()) + getAngle(getBottomLeftCorner());
    return (2 * M_PI * 0.99f <= anglesSum && anglesSum <= 2 * M_PI * 1.01);
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

void RectanglePhysicsBody::generateDebugTexture() {
    m_debugTexture = std::make_shared<sf::RenderTexture>();
    
    float width = m_width * std::cos(m_rotation) + m_heigth * std::sin(m_rotation);
    float height = m_width * std::sin(m_rotation) + m_heigth * std::cos(m_rotation);
    
    m_debugTexture->create(width, height);
    m_debugTexture->clear(sf::Color::Transparent);
    
    sf::ConvexShape debugShape;
    debugShape.setPointCount(4);
    debugShape.setFillColor(DEBUG_PHYSICS_FILL_COLOR);
    debugShape.setOutlineColor(DEBUG_PHYSICS_OUTLINE_COLOR);
    debugShape.setOutlineThickness(2.f);
    
    sf::Vector2f anchor {
        getCenter().x - m_width / 2.f,
        getCenter().y - m_heigth / 2.f
    };
    
    debugShape.setPoint(0, getTopLeftCorner() - anchor);
    std::cout << "Top left x: " << getTopLeftCorner().x - anchor.x << " y: " << getTopLeftCorner().y - anchor.y << std::endl;
    debugShape.setPoint(1, getTopRightCorner() - anchor);
    std::cout << "Top right x: " << getTopRightCorner().x - anchor.x << " y: " << getTopRightCorner().y - anchor.y << std::endl;
    debugShape.setPoint(2, getBottomRightCorner() - anchor);
    std::cout << "Bottom right x: " << getBottomRightCorner().x - anchor.x << " y: " << getBottomRightCorner().y - anchor.y << std::endl;;
    debugShape.setPoint(3, getBottomLeftCorner() - anchor);
    std::cout << "Bottom left x: " << getBottomLeftCorner().x - anchor.x << " y: " << getBottomLeftCorner().y - anchor.y << std::endl;
    
    m_debugTexture->draw(debugShape);
    
    m_debugTexture->display();
}

}
