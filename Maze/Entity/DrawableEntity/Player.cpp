//
//  Player.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include "Player.hpp"

namespace mz {

Player::Player(float width, std::uint64_t bodyId, PhysicsWorld* parentWorld) :
m_width(width),
m_bodyShape(m_width / 2.f),
m_texture(std::make_shared<sf::RenderTexture>()),
m_physicsBody(width / 2.f, sf::Vector2f{width / 2.f, width / 2.f}, parentWorld)
{
    m_bodyShape.setFillColor(sf::Color::Red);
    m_bodyShape.setPosition(0.f, 0.f);
    
    m_texture->create(m_width, m_width);
    m_texture->clear(sf::Color::Transparent);
    m_texture->draw(m_bodyShape);
    
    m_texture->display();
}

void Player::update(sf::Time timeElapsed) {
    for (auto it = m_directions.begin(); it != m_directions.end(); ++it) {
        auto delta {Orientation::getOrientation(*it).toVector()};
        Entity::move({
            delta.x * timeElapsed.asMilliseconds() * m_speed * 0.1f,
            delta.y * timeElapsed.asMilliseconds() * m_speed * 0.1f
        });
    }
    m_physicsBody.setCenter(sf::Vector2f{getPosition().x + m_width / 2.f, getPosition().y + m_width / 2.f});
    m_physicsBody.updateInWorld();
}

std::shared_ptr<sf::RenderTexture> Player::draw() {
    return m_texture;
}

void Player::orientedMove(Orientation const& orientation, bool isMoving) {
    if (isMoving) {
        m_directions.insert(orientation.getName());
    } else {
        m_directions.erase(orientation.getName());
    }
}

PhysicsBody* Player::getPhysicsBody() {
    return &m_physicsBody;
}

}
