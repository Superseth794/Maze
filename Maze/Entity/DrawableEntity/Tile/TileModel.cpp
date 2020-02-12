//
//  TileModel.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "TileModel.hpp"

namespace mz {

TileModel::TileModel(float width, float height, std::shared_ptr<sf::RenderTexture> && texture) :
m_width(width),
m_height(height),
m_texture(std::move(texture))
{
    
}

}
