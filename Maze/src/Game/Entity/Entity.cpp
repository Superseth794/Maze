//
//  Entity.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include "../../../include/Game/Entity/Entity.hpp"

namespace mz {

Entity::Entity() :
m_x(0.f),
m_y(0.f)
{
}

sf::Vector2f Entity::getPosition() const {
    return  sf::Vector2f{m_x, m_y};
}

void Entity::move(sf::Vector2f direction) {
    m_x += direction.x;
    m_y += direction.y;
}

void Entity::moveTo(sf::Vector2f const& position) {
    m_x = position.x;
    m_y = position.y;
}

void Entity::moveX(float dx) {
    m_x += dx;
}

void Entity::moveY(float dy) {
    m_y += dy;
}

}
