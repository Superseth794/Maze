//
//  Camera.cpp
//  Maze
//
//  Created by Jaraxus on 10/08/2020.
//

#include "../../include/Engine/Camera.hpp"

namespace mz {

Camera::Camera(unsigned int width, unsigned int height) :
m_height(height),
m_width(width)
{
    m_texture = std::make_unique<sf::RenderTexture>();
    m_texture->create(width, height);
}

void Camera::display(sf::RenderTarget& texture) {
    m_texture->display();
    
    sf::Sprite cameraSprite;
    cameraSprite.setTexture(m_texture->getTexture());
    cameraSprite.setPosition(0, 0);
    
    texture.draw(cameraSprite);
}

}
