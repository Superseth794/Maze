//
//  DebugInfosWindow.hpp
//  Maze
//
//  Created by Jaraxus on 28/02/2020.
//

#pragma once

#ifndef DebugInfosWindow_hpp
# define DebugInfosWindow_hpp

# include <iostream>
# include <sstream>
# include <memory>
# include <cmath>

# include <SFML/Graphics.hpp>

# include "../Game/Maze.hpp"

namespace mz {

class Maze;
class PhysicsWorld;

class Console {
public:
    Console() = default;
    void init(float width, float height, Maze* maze = nullptr, PhysicsWorld* world = nullptr);
    
    std::unique_ptr<sf::RenderTexture> display();
    
private:
    float m_width;
    float m_height;
    
    PhysicsWorld* m_physicsWorld = nullptr;
    Maze* m_maze = nullptr; // TODO change to Gamescene
    
    static const std::string FONT_FILENAME;
    sf::Font m_font;
    
    bool initialized = false;
    static bool windowCreated;
};

}

#endif /* DebugInfosWindow_hpp */
