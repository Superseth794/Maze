//
//  Maze.hpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#ifndef Maze_hpp
#define Maze_hpp

# include <iostream>

# include <SFML/Graphics.hpp>

# include "Entity/DrawableEntity/Player.hpp"

namespace mz {

class Maze {
public:
    Maze(unsigned int width, unsigned int height);
    ~Maze() = default;
    
    Maze(Maze const& maze) = delete;
    Maze(Maze && maze) = delete;
    Maze& operator=(Maze const& maze) = delete;
    
    void lauch();
    
private:
    void update();
    void updateCamera();
    
    void display();
    void drawEntity(DrawableEntity const& entity, sf::Sprite & entitySprite);
    
    void handleEvent(sf::Event const& event);
    
private:
    unsigned int m_width;
    unsigned int m_height;
    
    sf::RenderWindow m_window;
    sf::Clock m_gameClock;
    
    sf::Vector2f m_cameraPosition;
    
    Player m_player;
    
};


}

#endif /* Maze_hpp */
