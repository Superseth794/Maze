//
//  CircleShapeNode.inl
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

namespace mz {

inline float CircleShapeNode::getRadius() const {
    return m_shape.getRadius();
}

inline void CircleShapeNode::setPointCount(std::size_t count) {
    m_shape.setPointCount(count);
}

inline void CircleShapeNode::setRadius(float radius) {
    m_shape.setRadius(radius);
}

}
