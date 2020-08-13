//
//  Maze.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#include "../../include/Game/Maze.hpp"

namespace mz {

Maze::Maze(unsigned int width, unsigned int height) :
m_width(width),
m_height(height),
m_window(sf::VideoMode(m_width, m_height), "Maze"),
m_physicsWorld(),
m_gameClock(),
m_player(width / 50.f, EntitiesBitMasks::PLAYER_MASK, &m_physicsWorld),
m_cameraPosition(0.f, 0.f),
m_scene(width, height)
{
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

    // Draw physics
    auto physicsTexture {m_physicsWorld.getPhysicsTexture(m_width, m_height, m_cameraPosition)};
    sf::Sprite physicsSprite;
    physicsSprite.setTexture(physicsTexture->getTexture());
    physicsSprite.setPosition(0.f, 0.f);
    m_window.draw(physicsSprite);
    
    // Draw console
    if (s_show_console) {
        auto consoleTexture {m_console->display()};
        sf::Sprite consoleSprite;
        consoleSprite.setTexture(consoleTexture->getTexture());
        consoleSprite.setPosition(0.f, 0.f);
        m_window.draw(consoleSprite);
    }
}

void Maze::drawEntity(DrawableEntity const& entity, sf::Sprite & entitySprite) {
    sf::Vector2f relativePos {
        entity.getPosition().x - m_cameraPosition.x + m_width / 2.f,
        entity.getPosition().y - m_cameraPosition.y + m_height / 2.f
    };
    entitySprite.setPosition(relativePos);
    m_window.draw(entitySprite);
}


void Maze::generateMaze() {
    struct Cell {
        int x, y;
        bool visited = false;
    };
    
    std::vector<Cell> cells;
    std::vector<bool> walls;
    
    for (int y = 0; y < s_mazeHeight + 2; ++y) {
        for (int x = 0; x < s_mazeWidth + 2; ++x) {
            walls.push_back(true);
            cells.push_back({x, y, (x == 0 || y == 0 || x == s_mazeWidth + 1 || y == s_mazeHeight + 1)});
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
                else if (cell.x + 2 * dx < 0 || cell.x + 2 * dx >= s_mazeWidth + 2 || cell.y + 2 * dy < 0 || cell.y + 2 * dy >= s_mazeHeight + 2)
                    continue;
                else if (!cells[cell.x + 2 * dx + (cell.y + 2 * dy) * (s_mazeWidth + 2)].visited)
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
        walls[currentCell.x + currentCell.y * (s_mazeWidth + 2)] = false;
        auto orientation {pickRandomOrientation(currentCell)};
        if (orientation != Orientation::UNDEFINED) {
            walls[currentCell.x + orientation.toVector().x + (currentCell.y + orientation.toVector().y) * (s_mazeWidth + 2)] = false;
            cells[currentCell.x + orientation.toVector().x * 2 + (currentCell.y + orientation.toVector().y * 2) * (s_mazeWidth + 2)].visited = true;
            toVisitCells.push({static_cast<int>(currentCell.x + orientation.toVector().x * 2), static_cast<int>(currentCell.y + orientation.toVector().y * 2)});
        } else {
            toVisitCells.pop();
        }
    }
    
    auto filledWallTexture {std::make_shared<sf::RenderTexture>()};
    filledWallTexture->create(s_wallWidth, s_wallHeight);
    filledWallTexture->clear(sf::Color::White);
    filledWallTexture->display();
    auto filledPhysicsBody = new RectanglePhysicsBody(s_wallWidth, s_wallHeight, sf::Vector2f{0.f, 0.f}, WALLS_MASK, &m_physicsWorld);
    
    auto emptyWallTexture {std::make_shared<sf::RenderTexture>()};
    emptyWallTexture->create(s_wallWidth, s_wallHeight);
    emptyWallTexture->clear(sf::Color::Black);
    emptyWallTexture->display();
    auto emptyPhysicsBody = nullptr;
    
    auto filledWallModel {std::make_shared<TileModel>(s_wallWidth, s_wallHeight, std::move(filledWallTexture), filledPhysicsBody)};
    auto emptyWallModel {std::make_shared<TileModel>(s_wallWidth, s_wallHeight, std::move(emptyWallTexture), emptyPhysicsBody)};
    
    for (int y = 0; y < s_mazeHeight + 2; ++y) {
        for (int x = 0; x < s_mazeWidth + 2; ++x) {
            m_tiles.push_back(std::make_unique<Tile>(x * s_wallWidth, y * s_wallHeight, (walls[x + y * (s_mazeWidth + 2)] ? filledWallModel : emptyWallModel)));
            m_physicsWorld.addBody(m_tiles.back()->getPhysicsBody());
        }
    }
}

void Maze::handleEvent(sf::Event const& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            m_player.orientedMove(Orientation::UP, true);
        } else if (event.key.code == sf::Keyboard::Right) {
            m_player.orientedMove(Orientation::RIGHT, true);
        } else if (event.key.code == sf::Keyboard::Down) {
            m_player.orientedMove(Orientation::DOWN, true);
        } else if (event.key.code == sf::Keyboard::Left) {
            m_player.orientedMove(Orientation::LEFT, true);
        }
    } else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Up) {
            m_player.orientedMove(Orientation::UP, false);
        } else if (event.key.code == sf::Keyboard::Right) {
            m_player.orientedMove(Orientation::RIGHT, false);
        } else if (event.key.code == sf::Keyboard::Down) {
            m_player.orientedMove(Orientation::DOWN, false);
        } else if (event.key.code == sf::Keyboard::Left) {
            m_player.orientedMove(Orientation::LEFT, false);
        }
    }
}

void Maze::init() {
    m_physicsWorld.init((s_mazeWidth + 2) * s_wallWidth, (s_mazeHeight + 2) * s_wallHeight);
    
    m_physicsWorld.setShowPhysicsBodies(false);
    m_physicsWorld.setShowAABBs(false);
    m_physicsWorld.setShowOOBBs(false);
    m_physicsWorld.setShowCollisions(false);
    m_physicsWorld.setShowQuadtree(false);
    m_physicsWorld.setShowQuadtreeEvents(false);
    
    generateMaze();
    
    m_player.move(sf::Vector2f{s_wallWidth * 1.5f, s_wallHeight * 1.5f});
    m_player.getPhysicsBody()->addContactTestBitMask(EntitiesBitMasks::WALLS_MASK);
    m_physicsWorld.addBody(m_player.getPhysicsBody());
    m_physicsWorld.addBodyQuadtreeUpdateEvent(m_player.getPhysicsBody());
//    m_physicsWorld.addBodyQuadtreeAdditionEvent(m_player.getPhysicsBody());
    
    auto b1 = std::make_unique<SegmentPhysicsBody>(sf::Vector2f{-400.f, -400.f}, sf::Vector2f{200.f, 200.f}, 15, &m_physicsWorld);
    b1->addContactTestBitMask(EntitiesBitMasks::DEBUG_MASK);
//    m_physicsWorld.addBody(b1.get());
    debug_bodies.emplace_back(std::move(b1));
    auto b2 = std::make_unique<SegmentPhysicsBody>(sf::Vector2f{200.f, 0.f}, sf::Vector2f{0.f, 200.f}, 16, &m_physicsWorld);
    b2->addContactTestBitMask(EntitiesBitMasks::DEBUG_MASK);
//    m_physicsWorld.addBody(b2.get());
    debug_bodies.emplace_back(std::move(b2));
    m_player.getPhysicsBody()->addContactTestBitMask(EntitiesBitMasks::DEBUG_MASK);
    m_player.getPhysicsBody()->addContactTestBitMask(EntitiesBitMasks::DEBUG_MASK);
    
    if (s_show_console) {
        m_console = std::make_unique<Console>();
        m_console->init(500.f, 345.f, this, &m_physicsWorld);
    }
}

void Maze::lauch() {
    m_window.setFramerateLimit(60);
    m_gameClock.restart();
    
    init();
    
    auto shapesLayer = std::make_unique<RectangleShapeNode>(1900, 1400);
    shapesLayer->setFillColor(sf::Color(50, 50, 50, 150));
    shapesLayer->setPosition(400, 200);
    shapesLayer->setOrigin(100, 100);
    auto& shapesLayerRef = m_scene.getMainLayer().addChild(std::move(shapesLayer));
    
    auto originCircle = std::make_unique<CircleShapeNode>(10);
    auto& originCircleRef = shapesLayerRef.addChild(std::move(originCircle));
    originCircleRef.setFillColor(sf::Color::White);
    
    auto circle = std::make_unique<CircleShapeNode>(40);
    auto& circleRef = shapesLayerRef.addChild(std::move(circle));
    circleRef.setFillColor(sf::Color::Red);
    circleRef.setPosition(sf::Vector2f{150, 100});
    
    auto triangle = std::make_unique<TriangleShapeNode>(80);
    triangle->setFillColor(sf::Color::Cyan);
    triangle->setPosition(150, 300);
    shapesLayerRef.addChild(std::move(triangle));
    
    auto rectangle = std::make_unique<RectangleShapeNode>(sf::Vector2f{225, 75});
    auto& rectangleRef = shapesLayerRef.addChild(std::move(rectangle));
    rectangleRef.setFillColor(sf::Color::Blue);
    rectangleRef.setOutlineColor(sf::Color::Yellow);
    rectangleRef.setOutlineThickness(-5.f);
    rectangleRef.setPosition(150, 500);
    rectangleRef.setOrigin(rectangleRef.getSize().x / 2.f, rectangleRef.getSize().y / 2.f);
    
    auto pentagon = std::make_unique<PentagonShapeNode>(80);
    auto& pentagonRef = shapesLayerRef.addChild(std::move(pentagon));
    pentagonRef.setFillColor(sf::Color::Cyan);
    pentagonRef.setPosition(150, 700);
    
    auto convex = std::make_unique<ConvexShapeNode>(sf::Vector2f{0, 0},
                                                    sf::Vector2f{100, 0},
                                                    sf::Vector2f{100, 100},
                                                    sf::Vector2f{50, 200},
                                                    sf::Vector2f{0, 100});
    auto& convexRef = shapesLayerRef.addChild(std::move(convex));
    convexRef.setPosition(150, 900);
    convexRef.setFillColor(sf::Color::Yellow);
    
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
        std::uint64_t timeElapsed = static_cast<std::uint64_t>(m_gameClock.getElapsedTime().asMicroseconds());
        m_fps = 1.f / timeElapsed * 1000.f * 1000.f; // TODO show_fps debug var
        m_gameClock.restart();
        
        shapesLayerRef.move(1.f, 0.f);
        
//        update();
//        updateCamera();
        m_scene.update(timeElapsed);
        
//         Display
        m_window.clear();
        
//        display();
        m_scene.display(m_window);
        
        m_window.display();
    }
}

void Maze::update() {
    auto timeElpased {m_gameClock.getElapsedTime()};
    m_player.update(timeElpased);
    m_physicsWorld.simulate();
}

void Maze::updateCamera() {
    constexpr float marging = 0.4f;
    sf::Vector2f margingAllowed = {
        (1.f - marging) * m_width / 2.f,
        (1.f - marging) * m_height / 2.f
    };
    
    // Clamps camera around player
    m_cameraPosition.x = std::clamp(m_cameraPosition.x, m_player.getPosition().x - margingAllowed.x, m_player.getPosition().x + margingAllowed.x);
    m_cameraPosition.y = std::clamp(m_cameraPosition.y, m_player.getPosition().y - margingAllowed.y, m_player.getPosition().y + margingAllowed.y);
    
    // Clamps camera inside maze // DEBUG
    m_cameraPosition.x = std::clamp(m_cameraPosition.x, m_width / 2.f, s_wallWidth * (s_mazeWidth + 2) - m_width / 2.f);
    m_cameraPosition.y = std::clamp(m_cameraPosition.y, m_height / 2.f, s_wallHeight * (s_mazeHeight + 2) - m_height / 2.f);
}

}
