//
//  Node.cpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#include "../../include/Engine/Node.hpp"

namespace mz {

void Node::draw(Camera const& camera) {
    // TODO: draw physics bodies
    Layer::draw(camera);
}

void Node::update(std::uint64_t timeElapsed) {
    Layer::update(timeElapsed);
}

}
