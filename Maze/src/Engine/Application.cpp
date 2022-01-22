//
//  Application.cpp
//  Maze
//
//  Created by Jaraxus on 13/09/2021.
//

#include "../../include/Engine/Application.hpp"

mz::Application::Application(unsigned width, unsigned height, std::string title) :
Application(sf::VideoMode(width, height), std::move(title))
{
}

mz::Application::Application(sf::VideoMode mode, std::string title, sf::Uint32 style, sf::ContextSettings const& settings) :
m_gameScene(mode.width, mode.height),
m_lastFrameTime(getCurrentFrameTime()),
m_title(std::move(title)),
m_window(mode, m_title, style, settings)
{
    m_window.setFramerateLimit(60);
}

void mz::Application::run() {
    while (runOnce());
}

bool mz::Application::runOnce() {
    if (m_exiting) {
        // clear gameScene
        return true;
    }
    
    std::uint64_t currentFrameTime = getCurrentFrameTime();
    std::uint64_t deltaTime = m_lastFrameTime - currentFrameTime;
    m_lastFrameTime = currentFrameTime;
    
    // handle events
    m_gameScene.update(deltaTime);
    
    m_window.clear();
    m_gameScene.display(m_window);
    m_window.display();
    
    return !m_exiting; // check gameScene
}
