//
//  PhysicsWorld.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "PhysicsWorld.hpp"

namespace mz {

bool isBodyInsideNode(PhysicsBody* body, Node* node) {
    return body->isInsideAABB(node->box);
}

PhysicsWorld::PhysicsWorld(bool showPhysics) :
m_root({AABB{sf::Vector2f{0.f, 0.f}, 0.f, 0.f}}),
m_showPhysics(showPhysics)
{
}

void PhysicsWorld::init(float width, float height) {
    m_root = Node{AABB{sf::Vector2f{0.f, 0.f}, width, height}};
}

void PhysicsWorld::addBody(PhysicsBody* body) {
    if (!body)
        return;
    ++m_bodiesCount;
    addBody(body, &m_root);
}

void PhysicsWorld::removeBody(PhysicsBody* body) {
    Node* currentNode = &m_root;
    
    while (currentNode) {
        for (int bodyId = 0; bodyId < currentNode->bodies.size(); ++bodyId) {
            if (currentNode->bodies[bodyId] == body) {
                currentNode->bodies.erase(currentNode->bodies.begin() + bodyId);
                std::cout << "Successfully removed physicsBody\n";
                return;
            }
        }
        
        if (!currentNode->childs[0].get())
            break;
        
        bool childFound = false;
        for (int childId = 0; childId < 4; ++childId) {
            if (isBodyInsideNode(body, currentNode->childs[childId].get())) {
                currentNode = currentNode->childs[childId].get();
                childFound = true;
                break;
            }
        }
        
        if (!childFound)
            break;
    }
    
    std::cout << "Could not find body to erase\n";
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
    
    // lambda to show physics bodies
    auto drawBody = [&width, &height, &anchor, &texture] (PhysicsBody* body) -> sf::Sprite {
        auto bodyTexture {body->getDebugTexture()};
        
        sf::Vector2f spritePosition {
            body->getCenter().x - bodyTexture->getSize().x / 2.f - anchor.x + width / 2.f,
            body->getCenter().y - bodyTexture->getSize().y / 2.f - anchor.y + height / 2.f
        };
        
        sf::Sprite bodySprite;
        bodySprite.setTexture(bodyTexture->getTexture());
        bodySprite.setPosition(spritePosition);
        return bodySprite;
    };
    
    // Shows Quadtree
    std::queue<Node*> toDrawNodes;
    std::vector<sf::Sprite> toDrawBodiesSprites;
    toDrawNodes.push(&m_root);
    while (!toDrawNodes.empty()) {
        Node* currentNode = toDrawNodes.front();
        toDrawNodes.pop();
        
        sf::Vector2f shapePosition {
            currentNode->box.origin.x - anchor.x + width / 2.f,
            currentNode->box.origin.y - anchor.y + height / 2.f
        };

        sf::RectangleShape nodeShape {sf::Vector2f{currentNode->box.width, currentNode->box.height}};
        nodeShape.setPosition(shapePosition);
        nodeShape.setFillColor(DEBUG_QUADTREE_NODES_COLOR);
        nodeShape.setOutlineColor(sf::Color::Black);
        nodeShape.setOutlineThickness(3.f);
        texture->draw(nodeShape);
        
        for (auto& body : currentNode->bodies) {
            toDrawBodiesSprites.emplace_back(drawBody(body));
        }

        if (currentNode->childs[0].get()) {
            for (int childId = 0; childId < 4; ++childId)
                toDrawNodes.push(currentNode->childs[childId].get());
        }
    }
    
    // Shows bodies
    for (auto const& sprite : toDrawBodiesSprites) {
        texture->draw(sprite);
    }
    
    // Shows collisisons
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

void PhysicsWorld::addBody(PhysicsBody* body, Node* node) {
    if (node->childs[0].get()) {
        for (int childId = 0; childId < 4; ++childId) {
            if (isBodyInsideNode(body, node->childs[childId].get())){
                addBody(body, node->childs[childId].get());
                return;
            }
        }
    }
    
    if (node->bodies.size() < MAX_BODIES_PER_NODE || node->childs[0].get()) {
        node->bodies.emplace_back(body);
        return;
    }
    
    const float subNodeWidth = node->box.width / 2.f;
    const float subNodeHeigth = node->box.height / 2.f;
    
    node->childs[0] = std::make_unique<Node>(
            AABB{node->box.origin, subNodeWidth, subNodeHeigth}
    );
    node->childs[1] = std::make_unique<Node>(
            AABB{sf::Vector2f{node->box.origin.x + subNodeWidth, node->box.origin.y}, subNodeWidth, subNodeHeigth}
    );
    node->childs[2] = std::make_unique<Node>(
            AABB{sf::Vector2f{node->box.origin.x, node->box.origin.y + subNodeHeigth}, subNodeWidth, subNodeHeigth}
    );
    node->childs[3] = std::make_unique<Node>(
            AABB{sf::Vector2f{node->box.origin.x + subNodeWidth, node->box.origin.y + subNodeHeigth}, subNodeWidth, subNodeHeigth}
    );
    
    std::vector<PhysicsBody*> bodies;
    std::swap(bodies, node->bodies);
    bodies.emplace_back(body);
    for (int bodyId = 0; bodyId < bodies.size(); ++bodyId) {
        addBody(bodies[bodyId], node);
    }
}

const sf::Color PhysicsWorld::DEBUG_COLLISION_FILL_COLOR = sf::Color{255, 0, 0, 200};
const sf::Color PhysicsWorld::DEBUG_COLLISION_OUTLINE_COLOR = sf::Color{255, 145, 130};
const sf::Color PhysicsWorld::DEBUG_QUADTREE_NODES_COLOR = sf::Color{255, 140, 255, 75};

}
