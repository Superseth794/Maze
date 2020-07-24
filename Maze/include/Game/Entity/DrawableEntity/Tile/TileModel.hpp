//
//  TileModel.hpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#ifndef TileModel_hpp
#define TileModel_hpp

#include <memory>

#include <SFML/Graphics.hpp>

#include "../../../../Physics/PhysicsBody.hpp"

namespace mz {

class Tile;

class TileModel {
    friend Tile;
    
public:
    TileModel(float width, float height, std::shared_ptr<sf::RenderTexture> && texture, PhysicsBody* physicsBody);
    
    ~TileModel();
    
private:
    float                               m_height;
    PhysicsBody*                        m_physicsBody;
    std::shared_ptr<sf::RenderTexture>  m_texture;
    float                               m_width;
};

}

#endif /* TileModel_hpp */
