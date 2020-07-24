//
//  Console.hpp
//  Maze
//
//  Created by Jaraxus on 28/02/2020.
//

#pragma once

#ifndef DebugInfosWindow_hpp
# define DebugInfosWindow_hpp

# include <cmath>
# include <iostream>
# include <memory>
# include <sstream>

# include <SFML/Graphics.hpp>

# include "../Game/Maze.hpp"

namespace mz {

class Maze;
class PhysicsWorld;

class Console {
public:
    Console() = default;
    
    std::unique_ptr<sf::RenderTexture> display();
    
    void init(float width, float height, Maze* maze = nullptr, PhysicsWorld* world = nullptr);
    
private:
    sf::Font                    m_font;
    float                       m_height;
    bool                        initialized     = false;
    Maze*                       m_maze          = nullptr; // TODO change to Gamescene
    PhysicsWorld*               m_physicsWorld  = nullptr;
    float                       m_width;
    
    static const std::string    s_font_filename; // TODO: use std::filesystem
    static bool                 s_windowCreated;
};

}

#endif /* DebugInfosWindow_hpp */
