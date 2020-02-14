//
//  Player.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include "Player.hpp"

namespace mz {

Player::Player(float width, PhysicsWorld* parentWorld) :
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
    
    sf::RectangleShape physicsBodyShape {sf::Vector2f{m_physicsBody.getFrame().width, m_physicsBody.getFrame().height}};
    physicsBodyShape.setFillColor(sf::Color(101, 184, 127, 125));
    physicsBodyShape.setPosition(0.f, 0.f);
    m_texture->draw(physicsBodyShape);
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
}

std::shared_ptr<sf::RenderTexture> Player::draw() {
//    auto texture {std::make_shared<sf::RenderTexture>()};
//
//    texture->create(m_width, m_width);
//    texture->clear(sf::Color::Transparent);
//
//    texture->draw(m_bodyShape);
//    texture->display();
//
//    return texture;
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
