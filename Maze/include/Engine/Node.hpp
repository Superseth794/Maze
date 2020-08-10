//
//  Node.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#pragma once

#ifndef Node_hpp
#define Node_hpp

#include <memory>
#include <utility>

#include <SFML/Graphics.hpp>

#include "Layer.hpp"
#include "../Physics/PhysicsBody.hpp"

namespace mz {

class Layer;

class Node : public Layer { // Dynamic
public:
    Node() = default;
    Node(Node const& node) = delete;
    Node(Node && node) noexcept = default;
    
    Node& operator=(Node const& node) = delete;
    Node& operator=(Node && node) noexcept = default;
    
    virtual ~Node() = default;
    
    virtual void draw(Camera const& camera) override;
    
    virtual void update(std::uint64_t timeElapsed) override;
    
private:
    std::unique_ptr<PhysicsBody>    m_physicsBody; // TODO: add complete support for physics bodies
};

}

#endif /* Node_hpp */
