//
//  Node.cpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#include "../../include/Engine/Node.hpp"

namespace mz {

void Node::draw(sf::RenderTarget& target) {
    Layer::draw(target);
}

void Node::update(std::uint64_t timeElapsed) {
    Layer::update(timeElapsed);
}

}
