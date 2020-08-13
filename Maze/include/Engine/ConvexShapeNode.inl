//
//  ConvexShapeNode.inl
//  Maze
//
//  Created by Jaraxus on 13/08/2020.
//

namespace mz {

namespace Details {

[[maybe_unused]] static void setPoint(sf::ConvexShape& shape [[maybe_unused]], std::size_t vertexId [[maybe_unused]]) {}

template <typename ...Vertexes>
static inline void setPoint(sf::ConvexShape& shape, std::size_t vertexId, sf::Vector2f && vertex, Vertexes && ...vertexes) {
    shape.setPoint(vertexId, std::forward<sf::Vector2f>(vertex));
    setPoint(shape, vertexId + 1, std::forward<Vertexes>(vertexes)...);
}

} // namespace Details

template <typename ...Args, typename _>
ConvexShapeNode::ConvexShapeNode(Args && ...args) :
AbstractShapeNode(nullptr, sizeof...(args))
{
    Details::setPoint(m_shape, std::size_t(0), std::forward<Args>(args)...);
}

inline void ConvexShapeNode::setPoint(std::size_t index, sf::Vector2f const& point) {
    m_shape.setPoint(index, point);
}

inline void ConvexShapeNode::setPointCount(std::size_t count) {
    m_shape.setPointCount(count);
}

} // namespace mz
