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

# include "../../../Utils/Orientation.hpp"
# include "DrawableEntity.hpp"
# include "../../../Physics/PhysicsWorld.hpp"
# include "../../../Physics/CirclePhysicsBody.hpp"

namespace mz {

class Player : public DrawableEntity {
public:
    Player(float width, std::uint32_t categoryBitMask, PhysicsWorld* parentWorld);
    
    virtual void update(sf::Time timeElapsed);
    virtual std::shared_ptr<sf::RenderTexture> draw();
    
    void orientedMove(Orientation const& orientation, bool isMoving);
    
    PhysicsBody* getPhysicsBody();
    
private:
    float m_width;
    float m_speed = 5.f;
    
    std::unordered_set<std::string> m_directions;
    
    sf::CircleShape m_bodyShape;
    std::shared_ptr<sf::RenderTexture> m_texture;
    
    CirclePhysicsBody m_physicsBody;
};


}

#endif /* Player_hpp */
