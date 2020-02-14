//
//  PhysicsWorld.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "PhysicsWorld.hpp"

namespace mz {

PhysicsWorld::PhysicsWorld(bool showPhysics) :
m_root({sf::Vector2f{0.f, 0.f}, sf::Vector2f{0.f, 0.f}, {}, {}}),
m_showPhysics(showPhysics)
{
}

void PhysicsWorld::addBody(PhysicsBody* body) {
    if (!body)
        return;
    
    ++m_bodiesCount;
    
    // TODO implement quadtree
    m_root.bodies.push_back(body);
}

void PhysicsWorld::removeBody(PhysicsBody* body) {
    for (int i = 0; i < m_root.bodies.size(); ++i) {
        if (m_root.bodies[i] == body) {
            m_root.bodies.erase(m_root.bodies.begin() + i);
            --m_bodiesCount;
            break;
        }
    }
}

std::unique_ptr<std::vector<PhysicsWorld::Collision>> PhysicsWorld::checkCollision(PhysicsBody* body, sf::Vector2f const& anchor) {
    body->move(anchor);
    
    auto collisions {std::make_unique<std::vector<Collision>>()};
    
    for (auto bodyB : m_root.bodies) {
        if (body == bodyB)
            continue;
        auto intersections {bodyB->collideWith(body)};
        if (intersections->size() == 0)
            continue;
        collisions->push_back({bodyB, std::move(intersections)});
    }
    
    body->move(sf::Vector2f{-anchor.x, -anchor.y});
    
    return collisions;
}

int PhysicsWorld::getBodiesCount() {
    return m_bodiesCount;
}

std::unique_ptr<sf::RenderTexture> PhysicsWorld::getPhysicsDebugTexture(float width, float height, sf::Vector2f const& anchor) {
    auto texture {std::make_unique<sf::RenderTexture>()};
    texture->create(width, height);
    texture->clear(sf::Color::Transparent);
    
    if (!m_showPhysics) {
        return texture;
    }
    
    for (auto const& body : m_root.bodies) {
        auto bodyTexture {body->getDebugTexture()};
        
        sf::Vector2f spritePosition {
            body->getCenter().x - bodyTexture->getSize().x / 2.f - anchor.x + width / 2.f,
            body->getCenter().y - bodyTexture->getSize().y / 2.f - anchor.y + height / 2.f
        };
        
        sf::Sprite bodySprite;
        bodySprite.setTexture(bodyTexture->getTexture());
        bodySprite.setPosition(spritePosition);
        texture->draw(bodySprite);
    }
    
    if (m_showPhysics && false) {
        for (auto& collision : m_debugCollisions) {
            for (auto& collisionPosition : *std::get<1>(collision)) {
                sf::Vector2f debugSpritePosition {
                    collisionPosition.x - 10.f - anchor.x + width / 2.f,
                    collisionPosition.y - 10.f - anchor.y + height / 2.f
                };
                sf::CircleShape debugCollisionSprite {10.f};
                debugCollisionSprite.setFillColor(DEBUG_COLLISION_FILL_COLOR);
                debugCollisionSprite.setOutlineColor(DEBUG_COLLISION_OUTLINE_COLOR);
                debugCollisionSprite.setPosition(debugSpritePosition);
                texture->draw(debugCollisionSprite);
            }
        }
    }
    
    texture->display();
    
    return texture;
}

void PhysicsWorld::simulate() {
    if (!m_showPhysics || true)
        return;
    
    m_debugCollisions.clear();
    for (auto& body : m_root.bodies) {
        auto collisions {checkCollision(body)};
        for (int i = 0; i < collisions->size(); ++i) {
            m_debugCollisions.emplace_back(std::move((*collisions)[i]));
        }
    }
}

const sf::Color PhysicsWorld::DEBUG_COLLISION_FILL_COLOR = sf::Color{255, 0, 0, 200};
const sf::Color PhysicsWorld::DEBUG_COLLISION_OUTLINE_COLOR = sf::Color{255, 145, 0};

}
