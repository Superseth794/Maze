//
//  Entity.hpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#ifndef Entity_hpp
#define Entity_hpp

# include <cmath>
# include <iostream>

# include <SFML/Graphics.hpp>

namespace mz {

class Entity {
public:
    Entity();
    virtual ~Entity() = default;
    
    Entity(Entity const& entity) = delete;
    Entity(Entity && entity) = delete;
    Entity& operator=(Entity const& entity) = delete;
    
    sf::Vector2f getPosition() const;
    
    void move(sf::Vector2f direction);
    void moveX(float dx);
    void moveY(float dy);
    
    virtual void update(sf::Time timeElapsed) = 0;
    
private:
    float m_x;
    float m_y;
};


}

#endif /* Entity_hpp */
