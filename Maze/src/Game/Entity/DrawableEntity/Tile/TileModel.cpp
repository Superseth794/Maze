//
//  TileModel.cpp
//  Maze
//
//  Created by Jaraxus on 12/02/2020.
//

#include "../../../../../include/Game/Entity/DrawableEntity/Tile/TileModel.hpp"

namespace mz {

TileModel::TileModel(float width, float height, std::shared_ptr<sf::RenderTexture> && texture, PhysicsBody* physicsBody) :
m_width(width),
m_height(height),
m_texture(std::move(texture)),
m_physicsBody(physicsBody)
{
}

TileModel::~TileModel() {
    if (m_physicsBody)
        delete m_physicsBody;
}

}
