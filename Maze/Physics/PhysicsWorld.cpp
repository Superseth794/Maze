//
//  PhysicsWorld.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "PhysicsWorld.hpp"

namespace mz {

PhysicsWorld::PhysicsWorld() :
m_root({sf::Vector2f{0.f, 0.f}, sf::Vector2f{0.f, 0.f}, {}, {}})
{
}

void PhysicsWorld::addBody(PhysicsBody* body) {
    m_root.bodies.push_back(body);
}

void PhysicsWorld::removeBody(PhysicsBody* body) {
    for (int i = 0; i < m_root.bodies.size(); ++i) {
        if (m_root.bodies[i] == body) {
            m_root.bodies.erase(m_root.bodies.begin() + i);
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

}
