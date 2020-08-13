//
//  ConvexShapeNode.hpp
//  Maze
//
//  Created by Jaraxus on 13/08/2020.
//

#ifndef ConvexShapeNode_hpp
#define ConvexShapeNode_hpp

#include <initializer_list>

#include <SFML/Graphics.hpp>

#include "AbstractShapeNode.hpp"

namespace mz {

class ConvexShapeNode;

template <>
struct get_shape_node_drawable_type<ConvexShapeNode> {
    using type = sf::ConvexShape;
};

class ConvexShapeNode : public AbstractShapeNode<ConvexShapeNode> {
public:
    explicit ConvexShapeNode(std::size_t pointCount = 0, std::shared_ptr<sf::Texture> && texture = nullptr);
    explicit ConvexShapeNode(std::initializer_list<sf::Vector2f> && vertexes, std::shared_ptr<sf::Texture> && texture = nullptr);
    template <typename ...Args, typename _ = typename std::enable_if<std::conjunction<std::is_same<sf::Vector2f, Args>...>::value>>
    explicit ConvexShapeNode(Args && ...args);
    
    inline void setPoint(std::size_t index, sf::Vector2f const& point);
    
    inline void setPointCount(std::size_t count);
};

} // namespace mz

#include "ConvexShapeNode.inl"

#endif /* ConvexShapeNode_hpp */
