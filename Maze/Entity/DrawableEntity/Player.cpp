//
//  Player.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include "Player.hpp"

namespace mz {

Player::Player(float width) :
m_width(width),
m_bodyShape(m_width / 2.f)
{
    m_bodyShape.setFillColor(sf::Color::Red);
    m_bodyShape.setPosition(0.f, 0.f);
}

void Player::update(sf::Time timeElapsed) {
    for (auto it = m_directions.begin(); it != m_directions.end(); ++it) {
        auto delta {Orientation::getOrientation(*it).toVector()};
        Entity::move({
            delta.x * timeElapsed.asMilliseconds() * m_speed * 0.1f,
            delta.y * timeElapsed.asMilliseconds() * m_speed * 0.1f
        });
    }
}

std::unique_ptr<sf::RenderTexture> Player::draw() {
    auto texture {std::make_unique<sf::RenderTexture>()};
    
    texture->create(m_width, m_width);
    texture->clear(sf::Color::Transparent);
    
    texture->draw(m_bodyShape);
    
    return texture;
}

void Player::move(Orientation const& orientation, bool isMoving) {
    if (isMoving) {
        m_directions.insert(orientation.getName());
    } else {
        m_directions.erase(orientation.getName());
    }
}

}
