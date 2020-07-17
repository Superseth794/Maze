//
//  Player.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include "../../../../include/Game/Entity/DrawableEntity/Player.hpp"

namespace mz {

Player::Player(float width, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld) :
m_width(width),
m_bodyShape(width / 2.f),
m_texture(std::make_shared<sf::RenderTexture>()),
m_physicsBody(width / 2.f, sf::Vector2f{width / 2.f, width / 2.f}, categoryBitMask, parentWorld)
{
    m_bodyShape.setFillColor(sf::Color::Red);
    m_bodyShape.setPosition(0.f, 0.f);
    
    m_texture->create(m_width, m_width);
    m_texture->clear(sf::Color::Transparent);
    m_texture->draw(m_bodyShape);
    
    m_texture->display();
    
    m_physicsBody.setCollisionCallback([this](PhysicsBody::Collision const& collision) {
//        moveTo((*collision.second)[0]);
    });
}

std::shared_ptr<sf::RenderTexture> Player::draw() {
    return m_texture;
}

PhysicsBody* Player::getPhysicsBody() {
    return &m_physicsBody;
}

void Player::orientedMove(Orientation const& orientation, bool isMoving) {
    if (isMoving) {
        m_directions.insert(orientation.getName());
    } else {
        m_directions.erase(orientation.getName());
    }
}


void Player::update(sf::Time timeElapsed) {
    
//    auto previousPosition {getPosition()};
    
    for (auto it = m_directions.begin(); it != m_directions.end(); ++it) {
        auto delta {Orientation::getOrientation(*it).toVector()};
        Entity::move({
            delta.x * timeElapsed.asMilliseconds() * m_speed * 0.1f,
            delta.y * timeElapsed.asMilliseconds() * m_speed * 0.1f
        });
    }
    
    m_physicsBody.setCenter(sf::Vector2f{getPosition().x + m_width / 2.f, getPosition().y + m_width / 2.f});
    m_physicsBody.updateInWorld();
    
//    if (m_physicsBody.getParentWorld()->checkCollision(&m_physicsBody)->size() == 0) {
//        std::cout << "No collisions with player\n";
//    } else {
//        std::cout << "Collision with player found !\n";
//    }
    
//    if (m_physicsBody.getParentWorld()->checkCollision(&m_physicsBody)->size() != 0) {
//        move(previousPosition - getPosition());
//        m_physicsBody.setCenter(sf::Vector2f{getPosition().x + m_width / 2.f, getPosition().y + m_width / 2.f});
//        m_physicsBody.updateInWorld();
//    }
}

}
