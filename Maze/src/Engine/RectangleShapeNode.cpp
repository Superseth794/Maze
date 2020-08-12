//
//  RectangleShapeNode.cpp
//  Maze
//
//  Created by Jaraxus on 12/08/2020.
//

#include "RectangleShapeNode.hpp"

namespace mz {

RectangleShapeNode::RectangleShapeNode(sf::Vector2f const& size, std::shared_ptr<sf::Texture> && texture) :
AbstractShapeNode(std::forward<std::shared_ptr<sf::Texture>>(texture), size)
{
}

RectangleShapeNode::RectangleShapeNode(float width, float height, std::shared_ptr<sf::Texture> && texture) :
AbstractShapeNode(std::forward<std::shared_ptr<sf::Texture>>(texture), sf::Vector2f{width, height})
{
}

}
