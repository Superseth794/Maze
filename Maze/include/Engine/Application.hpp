//
//  Application.hpp
//  Maze
//
//  Created by Jaraxus on 13/09/2021.
//

#ifndef Application_hpp
#define Application_hpp

#include <string>

#include <SFML/Graphics.hpp>

#include "GameScene.hpp"

namespace mz {

class Application {
public:
    Application(unsigned width, unsigned height, std::string title);
    Application(sf::VideoMode mode, std::string title, sf::Uint32 style = sf::Style::Default, sf::ContextSettings const& settings = sf::ContextSettings());
    
    inline const GameScene& getScene() const noexcept;
    
    inline GameScene& getScene() noexcept;
    
    void run();
    
    template <typename F>
    void run(F && callback);
    
    bool runOnce();
    
    inline void quit();
    
private:
    inline std::uint64_t getCurrentFrameTime() const;
    
private:
    bool                m_exiting = false;
    GameScene           m_gameScene;
    std::uint64_t       m_lastFrameTime;
    std::string         m_title;
    sf::RenderWindow    m_window;
};

} // mz

#include "Application.inl"

#endif /* Application_hpp */
