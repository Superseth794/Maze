//
//  Tile.hpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#ifndef Tile_hpp
# define Tile_hpp

# include <memory>

# include "../DrawableEntity.hpp"
# include "TileModel.hpp"

namespace mz {

class Tile : public DrawableEntity {
public:
    Tile(float x, float y, std::shared_ptr<TileModel> model);
    
    virtual void update(sf::Time timeElapsed);
    virtual std::shared_ptr<sf::RenderTexture> draw();
    
private:
    std::shared_ptr<TileModel> m_model;
};

}

#endif /* Tile_hpp */
