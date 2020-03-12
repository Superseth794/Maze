//
//  PhysicsWorld.hpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#ifndef PhysicsWorld_hpp
# define PhysicsWorld_hpp

# include <iostream>
# include <cmath>
# include <array>
# include <vector>
# include <tuple>
# include <queue>
# include <stack>
# include <utility>
# include <functional>
# include <optional>

# include <SFML/Graphics.hpp>

# include "AABB.hpp"
# include "PhysicsBody.hpp"

namespace mz {

class PhysicsBody;

class Console;

struct QuadtreeNode {
    AABB box;
    
    std::array<std::unique_ptr<QuadtreeNode>, 4> childs;
    std::vector<PhysicsBody*> bodies;
    QuadtreeNode* parent;
    int depth;
    
    QuadtreeNode(AABB const& box, int depth = 0, QuadtreeNode* parent = nullptr) {
        this->box = box;
        this->depth = depth;
        this->parent = parent;
    };
    
    bool hasChildren() {
        return (childs[0].get());
    }
    
    bool operator==(QuadtreeNode const& node) const {
        return (box.origin == node.box.origin && depth == node.depth);
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
    
    void init(float width, float height);
    
    void addBody(PhysicsBody* body);
    void removeBody(PhysicsBody* body);
    void updateBody(PhysicsBody* body);
    
    void addBodyQuadtreeAdditionEvent(PhysicsBody* body);
    void addBodyQuadtreeUpdateEvent(PhysicsBody* body);
    
    std::uint64_t generateBodyId();
    int getBodiesCount();
    
    std::unique_ptr<std::vector<Collision>> checkCollision(PhysicsBody* body, sf::Vector2f const& anchor = sf::Vector2f{0.f, 0.f}, bool recursiveSearch = true);
    
    std::unique_ptr<sf::RenderTexture> getPhysicsTexture(float width, float height, sf::Vector2f const& anchor);
    
    void simulate();
    
    void setShowPhysicsBodies(bool show);
    void setShowAABBs(bool show);
    void setShowOBBs(bool show);
    void setShowCollisions(bool show);
    void setShowQuadtree(bool show);
    void setShowQuadtreeEvents(bool show);
    
private:
    void addBody(PhysicsBody* body, QuadtreeNode* node);
    void addAllBodies();
    void removeAllBodies();
    void reorderBodies();
    
    void addChildrens(QuadtreeNode* node);
    void addParent(QuadtreeNode* node, sf::Vector2f const& bodyPosition);
    
    int getPreciseBodiesCount(bool checkvalidity = false); // TODO const;
    int getIntersectionsCount() const;
    int getQuadtreeNodesCount(); // TODO const;
    int getQuadtreeMaxDepth(); // TODO const;
    
    std::unique_ptr<std::vector<Collision>> checkCollision(PhysicsBody* body, QuadtreeNode* node, bool recursiveSearch = true);
    
    std::optional<QuadtreeLocation> findBody(PhysicsBody* body, QuadtreeNode* rootNode);
    
private:
    QuadtreeNode m_root;
    int m_bodiesCount = 0;
    std::uint64_t m_currentBodyId = 1;
    
    std::vector<QuadtreeLocation> m_toRemoveBodiesPositions;
    std::vector<PhysicsBody*> m_toAddBodies;
    
    bool m_showPhysicsBodies = false;
    bool m_showAABBs = false;
    bool m_showOBBs = false;
    bool m_showCollisions = false;
    bool m_showQuadtree = false;
    bool m_showQuadtreeEvents = false;
    
    int m_computedCollisionsCount = 0;
    std::vector<Collision> m_debugCollisions;
    std::vector<PhysicsBody*> m_debugBodiesAdditionDisplay;
    std::vector<std::pair<PhysicsBody*, QuadtreeNode*>> m_debugBodiesUpdateDisplay; // TODO remove debug
    
    static constexpr std::size_t MAX_BODIES_PER_NODE = 10;
    
    static const sf::Color DEBUG_QUADTREE_NODES_COLOR;
    static const sf::Color DEBUG_COLLISION_FILL_COLOR;
    static const sf::Color DEBUG_COLLISION_OUTLINE_COLOR;
    
    static const sf::Color DEBUG_QUADTREE_ADDITION_COLOR;
    static const sf::Color DEBUG_QUADTREE_UPDATE_COLOR;
    
private:
    template<typename... Args> void forEachNode(std::function<void(QuadtreeNode*, Args...)>& func, Args... args) {
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
};

}

#endif /* PhysicsWorld_hpp */
