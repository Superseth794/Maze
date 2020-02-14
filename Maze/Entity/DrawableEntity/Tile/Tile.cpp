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
    if (m_physicsBody) {
        auto texture {std::make_shared<sf::RenderTexture>()};
        texture->create(m_model->m_width, m_model->m_height);
        texture->clear();
        
        sf::Sprite sprite{};
        sprite.setTexture(m_model->m_texture->getTexture());
        sprite.setPosition(0.f, 0.f);
        texture->draw(sprite);
        
        sf::RectangleShape physicsBodyShape(sf::Vector2f{m_physicsBody->getFrame().width, m_physicsBody->getFrame().height});
        physicsBodyShape.setFillColor(sf::Color(101, 184, 127, 125));
        physicsBodyShape.setPosition(m_physicsBody->getCenter().x - m_physicsBody->getFrame().width / 2.f - getPosition().x, m_physicsBody->getCenter().y - m_physicsBody->getFrame().height / 2.f - getPosition().y);
        texture->draw(physicsBodyShape);
        
        texture->display();
        return texture;
    } else {
        return m_model->m_texture;
    }
}

PhysicsBody* Tile::getPhysicsBody() {
    return m_physicsBody;
}

}
