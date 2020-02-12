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
    constexpr bool show_fps = true;
    
    m_window.setFramerateLimit(60);
    m_gameClock.restart();
    
    init();
    
    while (m_window.isOpen()) {
        
        sf::Event event;
        while (m_window.pollEvent(event)) {
            
            if (event.type == sf::Event::Closed) {
                m_window.close();
            } else {
                handleEvent(event);
            }
            
        }
        
        if (show_fps) {
            std::cout << "fps: " << 1.f / m_gameClock.getElapsedTime().asSeconds() << "\n";
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

void Maze::init() {
    generateMaze();
}

void Maze::generateMaze() {
    struct Cell {
        int x, y;
        bool visited = false;
    };
    
    std::vector<Cell> cells;
    std::vector<bool> walls;
    
    for (int y = 0; y < m_mazeHeight + 2; ++y) {
        for (int x = 0; x < m_mazeWidth + 2; ++x) {
            walls.push_back(true);
            cells.push_back({x, y, (x == 0 || y == 0 || x == m_mazeWidth + 1 || y == m_mazeHeight + 1)});
        }
    }
    
    std::stack<Cell> toVisitCells;
     toVisitCells.push({1, 1});
    
    auto pickRandomOrientation = [&cells](Cell const& cell) -> Orientation {
        std::vector<Orientation> orientationsAvailables;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if ((dx != 0 && dy != 0) || dx == dy)
                    continue;
                else if (cell.x + 2 * dx < 0 || cell.x + 2 * dx >= m_mazeWidth + 2 || cell.y + 2 * dy < 0 || cell.y + 2 * dy >= m_mazeHeight + 2)
                    continue;
                else if (!cells[cell.x + 2 * dx + (cell.y + 2 * dy) * (m_mazeWidth + 2)].visited)
                    orientationsAvailables.push_back(Orientation::getOrientation(sf::Vector2i{dx, dy}));
            }
        }
        
        if (orientationsAvailables.size() == 0)
            return Orientation::UNDEFINED;
        else
            return orientationsAvailables[rand() % orientationsAvailables.size()];
    };
    
    while (!toVisitCells.empty()) {
        Cell currentCell {toVisitCells.top()};
        currentCell.visited = true;
        walls[currentCell.x + currentCell.y * (m_mazeWidth + 2)] = false;
        auto orientation {pickRandomOrientation(currentCell)};
        std::cout << currentCell.x << " " << currentCell.y << " " << orientation.getName() << std::endl;
        if (orientation != Orientation::UNDEFINED) {
            walls[currentCell.x + orientation.toVector().x + (currentCell.y + orientation.toVector().y) * (m_mazeWidth + 2)] = false;
            cells[currentCell.x + orientation.toVector().x * 2 + (currentCell.y + orientation.toVector().y * 2) * (m_mazeWidth + 2)].visited = true;
            toVisitCells.push({static_cast<int>(currentCell.x + orientation.toVector().x * 2), static_cast<int>(currentCell.y + orientation.toVector().y * 2)});
        } else {
            toVisitCells.pop();
        }
    }
    
    auto filledWallTexture {std::make_shared<sf::RenderTexture>()};
    filledWallTexture->create(m_wallWidth, m_wallHeight);
    filledWallTexture->clear(sf::Color::White);
    
    auto emptyWallTexture {std::make_shared<sf::RenderTexture>()};
    emptyWallTexture->create(m_wallWidth, m_wallHeight);
    emptyWallTexture->clear(sf::Color::Black);
    
    auto filledWallModel {std::make_shared<TileModel>(m_wallWidth, m_wallHeight, std::move(filledWallTexture))};
    auto emptyWallModel {std::make_shared<TileModel>(m_wallWidth, m_wallHeight, std::move(emptyWallTexture))};
    
    for (int y = 0; y < m_mazeHeight + 2; ++y) {
        for (int x = 0; x < m_mazeWidth + 2; ++x) {
            m_tiles.push_back(std::make_unique<Tile>(x * m_wallWidth, y * m_wallHeight, (walls[x + y * (m_mazeWidth + 2)] ? filledWallModel : emptyWallModel)));
        }
    }
    
    m_backgroundMazeTexture = std::make_unique<sf::RenderTexture>();
    m_backgroundMazeTexture->create((m_mazeWidth + 3) * m_wallWidth, (m_mazeHeight + 3) * m_wallHeight);
    m_backgroundMazeTexture->clear(sf::Color::Red);
    for (auto const& tile : m_tiles) {
        auto tileTexture {tile->draw()};
        sf::Sprite tileSprite;
        tileSprite.setTexture(tileTexture->getTexture());
        tileSprite.setPosition(tile->getPosition().x, tile->getPosition().y);
        m_backgroundMazeTexture->draw(tileSprite);
    }
    m_backgroundMazeTexture->display();
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
    
    // Clamps camera around player
    m_cameraPosition.x = std::clamp(m_cameraPosition.x, m_player.getPosition().x - margingAllowed.x, m_player.getPosition().x + margingAllowed.x);
    m_cameraPosition.y = std::clamp(m_cameraPosition.y, m_player.getPosition().y - margingAllowed.y, m_player.getPosition().y + margingAllowed.y);
    
    // Clamps camera inside maze
    m_cameraPosition.x = std::clamp(m_cameraPosition.x, m_width / 2.f, m_wallWidth * (m_mazeWidth + 2) - m_width / 2.f);
    m_cameraPosition.y = std::clamp(m_cameraPosition.y, m_height / 2.f, m_wallHeight * (m_mazeHeight + 2) - m_height / 2.f);
}

void Maze::display() {
    // Draws tiles
    for (auto const& tile : m_tiles) {
        auto tileTexture {tile->draw()};
        sf::Sprite tileSprite;
        tileSprite.setTexture(tileTexture->getTexture());
        drawEntity(*tile, tileSprite);
    }
    
    // Draws player
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
