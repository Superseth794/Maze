//
//  GameScene.cpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#include "../../include/Engine/GameScene.hpp"

mz::GameScene::GameScene(float width, float height) :
m_height(height),
m_width(width)
{
    m_texture.create(m_width, m_height);
}

sf::Sprite mz::GameScene::display() {
    m_texture.clear(sf::Color::Transparent);
    
    // display layers;
    
    // display physics
#if defined MAZE_DEBUG
    auto physicsTexture {m_physicsWorld.getPhysicsTexture(m_width, m_height, sf::Vector2f{0.f, 0.f})}; // TODO: change into sprite
    sf::Sprite physicsSprite {};
    physicsSprite.setTexture(physicsTexture->getTexture());
    physicsSprite.setPosition(0.f, 0.f);
    m_texture.draw(physicsSprite);
#endif
    
    m_texture.display();
    
    sf::Sprite sceneSprite;
    sceneSprite.setTexture(m_texture.getTexture());
    sceneSprite.setPosition(0, 0);
    return sceneSprite;
}
