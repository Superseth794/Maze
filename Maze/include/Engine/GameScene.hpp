//
//  GameScene.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <SFML/Graphics.hpp>

#include "Camera.hpp"
#include "Layer.hpp"
#include "Node.hpp"
#include "../Physics/PhysicsWorld.hpp"
#include "../../Config.hpp"

namespace mz {

class GameScene {
public:
    GameScene(unsigned int width, unsigned int height);
    GameScene(GameScene const& scene) = delete;
    GameScene(GameScene && scene) = delete;
    
    GameScene operator=(GameScene const& scene) = delete;
    GameScene operator=(GameScene && scene) = delete;
    
    void display(sf::RenderTarget& texture);
    
private:
    Camera              m_camera;
    Layer               m_mainLayer;
    PhysicsWorld        m_physicsWorld;
};

}

#endif /* GameScene_hpp */
