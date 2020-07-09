//
//  Tile.hpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#ifndef Tile_hpp
# define Tile_hpp

# include <memory>

# include "TileModel.hpp"
# include "../DrawableEntity.hpp"
# include "../../../../Physics/PhysicsBody.hpp"

namespace mz {

class Tile : public DrawableEntity {
public:
    Tile(float x, float y, std::shared_ptr<TileModel> model);
    
    ~Tile();
    
    virtual std::shared_ptr<sf::RenderTexture> draw();
    
    PhysicsBody* getPhysicsBody();
    
    virtual void update(sf::Time timeElapsed);
    
private:
    std::shared_ptr<TileModel>  m_model;
    PhysicsBody*                m_physicsBody;
};

}

#endif /* Tile_hpp */
