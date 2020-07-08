//
//  Player.hpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#ifndef Player_hpp
#define Player_hpp

# include <cmath>
# include <unordered_set>
# include <iostream>

# include "../../../System/Orientation.hpp"
# include "DrawableEntity.hpp"
# include "../../../Physics/PhysicsWorld.hpp"
# include "../../../Physics/CirclePhysicsBody.hpp"

namespace mz {

class Player : public DrawableEntity {
public:
    Player(float width, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld);
    
    virtual std::shared_ptr<sf::RenderTexture> draw();
    
    PhysicsBody* getPhysicsBody();
    
    void orientedMove(Orientation const& orientation, bool isMoving);
    
    virtual void update(sf::Time timeElapsed);
    
private:
    sf::CircleShape                     m_bodyShape;
    std::unordered_set<std::string>     m_directions;
    CirclePhysicsBody                   m_physicsBody;
    float                               m_speed = 5.f;
    std::shared_ptr<sf::RenderTexture>  m_texture;
    float                               m_width;
};

}

#endif /* Player_hpp */
