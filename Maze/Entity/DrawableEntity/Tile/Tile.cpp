//
//  Tile.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "Tile.hpp"

namespace mz {

Tile::Tile(float x, float y, std::shared_ptr<TileModel> model) :
DrawableEntity(),
m_model(model)
{
    move({x, y});
}

void Tile::update(sf::Time timeElapsed) {
    // Empty for now
}

std::shared_ptr<sf::RenderTexture> Tile::draw() {
    return m_model->m_texture;
}

}
