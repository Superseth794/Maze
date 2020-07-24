//
//  GameScene.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <SFML/Graphics.hpp>

#include "Layer.hpp"
#include "Node.hpp"
#include "../Physics/PhysicsWorld.hpp"
#include "../../Config.hpp"

namespace mz {

class GameScene {
public:
    GameScene(float width, float height);
    GameScene(GameScene const& scene) = delete;
    GameScene(GameScene && scene) = delete;
    
    GameScene operator=(GameScene const& scene) = delete;
    GameScene operator=(GameScene && scene) = delete;
    
    sf::Sprite display();
    
private:
    float               m_height;
    Layer               m_mainLayer;
    PhysicsWorld        m_physicsWorld;
    sf::RenderTexture   m_texture;
    float               m_width;
};

}

#endif /* GameScene_hpp */
