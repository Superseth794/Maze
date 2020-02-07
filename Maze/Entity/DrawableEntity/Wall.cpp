//
//  Wall.cpp
//  Maze
//
//  Created by Jaraxus on 07/02/2020.
//

#include "Wall.hpp"

namespace mz {

Wall::Wall(float width, float height, float x, float y) :
m_width(width),
m_height(height),
m_bodyShape()
{
    move({x, y});
    
    m_bodyShape.setSize({m_width, m_height});
    m_bodyShape.setPosition(0.f, 0.f);
    updateTexture();
}

void Wall::update(sf::Time timeElapsed) {
    // Empty for now
}

std::unique_ptr<sf::RenderTexture> Wall::draw() {
    auto texture {std::make_unique<sf::RenderTexture>()};
    texture->create(m_width, m_height);
    texture->clear(sf::Color::Transparent);
    texture->draw(m_bodyShape);
    texture->display();
    return texture;
}

void Wall::setFilled(bool filled) {
    if (m_isFilled != filled) {
        m_isFilled = filled;
        updateTexture();
    }
}

bool Wall::isFilled() {
    return m_isFilled;
}

void Wall::updateTexture() {
    m_bodyShape.setFillColor((m_isFilled ? sf::Color::White : sf::Color::Black));
}


}
