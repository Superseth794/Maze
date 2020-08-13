//
//  ConvexShapeNode.cpp
//  Maze
//
//  Created by Jaraxus on 13/08/2020.
//

#include "ConvexShapeNode.hpp"

namespace mz {

ConvexShapeNode::ConvexShapeNode(std::size_t pointCount, std::shared_ptr<sf::Texture> && texture) :
AbstractShapeNode<ConvexShapeNode>(std::forward<std::shared_ptr<sf::Texture>>(texture), pointCount)
{
}

ConvexShapeNode::ConvexShapeNode(std::initializer_list<sf::Vector2f> && vertexes, std::shared_ptr<sf::Texture> && texture) :
AbstractShapeNode<ConvexShapeNode>(std::forward<std::shared_ptr<sf::Texture>>(texture), vertexes.size())
{
    for (std::size_t vertexId = 0; vertexId < vertexes.size(); ++vertexId)
        m_shape.setPoint(vertexId, *(vertexes.begin() + vertexId));
}

} // namespace mz
