//
//  ConvexRegularPolygonShapeNode.inl
//  Maze
//
//  Created by Jaraxus on 12/08/2020.
//

namespace mz {

template <std::size_t EdgesCount>
ConvexRegularPolygonShapeNode<EdgesCount>::ConvexRegularPolygonShapeNode(float radius, std::shared_ptr<sf::Texture> && texture) :
AbstractShapeNode<ConvexRegularPolygonShapeNode<EdgesCount>>(std::forward<std::shared_ptr<sf::Texture>>(texture), radius, EdgesCount)
{
}

template <std::size_t EdgesCount>
inline float ConvexRegularPolygonShapeNode<EdgesCount>::getSize() const {
    return m_size;
}

template <std::size_t EdgesCount>
inline ConvexRegularPolygonShapeNode<EdgesCount>& ConvexRegularPolygonShapeNode<EdgesCount>::setSize(float size) {
    m_size = size;
//    m_shape.setRadius(getRadiusFromSize(m_size));
    return *this;
}

template <std::size_t EdgesCount>
constexpr float ConvexRegularPolygonShapeNode<EdgesCount>::getRadiusFromSize(float size) {
    return size * std::cos(2 * M_PI / EdgesCount);
}

}
