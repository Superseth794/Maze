//
//  CircleShapeNode.cpp
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

#include "../../include/Engine/CircleShapeNode.hpp"

namespace mz {

CircleShapeNode::CircleShapeNode(float radius, std::shared_ptr<sf::Texture> && texture) :
AbstractShapeNode(std::forward<std::shared_ptr<sf::Texture>>(texture), radius, std::size_t(30))
{
}

CircleShapeNode::CircleShapeNode(float radius, std::size_t pointCount, std::shared_ptr<sf::Texture> && texture) :
AbstractShapeNode<CircleShapeNode, sf::CircleShape>(std::forward<std::shared_ptr<sf::Texture>>(texture), radius, pointCount)
{
}

}
