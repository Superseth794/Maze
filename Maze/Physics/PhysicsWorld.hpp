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

# include <SFML/Graphics.hpp>

# include "AABB.hpp"
# include "PhysicsBody.hpp"

namespace mz {

class PhysicsBody;

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
    
    using Collision = std::tuple<PhysicsBody*, std::unique_ptr<std::vector<sf::Vector2f>>>;
    using QuadtreeLocation = std::pair<QuadtreeNode*, std::size_t>;
    
public:
    PhysicsWorld(bool showPhysics, bool showQuadtree);
    ~PhysicsWorld();
    
    void init(float width, float height);
    
    void addBody(PhysicsBody* body);
    void removeBody(PhysicsBody* body);
    void updateBody(PhysicsBody* body);
    
    void addBodyDebugAdditionDisplay(PhysicsBody* body);
    void addBodyDebugUpdateDispay(PhysicsBody* body);
    
    std::uint64_t generateBodyId();
    int getBodiesCount();
    
    std::unique_ptr<std::vector<Collision>> checkCollision(PhysicsBody* body, sf::Vector2f const& anchor = sf::Vector2f{0.f, 0.f});
    
    std::unique_ptr<sf::RenderTexture> getPhysicsDebugTexture(float width, float height, sf::Vector2f const& anchor);
    
    void simulate();
    
private:
    void addBody(PhysicsBody* body, QuadtreeNode* node);
    
    void addChildrens(QuadtreeNode* node);
    void addParent(QuadtreeNode* node, sf::Vector2f const& bodyPosition);
    
    std::unique_ptr<std::vector<Collision>> checkCollision(PhysicsBody* body, QuadtreeNode* node, bool recursiveSearch = true);
    
    std::tuple<bool, QuadtreeLocation> findBody(PhysicsBody* body, QuadtreeNode* rootNode);
    
private:
    QuadtreeNode m_root;
    int m_bodiesCount = 0;
    std::uint64_t m_currentBodyId = 1;
    
    std::vector<PhysicsBody*> m_toRemoveBodiesBuffer;
    
    bool m_showPhysics = false;
    bool m_showQuadtree = false;
    std::vector<Collision> m_debugCollisions;
    std::vector<PhysicsBody*> m_debugBodiesAdditionDisplay;
    std::vector<std::pair<PhysicsBody*, QuadtreeNode*>> m_debugBodiesUpdateDisplay;
    
    static constexpr std::size_t MAX_BODIES_PER_NODE = 10;
    
    static const sf::Color DEBUG_QUADTREE_NODES_COLOR;
    static const sf::Color DEBUG_COLLISION_FILL_COLOR;
    static const sf::Color DEBUG_COLLISION_OUTLINE_COLOR;
    
    static const sf::Color DEBUG_QUADTREE_ADDITION_COLOR;
    static const sf::Color DEBUG_QUADTREE_UPDATE_COLOR;
    
};

}

#endif /* PhysicsWorld_hpp */
