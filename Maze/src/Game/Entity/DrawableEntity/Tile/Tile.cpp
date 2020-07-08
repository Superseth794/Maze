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
m_model(model),
m_physicsBody(model->m_physicsBody ? model->m_physicsBody->clone() : nullptr)
{
    move({x, y});
    if (m_physicsBody)
        m_physicsBody->setCenter({getPosition().x + m_model->m_width / 2.f, getPosition().y + m_model->m_width / 2.f});
}

Tile::~Tile() {
    if (m_physicsBody)
        delete m_physicsBody;
}

void Tile::update(sf::Time timeElapsed) {
    if (m_physicsBody)
        m_physicsBody->setCenter({getPosition().x + m_model->m_width / 2.f, getPosition().y + m_model->m_width / 2.f});
}

std::shared_ptr<sf::RenderTexture> Tile::draw() {
    return m_model->m_texture;
}

PhysicsBody* Tile::getPhysicsBody() {
    return m_physicsBody;
}

}
