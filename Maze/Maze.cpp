//
//  Maze.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include "Maze.hpp"

namespace mz {

Maze::Maze(unsigned int width, unsigned int height) :
m_width(width),
m_height(height)
{}

void Maze::lauch() {
    sf::RenderWindow window(sf::VideoMode(m_width, m_height), "Maze");
    
    window.setFramerateLimit(60);
    
    while (window.isOpen()) {
        
        sf::Event event;
        while (window.pollEvent(event)) {
            
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                
            }
            
        }
        
        // update
        update();
        
        // Display
        window.clear();
        
        window.display();
    }
}

void Maze::update() {
    
}

void Maze::handleEvent(sf::Event const& event) {
    
    // TODO handle events
    
}

}
