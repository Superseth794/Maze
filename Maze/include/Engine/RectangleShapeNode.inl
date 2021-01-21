//
//  RectangleShapeNode.inl
//  Maze
//
//  Created by Jaraxus on 12/08/2020.
//

namespace mz {

inline sf::Vector2f const& RectangleShapeNode::getSize() const {
    return m_shape.getSize();
}

inline RectangleShapeNode& RectangleShapeNode::setSize(sf::Vector2f const& size) {
    m_shape.setSize(size);
    return *this;
}

}
