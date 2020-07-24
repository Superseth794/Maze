//
//  PhysicsWorld.hpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#ifndef PhysicsWorld_hpp
#define PhysicsWorld_hpp

#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

#include "AABB.hpp"
#include "Collisions.hpp"
#include "PhysicsBody.hpp"
#include "Collisions.hpp"

namespace mz {

class Console;
class PhysicsBody;

struct QuadtreeNode {
    std::vector<PhysicsBody*>                       bodies;
    AABB                                            box;
    std::array<std::unique_ptr<QuadtreeNode>, 4>    childs; // vector
    int                                             depth;
    QuadtreeNode*                                   parent;
    
    QuadtreeNode(AABB const& _box, int _depth = 0, QuadtreeNode* _parent = nullptr) : box(_box), depth(_depth), parent(_parent) {}
    
    bool operator==(QuadtreeNode const& node) const {
        return (box.origin == node.box.origin && depth == node.depth);
    }
    
    bool hasChildren() {
        return (childs[0].get());
    }
};

bool isBodyInsideNode(PhysicsBody* body, QuadtreeNode* node);

class PhysicsWorld {
    
    using Collision = std::pair<PhysicsBody*, std::unique_ptr<std::vector<sf::Vector2f>>>;
    using QuadtreeLocation = std::pair<QuadtreeNode*, std::size_t>;
    
    friend Console;
    
public:
    PhysicsWorld();
    
    ~PhysicsWorld();
    
    void addBody(PhysicsBody* body);
    
    void addBodyQuadtreeAdditionEvent(PhysicsBody* body);
    
    void addBodyQuadtreeUpdateEvent(PhysicsBody* body);
    
    std::unique_ptr<std::vector<Collision>> checkCollision(PhysicsBody* body, sf::Vector2f const& anchor = sf::Vector2f{0.f, 0.f}, bool recursiveSearch = true);
    
    std::uint64_t generateBodyId();
    
    int getBodiesCount();
    
    std::unique_ptr<sf::RenderTexture> getPhysicsTexture(float width, float height, sf::Vector2f const& anchor);
    
    void init(float width, float height);
    
    bool removeBody(PhysicsBody* body);
    
    void setShowAABBs(bool show);
    
    void setShowCollisions(bool show);
    
    void setShowOOBBs(bool show);
    
    void setShowPhysicsBodies(bool show);
    
    void setShowQuadtree(bool show);
    
    void setShowQuadtreeEvents(bool show);
    
    void simulate();
    
    void updateBody(PhysicsBody* body);
    
private:
    void addAllBodies();
    
    void addBody(PhysicsBody* body, QuadtreeNode* node);
    
    void addChildrens(QuadtreeNode* node);
    
    void addParent(QuadtreeNode* node, sf::Vector2f const& bodyPosition);
    
    std::unique_ptr<std::vector<Collision>> checkCollision(PhysicsBody* body, QuadtreeNode* node, bool recursiveSearch = true);
    
    std::optional<QuadtreeLocation> findBody(PhysicsBody* body, QuadtreeNode* rootNode);
    
    int getQuadtreeMaxDepth(); // TODO const;
    
    int getQuadtreeNodesCount(); // TODO const;
    
    int getIntersectionsCount() const;
    
    int getPreciseBodiesCount(bool checkvalidity = false); // TODO const;
    
    void removeAllBodies();
    
    void reorderBodies();
    
    void updateAllBodies();
    
private:
    template<typename... Args> void forEachNode(std::function<void(QuadtreeNode*, Args...)>& func, Args... args) { // TODO: improve template
        std::stack<QuadtreeNode*> toInspectNodes;
        toInspectNodes.push(&m_root);
        while (!toInspectNodes.empty()) {
            QuadtreeNode* currentNode = toInspectNodes.top();
            toInspectNodes.pop();
            func(currentNode, args...);
            if (currentNode->hasChildren()) {
                for (int i = 0; i < 4; ++i)
                    toInspectNodes.push(currentNode->childs[i].get());
            }
        }
    }
    
private:
    int                                                 m_bodiesCount                   = 0;
    int                                                 m_computedCollisionsCount       = 0;
    std::uint64_t                                       m_currentBodyId                 = 1;
    std::vector<PhysicsBody*>                           m_debugBodiesAdditionDisplay; // TODO: remove debug from name
    std::vector<std::pair<PhysicsBody*, QuadtreeNode*>> m_debugBodiesUpdateDisplay; // TODO: remove debug from name
    std::vector<Collision>                              m_debugCollisions; // TODO: remove debug from name
    QuadtreeNode                                        m_root;
    bool                                                m_showAABBs                     = false;
    bool                                                m_showCollisions                = false;
    bool                                                m_showQuadtree                  = false;
    bool                                                m_showQuadtreeEvents            = false;
    bool                                                m_showOBBs                      = false;
    bool                                                m_showPhysicsBodies             = false;
    std::vector<PhysicsBody*>                           m_toAddBodies;
    std::vector<QuadtreeLocation>                       m_toRemoveBodiesPositions;
    std::vector<PhysicsBody*>                           m_toUpdateBodies;
    
    static const sf::Color                              s_debugQuadtreeNodesColor; // TODO: remove debug from name
    static const sf::Color                              s_debugCollisionFillColor; // TODO: remove debug from name
    static const sf::Color                              s_debugCollisionOutlineColor; // TODO: remove debug from name
    static const sf::Color                              s_debugQuadtreeAddtitionColor; // TODO: remove debug from name
    static const sf::Color                              s_debugQuadtreeUpdateColor; // TODO: remove debug from name
    static const float                                  s_debugCollisionWidth; // TODO: remove debug from name
    static constexpr std::size_t                        s_maxBodiesPerNode              = 10;
};

}

#endif /* PhysicsWorld_hpp */
