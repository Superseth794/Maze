//
//  DebugInfosWindow.cpp
//  Maze
//
//  Created by Jaraxus on 28/02/2020.
//

#include "Console.hpp"

namespace mz {

void Console::init(float width, float height, Maze* maze, PhysicsWorld* world) {
    if (windowCreated) {
        std::cout << "Error: debug infos window already created" << std::endl;
        return;
    }
    
    assert(m_font.loadFromFile(FONT_FILENAME));
    
    m_width = width;
    m_height = height;
    
    m_maze = maze;
    m_physicsWorld = world;
    
    Console::windowCreated = true;
    initialized = true;
}

std::unique_ptr<sf::RenderTexture> Console::display() {
    if (!initialized)
        return std::make_unique<sf::RenderTexture>();
    
    std::ostringstream infos;
    
    if (m_maze) {
        infos << std::round(m_maze->m_fps) << " fps\n";
        infos << std::round(1.f / m_maze->m_fps * 1000.f) / 1000.f << " s/frame\n";
        infos << "camera position: (" << m_maze->m_cameraPosition.x << "," << m_maze->m_cameraPosition.y << ")\n";
    }
    
    if (m_maze && m_physicsWorld) {
        infos << "\n";
    }
    
    if (m_physicsWorld) {
        const int bodiesCount = m_physicsWorld->m_bodiesCount;
        const int nodesCount = m_physicsWorld->getQuadtreeNodesCount();
        const int maxDepht = m_physicsWorld->getQuadtreeMaxDepth();
        const int intersectionsFoundCount = m_physicsWorld->getIntersectionsCount();
        
        infos << bodiesCount << " bodies\n";
        infos << bodiesCount - m_physicsWorld->getPreciseBodiesCount() << " invalid bodies\n";
        infos << nodesCount << " nodes in quadtree\n";
        infos << "max node's depth: " << maxDepht << "\n\n";
        
        infos << m_physicsWorld->m_computedCollisionsCount << " collisions computed\n";
        infos << m_physicsWorld->m_debugCollisions.size() << " collisions found\n";
        infos << intersectionsFoundCount << " intersections found";
    }
    
    sf::Text infosText;
    infosText.setFont(m_font);
    infosText.setCharacterSize(25.f);
    infosText.setString(infos.str());
    infosText.setFillColor(sf::Color::White);
    infosText.setPosition(8.f, 5.f);
    
    auto infosTexture {std::make_unique<sf::RenderTexture>()};
    infosTexture->create(m_width, m_height);
    infosTexture->clear(sf::Color(0, 0, 0, 120));
    infosTexture->draw(infosText);
    infosTexture->display();
    
    return infosTexture;
}

std::string const Console::FONT_FILENAME = "../../Resources/Fonts/Cousine/Cousine-Regular.ttf";
bool Console::windowCreated = false;

}
