//
//  Wall.hpp
//  Maze
//
//  Created by Jaraxus on 07/02/2020.
//

#ifndef Wall_hpp
# define Wall_hpp

# include "DrawableEntity.hpp"

namespace mz {

class Wall : public DrawableEntity {
public:
    Wall(float width, float height, float x, float y);
    
    virtual void update(sf::Time timeElapsed);
    virtual std::unique_ptr<sf::RenderTexture> draw();
    
    void setFilled(bool filled);
    bool isFilled();
    
private:
    void updateTexture();
    
private:
    float m_width, m_height;
    bool m_isFilled = false;
    
    sf::RectangleShape m_bodyShape;
};


}

#endif /* Wall_hpp */
