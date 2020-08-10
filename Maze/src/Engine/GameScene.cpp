//
//  GameScene.cpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#include "../../include/Engine/GameScene.hpp"

mz::GameScene::GameScene(unsigned int width, unsigned int height) :
m_camera(width, height)
{
}

void mz::GameScene::display(sf::RenderTarget& texture) {
    m_camera.clear(sf::Color::Transparent);
    
    // display layers;
    m_mainLayer.draw(m_camera);
    
    // display physics TODO: use camera
#if defined MAZE_DEBUG
//    auto physicsTexture {m_physicsWorld.getPhysicsTexture(m_width, m_height, sf::Vector2f{0.f, 0.f})}; // TODO: change into sprite
//    sf::Sprite physicsSprite {};
//    physicsSprite.setTexture(physicsTexture->getTexture());
//    physicsSprite.setPosition(0.f, 0.f);
//    m_texture.draw(physicsSprite);
#endif
    
    return m_camera.display(texture);
}
