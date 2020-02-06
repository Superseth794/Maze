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
m_height(height),
m_window(sf::VideoMode(m_width, m_height), "Maze"),
m_gameClock(),
m_player(width / 50.f),
m_cameraPosition(0.f, 0.f)
{}

void Maze::lauch() {
    m_window.setFramerateLimit(60);
    m_gameClock.restart();
    
    while (m_window.isOpen()) {
        
        sf::Event event;
        while (m_window.pollEvent(event)) {
            
            if (event.type == sf::Event::Closed) {
                m_window.close();
            } else {
                handleEvent(event);
            }
            
        }
        
        // update
        update();
        updateCamera();
        m_gameClock.restart();
        
        // Display
        m_window.clear();
        
        display();
        
        m_window.display();
    }
}

void Maze::update() {
    auto timeElpased {m_gameClock.getElapsedTime()};
    m_player.update(timeElpased);
}

void Maze::updateCamera() {
    constexpr float marging = 0.4f; // Marging not allowed
    sf::Vector2f margingAllowed = {
        (1.f - marging) * m_width / 2.f,
        (1.f - marging) * m_height / 2.f
    };
    m_cameraPosition.x = std::clamp(m_cameraPosition.x, m_player.getPosition().x - margingAllowed.x, m_player.getPosition().x + margingAllowed.x);
    m_cameraPosition.y = std::clamp(m_cameraPosition.y, m_player.getPosition().y - margingAllowed.y, m_player.getPosition().y + margingAllowed.y);
}

void Maze::display() {
    auto playerTexture {m_player.draw()};
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture->getTexture());
    drawEntity(m_player, playerSprite);
}

void Maze::drawEntity(DrawableEntity const& entity, sf::Sprite & entitySprite) {
    sf::Vector2f relativePos {
        entity.getPosition().x - m_cameraPosition.x + m_width / 2.f,
        entity.getPosition().y - m_cameraPosition.y + m_height / 2.f
    };
    entitySprite.setPosition(relativePos);
    m_window.draw(entitySprite);
}

void Maze::handleEvent(sf::Event const& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            m_player.move(Orientation::UP, true);
        } else if (event.key.code == sf::Keyboard::Right) {
            m_player.move(Orientation::RIGHT, true);
        } else if (event.key.code == sf::Keyboard::Down) {
            m_player.move(Orientation::DOWN, true);
        } else if (event.key.code == sf::Keyboard::Left) {
            m_player.move(Orientation::LEFT, true);
        }
    } else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Up) {
            m_player.move(Orientation::UP, false);
        } else if (event.key.code == sf::Keyboard::Right) {
            m_player.move(Orientation::RIGHT, false);
        } else if (event.key.code == sf::Keyboard::Down) {
            m_player.move(Orientation::DOWN, false);
        } else if (event.key.code == sf::Keyboard::Left) {
            m_player.move(Orientation::LEFT, false);
        }
    }
}

}
