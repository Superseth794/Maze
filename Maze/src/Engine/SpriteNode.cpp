//
//  SpriteNode.cpp
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

#include "../../include/Engine/SpriteNode.hpp"

namespace mz {

SpriteNode::SpriteNode(sf::Sprite && sprite, std::shared_ptr<sf::Texture> && texture) :
m_sprite(std::forward<sf::Sprite>(sprite)),
m_texture(std::forward<std::shared_ptr<sf::Texture>>(texture))
{
}

void SpriteNode::draw(Camera const& camera) {
    camera.draw(m_sprite, getGlobalTransform());
}

}
