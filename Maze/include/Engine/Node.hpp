//
//  Node.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#ifndef Node_hpp
#define Node_hpp

#include <memory>
#include <utility>

#include <SFML/Graphics.hpp>

#include "Layer.hpp"
#include "../Physics/PhysicsBody.hpp"

namespace mz {

class Node : public Layer { // Dynamic
    Node() = default;
    Node(Layer const& node) = delete;
    Node(Node && node) noexcept = default;
    
    Node& operator=(Node const& node) = delete;
    Node& operator=(Node && node) noexcept = default;
    
    virtual ~Node() = default;
    
    virtual void draw(sf::RenderTarget& target) override;
    
    virtual void update(std::uint64_t timeElapsed) override;
    
private:
    std::unique_ptr<PhysicsBody>    m_physicsBody; // TODO: add complete support for physics bodies
};

}

#endif /* Node_hpp */
