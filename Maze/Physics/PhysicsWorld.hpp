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
    PhysicsWorld();
    
    void addBody(PhysicsBody* body);
    void removeBody(PhysicsBody* body);
    
    std::unique_ptr<std::vector<Collision>> checkCollision(PhysicsBody* body, sf::Vector2f const& anchor = sf::Vector2f{0.f, 0.f});
    
    int getBodiesCount();
    
private:
    Node m_root;
    int m_bodiesCount = 0;
};

}

#endif /* PhysicsWorld_hpp */
