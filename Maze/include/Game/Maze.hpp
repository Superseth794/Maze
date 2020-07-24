//
//  Maze.hpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#pragma once

#ifndef Maze_hpp
#define Maze_hpp

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <memory>
#include <stack>
#include <stdlib.h>
#include <vector>

#include <SFML/Graphics.hpp>

#include "EntitiesBitMasks.hpp"
#include "Entity/DrawableEntity/Player.hpp"
#include "Entity/DrawableEntity/Tile/Tile.hpp"
#include "../Core/Console.hpp"
#include "../Engine/GameScene.hpp"
#include "../Physics/PhysicsWorld.hpp"
#include "../Physics/RectanglePhysicsBody.hpp"

namespace mz {

//class Console;

class Maze {
    
    friend Console;
    
public:
    Maze(unsigned int width, unsigned int height);
    
    ~Maze() = default;
    
    Maze(Maze const& maze) = delete;
    
    Maze(Maze && maze) = delete;
    
    Maze& operator=(Maze const& maze) = delete;
    
    void lauch();
    
private:
    void display();
    
    void drawEntity(DrawableEntity const& entity, sf::Sprite & entitySprite);
    
    void generateMaze();
    
    void handleEvent(sf::Event const& event);
    
    void init();
    
    void update();
    
    void updateCamera();
    
private:
    sf::Vector2f                                m_cameraPosition;
    std::unique_ptr<Console>                    m_console;
    float                                       m_fps               = 60.f;
    sf::Clock                                   m_gameClock;
    unsigned int                                m_height;
    PhysicsWorld                                m_physicsWorld;
    Player                                      m_player;
    std::vector<std::unique_ptr<Tile>>          m_tiles;
    unsigned int                                m_width;
    sf::RenderWindow                            m_window;
    std::vector<std::unique_ptr<PhysicsBody>>   debug_bodies; // DEBUG
    
    static constexpr int                        s_mazeWidth         = 25;
    static constexpr int                        s_mazeHeight        = 25;
    static constexpr float                      s_wallWidth         = 240.f;
    static constexpr float                      s_wallHeight        = 240.f;
    static constexpr bool                       s_show_console      = true;
};

}

#endif /* Maze_hpp */
