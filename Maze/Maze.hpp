//
//  Maze.hpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#pragma once

#ifndef Maze_hpp
# define Maze_hpp

# include <iostream>
# include <cmath>
# include <algorithm>
# include <vector>
# include <array>
# include <memory>
# include <stack>
# include <stdlib.h>

# include <SFML/Graphics.hpp>

# include "Entity/DrawableEntity/Player.hpp"
# include "Entity/DrawableEntity/Tile/Tile.hpp"

# include "Physics/PhysicsWorld.hpp"
# include "Physics/RectanglePhysicsBody.hpp"

# include "Console.hpp"

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
    void init();
    void generateMaze();
    
    void update();
    void updateCamera();
    
    void display();
    void drawEntity(DrawableEntity const& entity, sf::Sprite & entitySprite);
    
    void handleEvent(sf::Event const& event);
    
private:
    unsigned int m_width;
    unsigned int m_height;
    static constexpr int m_mazeWidth = 25;
    static constexpr int m_mazeHeight = 25;
    static constexpr float m_wallWidth = 240.f;
    static constexpr float m_wallHeight = 240.f;
    
    sf::RenderWindow m_window;
    sf::Clock m_gameClock;
    float m_fps = 60.f;
    
    std::unique_ptr<Console> m_console;
    static constexpr bool SHOW_CONSOLE = true;
    
    sf::Vector2f m_cameraPosition;
    
    PhysicsWorld m_physicsWorld;
    
    Player m_player;
    std::vector<std::unique_ptr<Tile>> m_tiles;
    
    static constexpr std::uint32_t FILLED_TILE_CATEGORY_BITMASK = 1;
    static constexpr std::uint32_t PLAYER_CATEGORY_BITMASK = 2;
};


}

#endif /* Maze_hpp */
