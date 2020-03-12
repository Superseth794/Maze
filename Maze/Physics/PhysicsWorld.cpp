//
//  PhysicsWorld.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "PhysicsWorld.hpp"

namespace mz {

bool isBodyInsideNode(PhysicsBody* body, QuadtreeNode* node) {
    return body->isInsideAABB(node->box);
}

PhysicsWorld::PhysicsWorld() :
m_root({AABB{sf::Vector2f{0.f, 0.f}, 0.f, 0.f}})
{
}

PhysicsWorld::~PhysicsWorld() {
    // Clears bodies to remove buffer
    removeAllBodies();
    
    int bodiesLeft = getBodiesCount();
    assert(bodiesLeft == getPreciseBodiesCount());
    
    if (bodiesLeft > 0)
        std::cout << "\nError: " << bodiesLeft << " bodie" << (bodiesLeft > 1 ? "s" : "") << " not removed from physics world" << std::endl;
    else
        std::cout << "\nSuccessfully removed all physics bodies from physics world" << std::endl;
}

void PhysicsWorld::init(float width, float height) {
    m_root = QuadtreeNode{AABB{sf::Vector2f{0.f, 0.f}, width, height}};
}

void PhysicsWorld::addBody(PhysicsBody* body) {
    if (!body)
        return;
    m_toAddBodies.emplace_back(std::move(body));
}

void PhysicsWorld::removeBody(PhysicsBody* body) {
    if (body) {
        auto searchResult {findBody(body, body->getParentNode())};
        if (searchResult.has_value()) {
            m_toRemoveBodiesPositions.emplace_back(std::move(searchResult.value()));
        }
    }
}

void PhysicsWorld::updateBody(PhysicsBody* body) {
    auto found {findBody(body, body->getParentNode())};
    
    if (!found.has_value()) {
        std::cout << "Body " << body->getId() << " not found for update\n";
        return;
    }
    
    QuadtreeNode* currentNode = found.value().first;
    currentNode->bodies.erase(currentNode->bodies.begin() + found.value().second);
    
    // Updates body debug node if needed
    for (auto& debugPair : m_debugBodiesUpdateDisplay) {
        if (*debugPair.first == *body)
            debugPair.second = currentNode;
    }
    
    addBody(body, currentNode);
}

void PhysicsWorld::addBodyQuadtreeAdditionEvent(PhysicsBody* body) {
    if (!body || !m_showQuadtree)
        return;
    m_debugBodiesAdditionDisplay.push_back(body);
}

void PhysicsWorld::addBodyQuadtreeUpdateEvent(PhysicsBody* body) {
    if (!body || !m_showQuadtree)
        return;
    m_debugBodiesUpdateDisplay.push_back({body, nullptr});
}

std::uint64_t PhysicsWorld::generateBodyId() {
    std::cerr << "Body id generated: " << m_currentBodyId << "\n";
    return m_currentBodyId++;
}

std::unique_ptr<std::vector<PhysicsWorld::Collision>> PhysicsWorld::checkCollision(PhysicsBody* body, sf::Vector2f const& anchor, bool recursiveSearch) {
    body->move(anchor);

    auto collisions {checkCollision(body, &m_root)};
    
    body->move(sf::Vector2f{-anchor.x, -anchor.y});
    
    return collisions;
}

int PhysicsWorld::getBodiesCount() {
    return m_bodiesCount;
}

std::unique_ptr<sf::RenderTexture> PhysicsWorld::getPhysicsTexture(float width, float height, sf::Vector2f const& anchor) {
    // Physics texture
    auto texture {std::make_unique<sf::RenderTexture>()};
    texture->create(width, height);
    texture->clear(sf::Color::Transparent);
    
    if (!m_showPhysicsBodies && !m_showAABBs && !m_showOBBs && !m_showCollisions && !m_showQuadtree && !m_showQuadtreeEvents) {
        return texture;
    }
    
    sf::Vector2f physicsShapesAnchor {
        -anchor.x + width / 2.f,
        -anchor.y + height / 2.f
    };
    
    std::vector<PhysicsBody*> toDrawBodies;
    auto nodesTexture {std::make_unique<sf::RenderTexture>()};
    nodesTexture->create(width, height);
    nodesTexture->clear(sf::Color::Transparent);
    
    std::function<void(QuadtreeNode*, PhysicsWorld*, std::unique_ptr<sf::RenderTexture>&, sf::Vector2f const&, std::vector<PhysicsBody*>&)> drawNode = [](QuadtreeNode* node, PhysicsWorld* world, std::unique_ptr<sf::RenderTexture>& texture, sf::Vector2f const& physicsShapesAnchor, std::vector<PhysicsBody*>& toDrawBodies) {
        if (world->m_showQuadtree) {
            sf::Color nodeShapeFillColor = DEBUG_QUADTREE_NODES_COLOR;
            
            // Displays quadtree events
            if (world->m_showQuadtreeEvents) {
                // Displays node as debug node where body update happened
                for (auto const& debugUpdateStruct : world->m_debugBodiesUpdateDisplay) {
                    if (debugUpdateStruct.second && *debugUpdateStruct.second == *node) {
                        nodeShapeFillColor = DEBUG_QUADTREE_UPDATE_COLOR;
                        break;
                    }
                }
                
                // Displays node as debug node where body addition happened
                for (auto const& debugBody : world->m_debugBodiesAdditionDisplay) {
                    if (debugBody && *debugBody->getParentNode() == *node) {
                        nodeShapeFillColor = DEBUG_QUADTREE_ADDITION_COLOR;
                        break;
                    }
                }
            }
            
            sf::Vector2f shapePosition {
                node->box.origin.x + physicsShapesAnchor.x,
                node->box.origin.y + physicsShapesAnchor.y
            };
            
            sf::RectangleShape nodeShape {sf::Vector2f{node->box.width, node->box.height}};
            nodeShape.setPosition(shapePosition);
            nodeShape.setFillColor(nodeShapeFillColor);
            nodeShape.setOutlineColor(sf::Color::Black);
            
            nodeShape.setOutlineThickness(3.f);
            texture->draw(nodeShape);
        }
        
        if (world->m_showPhysicsBodies || world->m_showAABBs || world->m_showOBBs) {
            for (auto& body : node->bodies) {
                toDrawBodies.push_back(body);
            }
        }
    };
    
    forEachNode<PhysicsWorld*, std::unique_ptr<sf::RenderTexture>&, sf::Vector2f const&, std::vector<PhysicsBody*>&>(drawNode, this, nodesTexture, physicsShapesAnchor, toDrawBodies);
    
    // Draws bodies sprites
    for (auto& body : toDrawBodies) {
        if (m_showPhysicsBodies) {
            auto bodySprite {body->getBodySprite(physicsShapesAnchor)};
            texture->draw(bodySprite);
        }
        
        if (m_showAABBs) {
            auto& bodyShape {body->getAABBShape(physicsShapesAnchor)};
            texture->draw(bodyShape);
        }
        
        if (m_showOBBs) {
            // TODO
        }
    }
    
    // Shows collisisons
    if (m_showCollisions) {
        for (auto& collision : m_debugCollisions) {
            for (auto& collisionPosition : *collision.second) {
                sf::Vector2f debugSpritePosition {
                    collisionPosition.x - 10.f - anchor.x + width / 2.f,
                    collisionPosition.y - 10.f - anchor.y + height / 2.f
                };
                sf::CircleShape debugCollisionSprite {10.f};
                debugCollisionSprite.setFillColor(DEBUG_COLLISION_FILL_COLOR);
                debugCollisionSprite.setOutlineColor(DEBUG_COLLISION_OUTLINE_COLOR);
                debugCollisionSprite.setOutlineThickness(-5.f);
                debugCollisionSprite.setPosition(debugSpritePosition);
                texture->draw(debugCollisionSprite);
            }
        }
    }
    
    // Draws nodes sprites
    nodesTexture->display();
    
    sf::Sprite nodesSprite;
    nodesSprite.setTexture(nodesTexture->getTexture());
    nodesSprite.setPosition(0.f, 0.f);
    texture->draw(nodesSprite);
    
    texture->display();
    
    return texture;
}

void PhysicsWorld::simulate() {
    // Clears bodies in addition buffer
    addAllBodies();
    // clears bodies in remove buffer
    removeAllBodies();
    
    // Computes collisions
    m_debugCollisions.clear();
    m_computedCollisionsCount = 0;
    
    std::function<void(QuadtreeNode*, int&)> computeCollisionsInNode = [this](QuadtreeNode* node, int& computedCollisionsCount) -> void {
        for (auto& body : node->bodies) {
            computedCollisionsCount++;
            auto collisions {checkCollision(body)};
            
            if (collisions->size() == 0)
                continue;
            
            // Throws callbacks
            for (auto& collision : *collisions) {
                body->didCollide(collision);
                
                PhysicsBody* otherBody = collision.first;
                collision.first = body;
                otherBody->didCollide(collision);
                
                collision.first = otherBody;
            }

            if (m_showCollisions) {
                for (int i = 0; i < collisions->size(); ++i) {
                    m_debugCollisions.emplace_back(std::move((*collisions)[i]));
                }
            }
        }
    };
    
    forEachNode<int&>(computeCollisionsInNode, m_computedCollisionsCount);
}

void PhysicsWorld::setShowPhysicsBodies(bool show) {
    m_showPhysicsBodies = show;
}

void PhysicsWorld::setShowAABBs(bool show) {
    m_showAABBs = show;
}

void PhysicsWorld::setShowOBBs(bool show) {
    m_showOBBs = show;
}

void PhysicsWorld::setShowCollisions(bool show) {
    m_showCollisions = show;
}

void PhysicsWorld::setShowQuadtree(bool show) {
    m_showQuadtree = show;
}

void PhysicsWorld::setShowQuadtreeEvents(bool show) {
    m_showQuadtreeEvents = show;
}

void PhysicsWorld::addBody(PhysicsBody* body, QuadtreeNode* node) {
    if (!node)
        return;
    
    // Adds body to parent node
    if (!isBodyInsideNode(body, node)) {
        if (!node->parent) {
            addParent(node, body->getCenter());
            addBody(body, node);
            
        } else {
            addBody(body, node->parent);
        }
        return;
    }
    
    // Adds body to child node
    if (node->hasChildren()) {
        for (int childId = 0; childId < 4; ++childId) {
            if (isBodyInsideNode(body, node->childs[childId].get())){
                addBody(body, node->childs[childId].get());
                return;
            }
        }
    }
    
    // Adds body to current node
    if (node->bodies.size() < MAX_BODIES_PER_NODE || node->hasChildren()) {
        node->bodies.emplace_back(body);
        body->setParentNode(node);
        return;
    }
    
    // Adds children if needed
    addChildrens(node);
    addBody(body, node);
}

void PhysicsWorld::addAllBodies() {
    for (auto body : m_toAddBodies) {
        if (!body)
            continue;
        ++m_bodiesCount;
        addBody(body, &m_root);
    }
    m_toAddBodies.clear();
}

void PhysicsWorld::removeAllBodies() {
    bool invalidState = false;
    
    auto compareLocations = [](QuadtreeLocation const& location1, QuadtreeLocation const& location2) -> bool {
        if (location1.first->box.width != location2.first->box.height) {
            return location1.first->box.width < location2.first->box.height;
        } else if (location1.first->box.origin.x != location2.first->box.origin.x) {
            return location1.first->box.origin.x < location2.first->box.origin.x;
        } else if (location1.first->box.origin.y != location2.first->box.origin.y) {
            return location1.first->box.origin.y < location2.first->box.origin.y;
        } else {
            return location1.second > location2.second;
        }
    };
    
    std::sort(m_toRemoveBodiesPositions.begin(), m_toRemoveBodiesPositions.end(), compareLocations);
    
    for (int locationId = 0; locationId < m_toRemoveBodiesPositions.size(); ++locationId) {
        auto& location {m_toRemoveBodiesPositions[locationId]};
        
        if (location.second >= location.first->bodies.size()) {
            invalidState = true;
            continue;
        }
        
        auto node = location.first;
        std::swap(node->bodies[location.second], node->bodies.back());
        node->bodies.pop_back();
        
        --m_bodiesCount;
    }
    
    m_toRemoveBodiesPositions.clear();
    
    if (invalidState)
        reorderBodies();
}

void PhysicsWorld::reorderBodies() {
    std::function<void(QuadtreeNode*)> reorderNode = [](QuadtreeNode* node) {
        std::remove_if(node->bodies.begin(), node->bodies.end(), [](PhysicsBody* body) {
            return !body;
        });
    };
    forEachNode(reorderNode);
}

void PhysicsWorld::addChildrens(QuadtreeNode* node) {
    const float subNodeWidth = node->box.width / 2.f;
    const float subNodeHeigth = node->box.height / 2.f;

    node->childs[0] = std::make_unique<QuadtreeNode>(
            AABB{node->box.origin, subNodeWidth, subNodeHeigth}, node->depth + 1, node
    );
    node->childs[1] = std::make_unique<QuadtreeNode>(
            AABB{sf::Vector2f{node->box.origin.x + subNodeWidth, node->box.origin.y}, subNodeWidth, subNodeHeigth}, node->depth + 1, node
    );
    node->childs[2] = std::make_unique<QuadtreeNode>(
            AABB{sf::Vector2f{node->box.origin.x, node->box.origin.y + subNodeHeigth}, subNodeWidth, subNodeHeigth}, node->depth + 1, node
    );
    node->childs[3] = std::make_unique<QuadtreeNode>(
            AABB{sf::Vector2f{node->box.origin.x + subNodeWidth, node->box.origin.y + subNodeHeigth}, subNodeWidth, subNodeHeigth}, node->depth + 1, node
    );
    
    std::vector<PhysicsBody*> bodies;
    std::swap(bodies, node->bodies);
    for (int bodyId = 0; bodyId < bodies.size(); ++bodyId) {
        addBody(bodies[bodyId], node);
    }
}

void PhysicsWorld::addParent(QuadtreeNode* node, sf::Vector2f const& bodyPosition) {
    float width = node->box.width;
    float height = node->box.height;
    
    float extendedWidth = width * 2.f;
    float extendedHeight = height * 2.f;
    
    sf::Vector2f originDirection {
        (bodyPosition.x < node->box.origin.x + node->box.width / 2.f ? -1.f : 0.f),
        (bodyPosition.y < node->box.origin.y + node->box.height / 2.f ? -1.f : 0.f)
    };
    
    QuadtreeNode newRoot {AABB{sf::Vector2f{
        width * originDirection.x + node->box.origin.x,
        height * originDirection.y + node->box.origin.y
    }, extendedWidth, extendedHeight}, 0};
    
    for (int dy = 0; dy < 2; ++dy) {
        for (int dx = 0; dx < 2; ++dx) {
            sf::Vector2f subAnchor {
                (originDirection.x + dx) * width + node->box.origin.x,
                (originDirection.y + dy) * height + node->box.origin.y
            };
            
            if (subAnchor == node->box.origin) {
                auto newNode {std::make_unique<QuadtreeNode>(AABB{subAnchor, width, height}, 1, node)}; // prevents errors from newRoot move
                newNode->bodies = std::move(node->bodies);
                newNode->childs = std::move(node->childs);
                
                if (newNode->hasChildren()) {
                    for (auto& child : newNode->childs) {
                        child->parent = newNode.get();
                    }
                }
                
                for (auto& body : newNode->bodies) {
                    body->setParentNode(newNode.get());
                }
                
                std::stack<QuadtreeNode*> toIncreaseDepthNode;
                toIncreaseDepthNode.push(newNode.get());
                while (!toIncreaseDepthNode.empty()) {
                    auto n = toIncreaseDepthNode.top();
                    toIncreaseDepthNode.pop();
                    if (n->hasChildren()) {
                        for (int i = 0; i < 4; ++i) {
                            ++n->childs[i].get()->depth;
                            toIncreaseDepthNode.push(n->childs[i].get());
                        }
                    }
                }
                
                newRoot.childs[dx + dy * 2] = std::move(newNode);
            } else {
                newRoot.childs[dx + dy * 2] = std::make_unique<QuadtreeNode>(AABB{subAnchor, width, height}, 1, node); // prevents errors from newRoot move
            }
        }
    }
    
    *node = std::move(newRoot);
}

int PhysicsWorld::getPreciseBodiesCount(bool checkvalidity) {
    int bodiesCount = 0;
    std::function<void(QuadtreeNode*, int&)> countBodies;
    
    if (!checkvalidity) {
         countBodies = [](QuadtreeNode* node, int& bodiesCount) {
            bodiesCount += node->bodies.size();
        };
    } else {
        countBodies = [](QuadtreeNode* node, int& bodiesCount) {
            bodiesCount += std::count_if(node->bodies.begin(), node->bodies.end(), [](PhysicsBody* body) -> bool {return body;});
        };
    }
    
    forEachNode<int&>(countBodies, bodiesCount);
    return bodiesCount;
}

int PhysicsWorld::getIntersectionsCount() const {
    int intersectionsCount = 0;
    for (auto& collision : m_debugCollisions) {
        intersectionsCount += collision.second->size();
    }
    return intersectionsCount;
}

int PhysicsWorld::getQuadtreeNodesCount() {
    int nodesCount = 0;
    std::function<void(QuadtreeNode*, int&)> countNodes = [this](QuadtreeNode* node, int& nodesCount) {
        ++nodesCount;
    };
    forEachNode<int&>(countNodes, nodesCount);
    return nodesCount;
}

int PhysicsWorld::getQuadtreeMaxDepth() {
    int maxDepth = 0;
    std::function<void(QuadtreeNode*, int&)> findMaxDepth = [this](QuadtreeNode* node, int& maxDepth) {
        maxDepth = std::max(maxDepth, node->depth);
    };
    forEachNode<int&>(findMaxDepth, maxDepth);
    return maxDepth;
}

std::unique_ptr<std::vector<PhysicsWorld::Collision>> PhysicsWorld::checkCollision(PhysicsBody* body, QuadtreeNode* node, bool recursiveSearch) {
    auto collisions {std::make_unique<std::vector<Collision>>()};
    
    if (!node)
        return collisions;
    
    if (!body)
        return collisions;
    
    if (body->getContactTestMasksCount() == 0 || body->getCategoryMask() == 0)
        return collisions;
    
    // Computes collisions inside current node
    for (auto bodyB : node->bodies) {
        // Prevents body from colliding with itself
        if (*body == *bodyB)
            continue;
        
        // Prevents non-requested collisions tests
        if (!body->shouldTestCollisionWithMask(bodyB->getCategoryMask()))
            continue;
        
        // Computes collision
        auto intersections {bodyB->collideWith(body)};
        if (intersections->size() == 0)
            continue;
        collisions->push_back({bodyB, std::move(intersections)});
    }
    
    if (!recursiveSearch)
        return collisions;
    
    // Computes collisions inside child nodes
    if (node->hasChildren()) {
        for (int childId = 0; childId < 4; ++childId) {
            QuadtreeNode* childNode = node->childs[childId].get();
            
            // Prevents search when collision is impossible
            if (!(isCollisionBetweenAABB(body->getFrame(), childNode->box) || body->isCollidingWithAABB(childNode->box)))
                continue;
            
            auto newCollisions {checkCollision(body, node->childs[childId].get())};
            for (int collisionId = 0; collisionId < newCollisions->size(); ++collisionId) {
                collisions->emplace_back(std::move((*newCollisions)[collisionId]));
            }
        }
    }
    
    return collisions;
}

std::optional<PhysicsWorld::QuadtreeLocation> PhysicsWorld::findBody(PhysicsBody* body, QuadtreeNode* rootNode) {
    QuadtreeNode* currentNode = rootNode;
    bool nextNodeFound = true;
    
    while (currentNode && nextNodeFound) {
        for (int i = 0; i < currentNode->bodies.size(); ++i) {
            if (*body == *currentNode->bodies[i]) {
                return std::make_optional(std::make_tuple(currentNode, i));
            }
        }
        
        if (!(currentNode->hasChildren()))
            break;
        
        nextNodeFound = false;
        for (int i = 0; i < 4; ++i) {
            if (isBodyInsideNode(body, currentNode->childs[i].get())) {
                currentNode = currentNode->childs[i].get();
                nextNodeFound = true;
                break;
            }
        }
    }
    return std::nullopt;
}

const sf::Color PhysicsWorld::DEBUG_COLLISION_FILL_COLOR = sf::Color{255, 0, 0, 255};
const sf::Color PhysicsWorld::DEBUG_COLLISION_OUTLINE_COLOR = sf::Color{19, 64, 23, 255};
const sf::Color PhysicsWorld::DEBUG_QUADTREE_NODES_COLOR = sf::Color{255, 140, 255, 75};

const sf::Color PhysicsWorld::DEBUG_QUADTREE_ADDITION_COLOR = sf::Color(255, 0, 0, 150);
const sf::Color PhysicsWorld::DEBUG_QUADTREE_UPDATE_COLOR = sf::Color(255, 255, 0, 200);

}
