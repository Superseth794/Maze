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

# include <SFML/Graphics.hpp>

# include "PhysicsBody.hpp"

namespace mz {

class PhysicsBody;

struct Node {
    sf::Vector2f origin;
    sf::Vector2f size;
    std::array<std::unique_ptr<Node>, 4> childs;
    std::vector<PhysicsBody*> bodies;
};

class PhysicsWorld {
    
    using Collision = std::tuple<PhysicsBody*, std::unique_ptr<std::vector<sf::Vector2f>>>;
    
public:
    PhysicsWorld(bool showPhysics);
    
    void addBody(PhysicsBody* body);
    void removeBody(PhysicsBody* body);
    
    std::unique_ptr<std::vector<Collision>> checkCollision(PhysicsBody* body, sf::Vector2f const& anchor = sf::Vector2f{0.f, 0.f});
    
    int getBodiesCount();
    std::unique_ptr<sf::RenderTexture> getPhysicsDebugTexture(float width, float height, sf::Vector2f const& anchor);
    
    void simulate();
    
private:
    Node m_root;
    int m_bodiesCount = 0;
    
    bool m_showPhysics = false;
    std::vector<Collision> m_debugCollisions;
    
    static const sf::Color DEBUG_COLLISION_FILL_COLOR;
    static const sf::Color DEBUG_COLLISION_OUTLINE_COLOR;
};

}

#endif /* PhysicsWorld_hpp */
