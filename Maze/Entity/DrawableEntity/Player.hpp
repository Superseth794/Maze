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

# include "../../Utils/Orientation.hpp"
# include "DrawableEntity.hpp"

namespace mz {

class Player : public DrawableEntity {
public:
    Player(float width);
    
    virtual void update(sf::Time timeElapsed);
    virtual std::shared_ptr<sf::RenderTexture> draw();
    
    void orientedMove(Orientation const& orientation, bool isMoving);
    
private:
    float m_width;
    float m_speed = 20.f;
    
    std::unordered_set<std::string> m_directions;
    
    sf::CircleShape m_bodyShape;
};


}

#endif /* Player_hpp */
