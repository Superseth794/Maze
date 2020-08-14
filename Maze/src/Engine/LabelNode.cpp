//
//  LabelNode.cpp
//  Maze
//
//  Created by Jaraxus on 14/08/2020.
//

#include "LabelNode.hpp"

namespace mz {

LabelNode::LabelNode() :
Node(),
m_text()
{
}

LabelNode::LabelNode(std::string const& string, sf::Font const& font, unsigned int characterSize) :
Node(),
m_text(static_cast<sf::String>(string), font, characterSize)
{
}

void LabelNode::draw(Camera const& camera) {
    camera.draw(m_text, getGlobalTransform());
    Layer::draw(camera);
}

} // namespace mz
